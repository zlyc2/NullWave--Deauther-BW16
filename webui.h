#ifndef WEBUI_H
#define WEBUI_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <vector>

// Definisikan struct di sini biar bisa diakses sama file utama
struct WiFiScanResult {
  String ssid;
  uint8_t bssid[6];
  short rssi;
  uint8_t channel;
};

void handleRoot(WiFiClient &client, std::vector<WiFiScanResult> &scan_results) {
  client.println("HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n\n");
  client.println("<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<title>NullWave BW16</title>");
  client.println("<style>");
  client.println("body{background:#0b0f19;color:#e2e8f0;font-family:system-ui,sans-serif;margin:0;padding:20px;}");
  client.println(".container{max-width:800px;margin:auto;}");
  client.println(".card{background:#151e2e;border:1px solid #1e293b;border-radius:12px;margin-bottom:20px;overflow:hidden;}");
  client.println(".card-header{padding:15px;background:rgba(30,41,59,0.4);border-bottom:1px solid #1e293b;font-weight:bold;font-size:14px;text-transform:uppercase;letter-spacing:1px;}");
  client.println(".band-24{border-left:4px solid #10b981;} .band-5{border-left:4px solid #06b6d4;}");
  client.println("table{width:100%;border-collapse:collapse;font-size:14px;}");
  client.println("th,td{padding:12px 15px;text-align:left;border-bottom:1px solid #1e293b;}");
  client.println("th{color:#94a3b8;font-weight:500;font-size:12px;text-transform:uppercase;}");
  client.println("input[type=checkbox]{width:18px;height:18px;cursor:pointer;accent-color:#6366f1;}");
  client.println(".btn-launch{width:100%;background:#ef4444;color:white;border:none;padding:18px;border-radius:8px;font-size:16px;font-weight:bold;cursor:pointer;text-transform:uppercase;letter-spacing:1px;box-shadow:0 4px 6px rgba(239,68,68,0.3);} ");
  client.println(".btn-refresh{background:#1e293b;color:#e2e8f0;border:1px solid #334155;padding:10px 15px;border-radius:8px;cursor:pointer;font-size:14px;text-decoration:none;display:inline-block;margin-bottom:20px;}");
  client.println(".rssi-good{color:#10b981;} .rssi-fair{color:#f59e0b;} .rssi-poor{color:#f43f5e;}");
  client.println("</style></head><body>");
  
  client.println("<div class='container'>");
  client.println("<div style='display:flex;justify-content:space-between;align-items:center;margin-bottom:20px;'>");
  client.println("<h1 style='margin:0;font-size:24px;'><span style='color:#818cf8;'>Null</span>Wave</h1>");
  client.println("<a href='/scan' class='btn-refresh'>Refresh Scan</a>");
  client.println("</div>");

  client.println("<form action='/deauth' method='POST'>");
  
  // --- TABEL 2.4 GHz ---
  client.println("<div class='card band-24'><div class='card-header'>2.4 GHz Band</div>");
  client.println("<table><tr><th>Select</th><th>SSID</th><th>CH</th><th>RSSI</th></tr>");
  for (uint32_t i = 0; i < scan_results.size(); i++) {
    if (scan_results[i].channel <= 14) {
      client.print("<tr><td><input type='checkbox' name='network' value='"); client.print(i); client.print("'></td>");
      client.print("<td>"); client.print(scan_results[i].ssid); client.print("</td>");
      client.print("<td>"); client.print(scan_results[i].channel); client.print("</td>");
      client.print("<td class='"); 
      if (scan_results[i].rssi > -60) client.print("rssi-good'>"); 
      else if (scan_results[i].rssi > -80) client.print("rssi-fair'>"); 
      else client.print("rssi-poor'>");
      client.print(scan_results[i].rssi); client.print(" dBm</td></tr>");
    }
  }
  client.println("</table></div>");

  // --- TABEL 5 GHz ---
  client.println("<div class='card band-5'><div class='card-header'>5 GHz Band</div>");
  client.println("<table><tr><th>Select</th><th>SSID</th><th>CH</th><th>RSSI</th></tr>");
  for (uint32_t i = 0; i < scan_results.size(); i++) {
    if (scan_results[i].channel > 14) {
      client.print("<tr><td><input type='checkbox' name='network' value='"); client.print(i); client.print("'></td>");
      client.print("<td>"); client.print(scan_results[i].ssid); client.print("</td>");
      client.print("<td>"); client.print(scan_results[i].channel); client.print("</td>");
      client.print("<td class='"); 
      if (scan_results[i].rssi > -60) client.print("rssi-good'>"); 
      else if (scan_results[i].rssi > -80) client.print("rssi-fair'>"); 
      else client.print("rssi-poor'>");
      client.print(scan_results[i].rssi); client.print(" dBm</td></tr>");
    }
  }
  client.println("</table></div>");

  client.println("<button type='submit' class='btn-launch'>⚡ Launch Attack (Fire & Forget)</button>");
  client.println("<p style='color:#f59e0b;font-size:12px;text-align:center;margin-top:10px;'>WARNING: Press RESET on BW16 to stop.</p>");
  client.println("</form></div></body></html>");
}

#endif
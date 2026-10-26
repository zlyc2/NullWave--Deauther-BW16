# NullWave BW16 - Dual-Band WiFi Deauther

![License](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Platform](https://img.shields.io/badge/Platform-BW16%20(RTL8720DN)-blue.svg)
![C++](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)
![C](https://img.shields.io/badge/Language-C-lightgrey.svg)

# NullWave BW16 - Dual-Band WiFi Deauther

A high-performance WiFi Deauthentication framework designed for the **Ai-Thinker BW16 (RTL8720DN)** microcontroller. It features an ultra-lightweight Web UI and a "Fire & Forget" mode (100% CPU focus on attacking).

## ⚠️ DISCLAIMER
**FOR EDUCATIONAL AND AUTHORIZED SECURITY TESTING (PENTEST) PURPOSES ONLY.** 
Do NOT use this tool on networks you do not own or have explicit permission to test. The author is not responsible for any misuse or legal consequences (such as violation of ITE laws) arising from the use of this software.

<img width="2752" height="537" alt="image" src="https://github.com/user-attachments/assets/a07a44da-7a5e-44e6-897b-1e25184c4d06" />

## ✨ Features
*   **Dual-Band**: Scan and attack both 2.4 GHz and 5 GHz networks.
*   **Ultra-Lightweight Web UI**: No CDN used, RAM-friendly, and works completely offline.
*   **Fire & Forget Mode**: Once "Launch" is clicked, the Web Server shuts down, and BW16 dedicates 100% of its CPU to firing Deauth packets non-stop.
*   **Hardware Stop**: Stop the attack instantly by pressing the physical **RESET/EN** button on the BW16 board.

## 🛠️ Preparation (Arduino IDE)
Before uploading, you must configure your Arduino IDE as follows:
1. **Board Manager URL**: Go to `File > Preferences`, and add this URL to *Additional Boards Manager URLs*:
   `https://github.com/ambiot/ambd_arduino/raw/master/Arduino_package/package_realtek_amebad_index.json`
2. **Install Board**: Go to `Tools > Board > Boards Manager`, search for **Ameba** (Realtek AmebaD), and install it.
3. **Tools Settings** (Required):
   *   **Board**: `Ai-Thinker BW16 (RTL8720DN)`
   *   **Port**: (Select your BW16 COM port)
   *   **Upload Speed**: `921600`
   *   **Auto Flash Mode**: `Enable` (Required to avoid manual BURN button pressing)
   *   **Erase All Flash Memory**: `Disable`

## 🚀 Quick Start
1. Upload `NullWave.ino` (along with the supporting `.h` and `.cpp` files) via Arduino IDE.
2. **REQUIRED**: Attach the antenna before powering the BW16!
3. Plug the BW16 into a Power Bank / 5V Charger.
4. Connect your phone to the WiFi: **`NullWave_BW16`** (Pass: `12345678`).
5. Open your browser and go to: **`192.168.1.1`**.
6. Click **Refresh Scan** -> Check your targets -> Click **Launch Attack**.

*(Read the full installation guide in the Wiki or discussions)*

## 📜 License
This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**. See the [LICENSE](LICENSE) file for details.

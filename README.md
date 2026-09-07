# YCODE IoT Dashboard 🎛️

A self-hosted web control panel for ESP32 — control an LED, laser, and servo motor directly from any phone or laptop browser, no app required.

## How It Works
The ESP32 runs its own Wi-Fi access point and web server. It serves a full HTML/CSS/JS dashboard directly from its memory — when you connect to its network and open its IP address, you get a live control panel with no internet connection needed.

## Features
- Toggle LED and laser on/off with visual state feedback
- Real-time servo control via a slider (0–180°)
- Fully self-contained — works with zero internet access
- Clean, responsive dashboard UI matching the YCODE design system

## Tech Stack
- ESP32 (Arduino/C++)
- WebServer library (HTTP POST endpoints)
- Vanilla HTML/CSS/JS (served directly from the microcontroller)

## Project Structure
ycode-iot-dashboard/
└── esp32_dashboard.ino   # Full firmware: Wi-Fi AP, web server, and embedded dashboard UI

## How to Run
1. Flash esp32_dashboard.ino to the ESP32
2. Connect your phone/laptop to the ESP32's Wi-Fi network (check Serial Monitor for SSID)
3. Open the printed IP address (e.g. 192.168.4.1) in any browser
4. Control the LED, laser, and servo live from the dashboard

---
🇪🇬 [النسخة العربية](README.ar.md)

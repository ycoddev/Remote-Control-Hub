#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#define led 2
#define laser 4

Servo myservo;

WebServer server(80);

void handleRoot() {
    server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>YCODE Dashboard</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: -apple-system, 'Segoe UI', Roboto, sans-serif;
        }

        body {
            background: radial-gradient(circle at 30% 20%, #0d1830 0%, #05070d 60%);
            color: #eef2f7;
            min-height: 100dvh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }

        .center {
            width: min(340px, 100%);
            background: #0b1220;
            border: 1px solid #17233d;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.45);
            padding: 26px 24px;
        }

        .brand {
            text-align: center;
            font-weight: 700;
            font-size: 18px;
            letter-spacing: 0.02em;
            margin-bottom: 22px;
        }
        .brand .y { color: #7cc4ff; }

        .row {
            margin-bottom: 20px;
            padding-bottom: 18px;
            border-bottom: 1px solid #17233d;
        }
        .row:last-child {
            border-bottom: none;
            margin-bottom: 0;
            padding-bottom: 0;
        }

        .row-label {
            display: flex;
            justify-content: space-between;
            align-items: center;
            font-size: 14px;
            color: #7c8aa0;
            margin-bottom: 12px;
        }

        .toggle-btn {
            width: 100%;
            height: 44px;
            border: none;
            border-radius: 10px;
            background: #17233d;
            color: #eef2f7;
            font-size: 15px;
            font-weight: 600;
            cursor: pointer;
            transition: background 0.15s ease, color 0.15s ease;
        }
        .toggle-btn.on {
            background: #2f8fff;
            color: #04101f;
        }

        input[type="range"] {
            -webkit-appearance: none;
            width: 100%;
            height: 6px;
            border-radius: 4px;
            background: #17233d;
            outline: none;
        }
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #2f8fff;
            cursor: pointer;
        }

        .angle-value {
            font-family: monospace;
            color: #7cc4ff;
        }
    </style>
</head>
<body>
    <div class="center">
        <div class="brand"><span class="y">Y</span>CODE Dashboard</div>

        <div class="row">
            <div class="row-label"><span>LED</span></div>
            <button class="toggle-btn" id="btnled">Off</button>
        </div>

        <div class="row">
            <div class="row-label"><span>Servo</span><span class="angle-value" id="angleLabel">0°</span></div>
            <input type="range" min="0" max="180" value="0" id="Servo">
        </div>

        <div class="row">
            <div class="row-label"><span>Laser</span></div>
            <button class="toggle-btn" id="btnlaser">Off</button>
        </div>
    </div>
    <script>
const btnled = document.getElementById("btnled");
const Servo = document.getElementById("Servo");
const btnlaser = document.getElementById("btnlaser");
const angleLabel = document.getElementById("angleLabel");

let ledOn = false;

btnled.onclick = function(){
    ledOn = !ledOn;
    if(ledOn){
        btnled.classList.add("on");
        btnled.textContent = "On";
        fetch("/on", {method:"POST"});
    } else {
        btnled.classList.remove("on");
        btnled.textContent = "Off";
        fetch("/off", {method:"POST"});
    }
}

Servo.oninput = function(){
    const number = Servo.value;
    angleLabel.textContent = number + "°";
    fetch(`/servo?angle=${number}`, {method:"POST"});
}

let laserOn = false;

btnlaser.onclick = function(){
    laserOn = !laserOn;
    if(laserOn){
        btnlaser.classList.add("on");
        btnlaser.textContent = "On";
        fetch("/laseron", {method:"POST"});
    } else {
        btnlaser.classList.remove("on");
        btnlaser.textContent = "Off";
        fetch("/laseroff", {method:"POST"});
    }
}
    </script>
</body>
</html>
    )rawliteral");
}

void Led_on(){
    digitalWrite(led, HIGH);
    server.send(200, "text/plain", "LED ON");
}

void Led_off(){
    digitalWrite(led, LOW);
    server.send(200, "text/plain", "LED off");
}

void laser_on(){
    digitalWrite(laser, HIGH);
    server.send(200, "text/plain", "laser ON");
}

void laser_off(){
    digitalWrite(laser, LOW);
    server.send(200, "text/plain", "laser OFF");
}

void setup() {
    Serial.begin(115200);
    pinMode(led, OUTPUT);
    pinMode(laser, OUTPUT);
    myservo.attach(5);
    myservo.write(0);
    WiFi.softAP("YOYO", "your_password_here");
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
    server.on("/servo", HTTP_POST, []() {
      int angle = server.arg("angle").toInt();
      myservo.write(angle);
      server.send(200, "text/plain", "OK");
    });
    server.on("/", HTTP_GET, handleRoot);
    server.on("/on", HTTP_POST, Led_on);
    server.on("/off", HTTP_POST, Led_off);
    server.on("/laseroff", HTTP_POST, laser_off);
    server.on("/laseron", HTTP_POST, laser_on);
    server.begin();
    Serial.println("Server started");
}

void loop() {
  server.handleClient();
}

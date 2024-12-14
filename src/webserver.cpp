#ifndef USE_WEBSERVER_STANDALONE
#define USE_WEBSERVER_STANDALONE
#endif

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <cstdlib>

// NOTE: You will have to configure this manually. If you are on a public network,
// then it would be better to create a hotspot to connect to.
#include "headers/webserver.h"
#include "headers/secrets.h"
#include "headers/webserver_style.h"

WebServer server(80); // port 80

char armsBuffer[64];
// Raw data: v0, v1, v2; this should be processed to either XYZ or angles depending on the flag `useXYZ`
float v0, v1, v2;
float X, Y, Z;
float base, elbow, wrist;

bool grab, useXYZ;

// Initialize the LCD with the pins: RS, E, D4, D5, D6, D7
// LiquidCrystal lcd(14, 12, 27, 26, 25, 33);

/// @brief Handles the root endpoint. Values are defined as `v0`, `v1`, and `v2`
/// as either joint angles OR coordinates of the end effector.
void handleRoot() {
  String message;

  if (server.hasArg("v0")) v0 = server.arg("v0").toFloat();
  if (server.hasArg("v1")) v1 = server.arg("v1").toFloat();
  if (server.hasArg("v2")) v2 = server.arg("v2").toFloat();

  // Mode is handled separately with a different endpoint.
  grab = server.hasArg("grab") && server.arg("grab") == "1";

  // NOTE: Copy values v0, v1, v2, differentiate them XYZ or angles
  // and then send them to the motors here.

  message = "<html><head><title>CMPUT 312 3DOF Arm Controller</title>"
            "<link rel='shortcut icon' href='#'>"
            "<link rel='stylesheet' href='styles.css'>"
            "<script>"
            "function toggleMode() {"
            "  const mode = document.getElementById('mode-toggle').checked ? 1 : 0;"
            "  fetch('/toggleMode?use_xyz=' + mode)"
            "    .then(response => response.text())"
            "    .then(data => {"
            "      const isXYZ = mode === 1;"
            "      document.getElementById('label-v0').innerText = isXYZ ? 'X: ' : 'Base: ';"
            "      document.getElementById('label-v1').innerText = isXYZ ? 'Y: ' : 'Elbow: ';"
            "      document.getElementById('label-v2').innerText = isXYZ ? 'Z: ' : 'Wrist: ';"
            "    })"
            "    .catch(err => console.error('Error updating mode:', err));"
            "}"

            "let grabState = " + String(grab ? "true" : "false") + ";"
            "function toggleGrab() {"
            "  grabState = !grabState;"
            "  document.getElementById('grab-button').innerText = grabState ? 'Grab: ON' : 'Grab: OFF';"
            "  document.getElementById('grab-input').value = grabState ? '1' : '0';"
            "}"

            // Continually fetch internal data from the arm.
            "function updateInfo() {"
            "  fetch('/getInfo')"
            "    .then(response => response.json())"
            "    .then(data => {"
            "      document.getElementById('p-v0').innerText = (data.use_xyz_mode ? 'X: ' : 'Base: ')  + data.v0;"
            "      document.getElementById('p-v1').innerText = (data.use_xyz_mode ? 'Y: ' : 'Elbow: ') + data.v1;"
            "      document.getElementById('p-v2').innerText = (data.use_xyz_mode ? 'Z: ' : 'Wrist: ') + data.v2;"
            "      document.getElementById('p-grab').innerText = 'Grab: ' + (data.grab ? 'ON' : 'OFF');"
            "    })"
            "    .catch(err => console.error('Error fetching telemetry:', err));"
            "}"

            "setInterval(updateInfo, 750);"
            "window.onload = () => {updateInfo();};"
            "</script>"

            "</head><body>"
            "<h1>3DOF Arm Control</h1>"
            "<div id='control-container'>"
            "<label>Use XYZ?</label>"
            "<input type='checkbox' id='mode-toggle' onclick='toggleMode()' " + 
            String(useXYZ ? "checked" : "") + "><br>"
            "<form method='GET' action='/'>"
            "<label id='label-v0'>" + String(useXYZ ? "X: " : "Base: ") + 
            "</label><input type='number' name='v0' step='0.1' value='" + String(v0) + "'><br>"
            "<label id='label-v1'>" + String(useXYZ ? "Y: " : "Elbow: ") + 
            "</label><input type='number' name='v1' step='0.1' value='" + String(v1) + "'><br>"
            "<label id='label-v2'>" + String(useXYZ ? "Z: " : "Wrist: ") + 
            "</label><input type='number' name='v2' step='0.1' value='" + String(v2) + "'><br>"
            "<button type='button' id='grab-button' onclick='toggleGrab()'>Grab: " + (grab ? "ON" : "OFF") + "</button><br>"
            "<input type='hidden' name='grab' id='grab-input' value='" + String(grab ? "1" : "0") + "'>"
            "<input type='submit' value='Send'>"
            "</form>"
            "</div>"
            "<h2>Telemetry</h2>"
            "<div id='information-container'>"
            "<p id='p-v0'></p>"
            "<p id='p-v1'></p>"
            "<p id='p-v2'></p>"
            "<p id='p-grab'></p>"
            "</div>"
            "</body></html>";

  server.send(200, "text/html", message);
}

/// @brief Handler to fetch arm info from the ESP32.
/// TODO: Differentiate between the ESP's XYZ/angle data and the incoming data.
/// You might have to unpack the variables and differentiate what to send.
void handleGetInfo() {
  String json = "{\"v0\": " + String(v0) + ", "
                "\"v1\": " + String(v1) + ", "
                "\"v2\": " + String(v2) + ", "
                "\"grab\": " + String(grab ? "true" : "false") + ", "
                "\"use_xyz_mode\": " + String(useXYZ ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

/// @brief Handler for toggling the mode to send data.
/// if the user toggles the mode from the UI it should be
/// expected that the telemetry values change? or just display both of them.
void handleToggleMode() {
  if (server.hasArg("use_xyz")) {
    useXYZ = server.arg("use_xyz") == "1";
    server.send(200, "text/plain", "Mode updated.");
  } else {
    server.send(400, "text/plain", "Invalid request.");
  }
}

/// @brief Handler to poll the style sheet.
void handleStyling() {
  server.send(200, "text/css", style);
}

void initWebServer() {
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi.");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Open this up in your browser.

  server.on("/", handleRoot);
  server.on("/getInfo", handleGetInfo);
  server.on("/toggleMode", handleToggleMode);
  server.on("/styles.css", handleStyling);

  server.begin();                
  Serial.println("Server started. Listening for angles.\n");
}

void handleWebServer() {
  server.handleClient();  // Listen for incoming clients
}

#ifndef USE_WEBSERVER_STANDALONE // For standalone debugging
void setup() {
  Serial.begin(115200);
  initWebServer();
}

void loop() {
  handleWebServer();
}
#endif
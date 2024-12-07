#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <cstdlib>
// #include <LiquidCrystal.h>

// NOTE: You will have to configure this. If you are on a public network,
// then it would be better to create a hotspot to connect to.
#include "secrets.h" 
#include "webserver_style.h"

WebServer server(80); // port 80

char armsBuffer[64];
// Set to int for display purposes.
int base = 0, elbow = 0, wrist = 0;
float x = 0, y = 0, z = 0;

bool grab = false, useXYZ = false;
float v0, v1, v2;

// Initialize the LCD with the pins: RS, E, D4, D5, D6, D7
// LiquidCrystal lcd(14, 12, 27, 26, 25, 33);


void handleRoot2() {
  String message;

  if (server.hasArg("v0")) v0 = server.arg("v0").toFloat();
  if (server.hasArg("v1")) v1 = server.arg("v1").toFloat();
  if (server.hasArg("v2")) v2 = server.arg("v2").toFloat();

  grab = server.hasArg("grab") && server.arg("grab") == "1";

  message = "<html><head><title>3DOF Arm Control</title>"
            "<link rel='shortcut icon' href='#'>"

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
            "<p>" + String(useXYZ ? "X: " : "Base: ") + String(v0) + "</p>"
            "<p>" + String(useXYZ ? "Y: " : "Elbow: ") + String(v1) + "</p>"
            "<p>" + String(useXYZ ? "Z: " : "Wrist: ") + String(v2) + "</p>"
            // "<p>v0: " + String(v0) + "</p>"
            // "<p>v1: " + String(v1) + "</p>"
            // "<p>v2: " + String(v2) + "</p>"
            "<p>Grab: " + (grab ? "ON " : "OFF") + "</p>"
            "</div>"
            "</body></html>";
  
  // TODO: Print internal information here at some point.

  server.send(200, "text/html", message);
}

void handleGetData() {
  String json = "{\"v0\": " + String(v0) + ", "
                "\"v1\": " + String(v1) + ", "
                "\"v2\": " + String(v2) + ", "
                "\"grab\": " + String(grab ? "true" : "false") + ", "
                "\"use_xyz_mode\": " + String(useXYZ ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void handleToggleMode() {
  if (server.hasArg("use_xyz")) {
    useXYZ = server.arg("use_xyz") == "1";
    server.send(200, "text/plain", "Mode updated.");
  } else {
    server.send(400, "text/plain", "Invalid request.");
  }
}

//////////

/// @brief Handler for the root '/' endpoint, containing the main
/// webpage. Also handles sending values to the robot.
void handleRoot() {

  String message;
  
  // Check if each value parameter is in the URL and update variables
  // value parameters basically modify the url
  // example: <ip>/base=1&elbow=2&wrist=3


  if (useXYZ) {
    if (server.hasArg("x")) base = server.arg("x").toFloat();
    if (server.hasArg("y")) elbow = server.arg("y").toFloat();
    if (server.hasArg("z")) wrist = server.arg("z").toFloat();
  } else {
    if (server.hasArg("base")) base = server.arg("base").toInt();
    if (server.hasArg("elbow")) elbow = server.arg("elbow").toInt();
    if (server.hasArg("wrist")) wrist = server.arg("wrist").toInt();
  }
  
  grab = server.hasArg("grab") && server.arg("grab") == "1";

  // Dynamically update the webpage with motor angles.
message = "<html><head><title>CMPUT 312 3DOF ARM</title>"
          // "<link rel='stylesheet' href='/styles.css'>"
          "<link rel='shortcut icon' href='#'>"
          "<script>"
          "let grabState = " + String(grab ? "true" : "false") + ";"
          
          "function toggleGrab() {"
          "  grabState = !grabState;"
          "  document.getElementById('grab-button').innerText = grabState ? 'Grab: ON' : 'Grab: OFF';"
          "  document.getElementById('grab-input').value = grabState ? '1' : '0';"
          "}"

          "function toggleMode() {"
          "  const mode = document.getElementById('mode-toggle').checked ? 1 : 0;"
          "  window.location.href = '/?useXYZ=' + mode;"
          "}"

          // "function updateTelemetry() {"
          // "  fetch('/getAngles')"
          // "    .then(response => response.json())"
          // "    .then(data => {"
          // "      document.getElementById('base').innerHTML = 'Base: ' + data.base;"
          // "      document.getElementById('elbow').innerHTML = 'Elbow: ' + data.elbow;"
          // "      document.getElementById('wrist').innerHTML = 'Wrist: ' + data.wrist;"
          // "      document.getElementById('grab').innerHTML = 'Grab: ' + (data.grab ? 'ON' : 'OFF');"
          // "    })"
          // "    .catch(err => console.error('Error fetching telemetry:', err));"
          // "}"

          // "setInterval(updateTelemetry, 500);"
          
          // "window.onload = () => {"
          // "  updateTelemetry();"
          // "};"
          "</script>"
          "</head><body>"
          "<h1>Arm Control</h1>"
          "<label>Use XYZ?</label>"
          "<input type='checkbox' id='mode-toggle' onclick='toggleMode()' " + String(useXYZ ? "checked" : "") + "><br>"
          "<div id='form-container'>"
          "<form method='GET' action='/'>"
          "<label>Base: </label><input type='number' name='base' min='0' max='360' placeholder='Base angle' value='" + String(base) + "'><br>"
          "<label>Elbow: </label><input type='number' name='elbow' min='0' max='360' placeholder='Elbow angle' value='" + String(elbow) + "'><br>"
          "<label>Wrist: </label><input type='number' name='wrist' min='0' max='360' placeholder='Wrist angle' value='" + String(wrist) + "'><br>"
          "<button type='button' id='grab-button' onclick='toggleGrab()'>Grab: " + (grab ? "ON" : "OFF") + "</button><br>"
          "<input type='hidden' name='grab' id='grab-input' value='" + String(grab ? "1" : "0") + "'>"
          "<input type='submit' value='Send'/><br>"
          "</form>"
          "</div>"
          "<h2>Telemetry</h2><div id='angles'>"
          "<p id='base'>Base: " + String(base) + "</p>"
          "<p id='elbow'>Elbow: " + String(elbow) + "</p>"
          "<p id='wrist'>Wrist: " + String(wrist) + "</p>"
          "<p id='grab'>Grab: " + (grab ? "ON " : "OFF") + "</p>"
          "<p id='status'>Mode: " + (useXYZ ? "XYZ" : "ANGLES") + "</p>"
          "</div>"
          "</body></html>";

// message = "<html><head><title>CMPUT 312 3DOF ARM</title>"
//     "<link rel='stylesheet' href='/styles.css'>"
//     "<script>"
//     "let mode = 'angles';"  // Default mode is angles
//     "function toggleMode() {"
//     "  mode = mode === 'angles' ? 'xyz' : 'angles';"
//     "  document.getElementById('angles-inputs').style.display = mode === 'angles' ? 'block' : 'none';"
//     "  document.getElementById('xyz-inputs').style.display = mode === 'xyz' ? 'block' : 'none';"
//     "  document.getElementById('mode-toggle').innerText = mode === 'angles' ? 'Switch to XYZ Input' : 'Switch to Angles Input';"
//     "}"
//     "</script>"
//     "</head><body><h1>3DOF Arm Control</h1>"
//     "<button id='mode-toggle' onclick='toggleMode()'>Switch to XYZ Input</button>"
//     "<div id='form-container'>"
//     "<form method='GET' action='/'>"
    
//     // Angles Input Section
//     "<div id='angles-inputs'>"
//     "<h2>Set Angles</h2>"
//     "<label>Base:</label><input type='number' name='base' min='0' max='360' placeholder='Base angle' value='" + String(base) + "'>"
//     "<label>Elbow:</label><input type='number' name='elbow' min='0' max='360' placeholder='Elbow angle' value='" + String(elbow) + "'>"
//     "<label>Wrist:</label><input type='number' name='wrist' min='0' max='360' placeholder='Wrist angle' value='" + String(wrist) + "'>"
//     "</div>"

//     // XYZ Input Section
//     "<div id='xyz-inputs' style='display: none;'>"
//     "<h2>Set XYZ Coordinates</h2>"
//     "<label>X:</label><input type='number' name='x' placeholder='X coordinate' value='0'>"
//     "<label>Y:</label><input type='number' name='y' placeholder='Y coordinate' value='0'>"
//     "<label>Z:</label><input type='number' name='z' placeholder='Z coordinate' value='0'>"
//     "</div>"

//     "<input type='submit' value='Send'>"
//     "</form>"
//     "</div>"

//     "<div id='angles'>"
//     "<p id='base'>Base: " + String(base) + "</p>"
//     "<p id='elbow'>Elbow: " + String(elbow) + "</p>"
//     "<p id='wrist'>Wrist: " + String(wrist) + "</p>"
//     "<p id='grab'>Grab: " + (grab ? "ON" : "OFF") + "</p>"
//     "</div>"
//     "</body></html>";

  // Send response with form and message
  server.send(200, "text/html", message);
}

/// @brief Handler to poll data from the '/getAngles' endpoint.
void handleGetAngles() {
  String json = "{\"base\": " + String(base) + ", "
                "\"elbow\": " + String(elbow) + ", "
                "\"wrist\": " + String(wrist) + ", "
                 "\"grab\": " + String(grab ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

/// @brief Handler to poll the style sheet.
void handleStyling() {
  server.send(200, "text/css", style);
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi.");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up server routes
  server.on("/", handleRoot2);
  server.on("/getData", handleGetData);
  server.on("/toggleMode", handleToggleMode);

  // server.on("/getAngles", handleGetAngles);
  // server.on("/styles.css", handleStyling);
  server.begin();                // Start the server
  Serial.println("Server started. Listening for angles.\n");
}

void loop() {
  server.handleClient();  // Listen for incoming clients
}

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <cstdlib>
#include <LiquidCrystal.h>

// NOTE: You will have to configure this. If you are on a public network,
// then it would be better to create a hotspot to connect to.
#include "secrets.h" 

WebServer server(80); // server on port 80

char armsBuffer[64];
// Set to int for display purposes.
int base = 0;
int elbow = 0;
int wrist = 0;
int hand = 0;

// Initialize the LCD with the pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(14, 12, 27, 26, 25, 33);

/// @brief Handler for the root '/' endpoint, containing the main
/// webpage. Also handles sending values to the robot.
void handleRoot() {

  String message;
  
  // Check if each value parameter is in the URL and update variables
  // value parameters basically modify the url
  // example: <ip>/base=1&elbow=2&wrist=3
  if (server.hasArg("base")) base = server.arg("base").toInt();
  if (server.hasArg("elbow")) elbow = server.arg("elbow").toInt();
  if (server.hasArg("wrist")) wrist = server.arg("wrist").toInt();

  // Display received values/
  sprintf(
    armsBuffer,
    "\rBase: %03d | Elbow: %03d | Wrist: %03d", // Zero pad for simplicity in printing.
    base,
    elbow,
    wrist
  );
  // Serial monitor approach -- use LCD instead.
  // Serial.flush();
  // Serial.print('\r');
  // Serial.print(armsBuffer);

  lcd.clear();
  lcd.print(armsBuffer);

  // Use JavaScript AJAX to dynamically update the webpage with motor angles.
  // TODO: It might be worth looking into moving this into a deficated HTML file and whatnot
  message = "<html><head><title>CMPUT 312 3DOF ARM</title>"
      "<script>"
      "function refresh() {"
      "  setTimeout(() => {"
      "    fetch('/getAngles')"
      "    .then(response => response.json())"
      "    .then(data => {" 
      "      document.getElementById('base').innerHTML = 'Base: ' + data.base;"
      "      document.getElementById('elbow').innerHTML = 'Elbow: ' + data.elbow;"
      "      document.getElementById('wrist').innerHTML = 'Wrist: ' + data.wrist;"
      "    });"
      "  }, 250);"
      "}"
      "window.onload = refresh;"
      "</script>"
      "</head>"
      "<body><h1>Simple HTTP server</h1>"
      "<div id='form-container'>"
      "<form method='GET' action='/'>"
      "<label>Base: </label><input type='number' name='base' min='0' max='360' placeholder='Base angle' value=''><br>"
      "<label>Elbow: </label><input type='number' name='elbow' min='0' max='360' placeholder='Elbow angle' value=''><br>"
      "<label>Wrist: </label><input type='number' name='wrist' min='0' max='360' placeholder='Wrist angle' value=''><br>"
      "<input type='submit' value='Send'/><br>"
      "</form>"
      "</div>"
      "<h2>Telemetry</h2><div id='angles'>"
      "<p id='base'>Base: " + String(base) + "</p>"
      "<p id='elbow'>Elbow: " + String(elbow) + "</p>"
      "<p id='wrist'>Wrist: " + String(wrist) + "</p>"
      "</div>"
      "</body></html>";

  // Send response with form and message
  server.send(200, "text/html", message);
}

/// @brief Handler to poll data from the '/getAngles' endpoint.
void handleGetAngles() {
  String json = "{\"base\": " + String(base) + ", "
                "\"elbow\": " + String(elbow) + ", "
                "\"wrist\": " + String(wrist) + "}";
  server.send(200, "application/json", json);
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
  server.on("/", handleRoot);    // Define the root endpoint
  server.on("/getAngles", handleGetAngles);
  server.begin();                // Start the server
  Serial.println("Server started. Listening for angles.\n");
}

void loop() {
  server.handleClient();  // Listen for incoming clients
}

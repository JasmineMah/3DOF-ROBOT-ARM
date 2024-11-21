#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Jasmine";

ESP8266WebServer server(80); // server on port 80

// Variables to hold the five values
String base = "0";
String elbow = "0";
String wrist = "0";

void handleRoot() {
  String message;
  
  // Check if each value parameter is in the URL and update variables
  if (server.hasArg("base")) base = server.arg("base");
  if (server.hasArg("elbow")) elbow = server.arg("elbow");
  if (server.hasArg("wrist")) wrist = server.arg("wrist");

  // Display received values in Serial Monitor
  Serial.println("Received values:");
  Serial.println("Base motor angle: " + base);
  Serial.println("Elbow motor angle: " + elbow);
  Serial.println("Wrist motor angle: " + wrist);

  // Send the values serially to Arduino
  Serial.print(base + "," + elbow + "," + wrist + "," + vexArm + "," + flipArm + "\n");

  // Create HTML form for submitting values
  message = "<form method='GET' action='/'>";
  message += "<label>Base motor angle: </label><input type='text' name='base' placeholder='Enter Left wheel'><br>";
  message += "<label>Elbow motor angle: </label><input type='text' name='elbow' placeholder='Enter Right wheel'><br>";
  message += "<label>Wrist motor angle: </label><input type='text' name='wrist' placeholder='Enter Stepper Arm'><br>";
  message += "<input type='submit' value='Send'>";
  message += "</form>";

  // Send response with form and message
  server.send(200, "text/html", message);
}

void setup() {
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up server routes
  server.on("/", handleRoot);    // Define the root endpoint
  server.begin();                // Start the server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Listen for incoming clients
}

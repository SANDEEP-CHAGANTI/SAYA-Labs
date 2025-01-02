#include <WiFi.h>
#include <WebServer.h>

// Motor Pins
#define in1 23
#define in2 25
#define in3 26
#define in4 27

// Wi-Fi Credentials
const char *ssid = "S";
const char *password = "123456789";

// Web server instance
WebServer server(80);

// Functions for motor control
void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void right() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// Web server handlers
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Motor Control</title>
      <style>
        button { padding: 15px; font-size: 16px; margin: 5px; }
      </style>
    </head>
    <body>
      <h1>Motor Control</h1>
      <button onclick="fetch('/forward')">Forward</button>
      <button onclick="fetch('/backward')">Backward</button><br>
      <button onclick="fetch('/left')">Left</button>
      <button onclick="fetch('/right')">Right</button><br>
      <button onclick="fetch('/stop')">Stop</button>
    </body>
    </html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handleForward() {
  forward();
  server.send(200, "text/plain", "Forward");
}

void handleBackward() {
  backward();
  server.send(200, "text/plain", "Backward");
}

void handleLeft() {
  left();
  server.send(200, "text/plain", "Left");
}

void handleRight() {
  right();
  server.send(200, "text/plain", "Right");
}

void handleStop() {
  stopMotors();
  server.send(200, "text/plain", "Stop");
}

void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Setup motor control pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  stopMotors();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);

  // Start the web server
  server.begin();
}

void loop() {
  server.handleClient();
}

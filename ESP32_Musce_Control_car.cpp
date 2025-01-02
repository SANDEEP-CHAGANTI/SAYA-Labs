#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Reset pin (not used here)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi credentials
const char* ssid = "ESP32_AP"; // Access Point SSID
const char* password = "123456789"; // Access Point Password

// Wi-Fi server setup
WiFiServer server(80);  // HTTP server on port 80

bool isConnected = false; // Tracks connection status

// Motor driver pins
#define MOTOR1_PIN1 23
#define MOTOR1_PIN2 25
#define MOTOR2_PIN1 26
#define MOTOR2_PIN2 27

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();

  // Set up Wi-Fi as Access Point (AP)
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Display AP details on OLED
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("IP: ");
  display.print(WiFi.softAPIP());
  display.display();

  // Start the server
  server.begin();

  // Initialize motor pins as output
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  // Stop motors initially
  stopMotors();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String mpuData = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        mpuData += c;
      }
    }

    // Process received data
    if (!isConnected) { // Update display only on first connection
      isConnected = true;

      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("IP: ");
      display.print(WiFi.softAPIP());
      display.setCursor(0, 20);
      display.print("Neuro Drive Connected");
      display.display();

      Serial.println("Neuro Drive Connected");
    }

    // Extract the first value (assumes values are formatted as "Accel: x,y,z")
    int firstValue = 0;
    if (mpuData.startsWith("Accel: ")) {
      int commaIndex = mpuData.indexOf(',');
      if (commaIndex > 7) { // Ensure we find a valid first value
        firstValue = mpuData.substring(7, commaIndex).toInt();
      }
    }

    // Determine muscle state and control motors
    if (firstValue > 10000 || firstValue < -10000) {
      Serial.println("Pressured");
      moveForward();
    } else {
      Serial.println("Relaxed");
      stopMotors();
    }

    client.stop();
  }
}

// Function to move motors forward
void moveForward() {
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
}

// Function to stop motors
void stopMotors() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
}

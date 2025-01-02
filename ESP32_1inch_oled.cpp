#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>  // Include Wi-Fi library for ESP32/ESP8266

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET    -1 // Reset pin (not used here)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi credentials
const char* ssid = "S";
const char* password = "123456789";

// Function prototypes
void drawThickParallelogram(int x, int y, int width, int height, int thickness, bool mirrored = false, bool flipVertical = false);

void setup() {
  // Initialize serial and OLED display
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay(); // Clear the buffer

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  // Wait for the Wi-Fi to connect
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  // Display connection details
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Draw parallelogram eyes (moving them a little higher)
  drawThickParallelogram(20, 10, 30, 10, 3, false, true); // Left eye flipped vertically and moved up
  drawThickParallelogram(78, 10, 30, 10, 3, true); // Right eye (mirrored)

  // Draw thicker text below the eyes, centered horizontally
  display.setTextSize(1);            // 2:1 pixel scale for thicker text
  display.setTextColor(WHITE);       // White text
  display.setCursor((SCREEN_WIDTH - 60) / 2, 10 + 10 + 2);  // Position text just below eyes with a small gap
  display.println("SAYA Labs");

  // Display IP address at the bottom
  display.setTextSize(1);            // Regular text size
  display.setCursor(0, SCREEN_HEIGHT - 10);  // Position text at the bottom
  display.print("IP: ");
  display.print(WiFi.localIP());     // Print IP address
  
  display.display();                 // Show the display buffer
}

void loop() {
  // No looping actions needed
}

// Function to draw a thick parallelogram
void drawThickParallelogram(int x, int y, int width, int height, int thickness, bool mirrored, bool flipVertical) {
  for (int i = 0; i < thickness; i++) {
    if (mirrored) {
      if (flipVertical) {
        display.drawLine(x - i, y + height, x + width - i, y + height, WHITE);                             // Bottom edge (flipped)
        display.drawLine(x + width - i, y + height, x + width - (height / 2) - i, y, WHITE); // Right slant (flipped)
        display.drawLine(x + width - (height / 2) - i, y, x - (height / 2) - i, y, WHITE); // Top edge (flipped)
        display.drawLine(x - i, y + height, x - (height / 2) - i, y, WHITE);             // Left slant (flipped)
      } else {
        display.drawLine(x - i, y, x + width - i, y, WHITE);                             // Top edge
        display.drawLine(x + width - i, y, x + width - (height / 2) - i, y + height, WHITE); // Right slant
        display.drawLine(x + width - (height / 2) - i, y + height, x - (height / 2) - i, y + height, WHITE); // Bottom edge
        display.drawLine(x - i, y, x - (height / 2) - i, y + height, WHITE);             // Left slant
      }
    } else {
      if (flipVertical) {
        display.drawLine(x + i, y + height, x + width + i, y + height, WHITE);                             // Bottom edge (flipped)
        display.drawLine(x + i, y + height, x - (height / 2) + i, y, WHITE);             // Left slant (flipped)
        display.drawLine(x + width + i, y + height, x + width - (height / 2) + i, y, WHITE); // Right slant (flipped)
        display.drawLine(x - (height / 2) + i, y, x + width - (height / 2) + i, y, WHITE); // Top edge (flipped)
      } else {
        display.drawLine(x + i, y, x + width + i, y, WHITE);                             // Top edge
        display.drawLine(x + i, y, x - (height / 2) + i, y + height, WHITE);             // Left slant
        display.drawLine(x + width + i, y, x + width - (height / 2) + i, y + height, WHITE); // Right slant
        display.drawLine(x - (height / 2) + i, y + height, x + width - (height / 2) + i, y + height, WHITE); // Bottom edge
      }
    }
  }
}

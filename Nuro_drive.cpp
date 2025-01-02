#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <MPU6050.h>

const char* ssid = "ESP32_AP";
const char* password = "123456789";
const char* host = "192.168.4.1";  // Receiver's IP address

WiFiClient client;
MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  } else {
    Serial.println("MPU6050 connected!");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  Serial.println("Connected to Wi-Fi!");
}

void loop() {
  // Read accelerometer and gyroscope values
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Prepare data to send
  String data = String("Accel: ") + ax + "," + ay + "," + az + " Gyro: " + gx + "," + gy + "," + gz;

  // Connect to the receiver (Server) and send the data
  if (client.connect(host, 80)) {
    Serial.println("Connected to receiver");
    client.println(data);  // Send data
    client.stop();  // Disconnect
  } else {
    Serial.println("Connection failed");
  }

  delay(500);  // Wait before reading again
}

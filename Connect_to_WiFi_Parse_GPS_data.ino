#include <WiFi.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "pass";

HardwareSerial gpsSerial(1); // UART1 on ESP32-C3-DevKitM-1
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 3, 2); // GPS module connected to UART1 on GPIO3 (RX) and GPIO2 (TX)

  connectToWiFi();
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        Serial.print("Latitude: ");
        Serial.println(latitude, 6);

        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
      }
    }
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());
}

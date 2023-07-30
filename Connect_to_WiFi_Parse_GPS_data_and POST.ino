#include <WiFi.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "pass";

const char* serverAddress = "Your_Machine_IP"; // Replace with your server address. You can add your IP address where you flask app.py is running
const int serverPort = 80;
const char* serverEndpoint = "/update_location"; // Replace with the server endpoint to handle the location data

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

        sendDataToServer(latitude, longitude);
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

void sendDataToServer(float latitude, float longitude) {
  HTTPClient http;

  // Prepare the URL with the server address and endpoint
  String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(serverEndpoint);

  // Prepare the data to send in the POST request
  String postData = "lat=" + String(latitude, 6) + "&lon=" + String(longitude, 6);

  // Send HTTP POST request
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(postData);

  // Check for a successful HTTP response
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    Serial.print("Server Response: ");
    Serial.println(response);
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(httpResponseCode);
  }

  // Close the connection
  http.end();
}

/*
  ESP32 sensor client
  - Reads: DHT22 (temp/humidity), capacitive soil moisture (analog), simulated NPK (analog)
  - Sends JSON via HTTP POST to backend `/ingest`
  - Optional: polls /control for relay commands
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_PIN 34 // ADC input
#define NPK_PIN 35  // ADC input (simulated)
#define RELAY_PIN 16

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";
const char* BACKEND_URL = "http://127.0.0.1:5000"; // update to your backend

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) { Serial.print('.'); delay(500); }
  Serial.println("\nWiFi connected");
}

float readSoilPercent() {
  int raw = analogRead(SOIL_PIN);
  // map ADC (0-4095) to percentage
  return constrain(100.0 - ((raw / 4095.0) * 100.0), 0.0, 100.0);
}

float readNPK() {
  int raw = analogRead(NPK_PIN);
  // simulated NPK index 0-100
  return constrain((raw / 4095.0) * 100.0, 0.0, 100.0);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float soil = readSoilPercent();
    float npk = readNPK();
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("Failed DHT sensor read");
      delay(5000);
      return;
    }

    String payload = "{";
    payload += String("\"device_id\":\"esp32-001\"");
    payload += String(",\"temperature\":") + String(temp,2);
    payload += String(",\"humidity\":") + String(hum,2);
    payload += String(",\"soil_moisture\":") + String(soil,2);
    payload += String(",\"npk_index\":") + String(npk,2);
    payload += String("}");

    HTTPClient http;
    String url = String(BACKEND_URL) + "/ingest";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(payload);
    if (httpCode > 0) {
      String resp = http.getString();
      Serial.printf("POST %d: %s\n", httpCode, resp.c_str());
    } else {
      Serial.printf("POST failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  delay(15000); // send every 15s
}

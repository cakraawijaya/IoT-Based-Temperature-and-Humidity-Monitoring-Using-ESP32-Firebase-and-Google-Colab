#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define DATABASE_URL "https://suhu-kelembapan-bpvp-sda-default-rtdb.firebaseio.com/sensor/dht.json"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");
}

void loop()
{

  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  Serial.println("=================================");

  if (!isnan(suhu) && !isnan(kelembaban))
  {

    Serial.print("Suhu        : ");
    Serial.print(suhu);
    Serial.println(" °C");

    Serial.print("Kelembaban  : ");
    Serial.print(kelembaban);
    Serial.println(" %");

    HTTPClient http;
    http.begin(DATABASE_URL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"suhu\":" + String(suhu) +
                      ",\"kelembaban\":" + String(kelembaban) + "}";

    Serial.println("Mengirim data ke Firebase...");
    Serial.println("JSON: " + jsonData);

    int httpResponseCode = http.POST(jsonData);

    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == 200)
    {
      Serial.println("Status: BERHASIL terkirim ✅");
    }
    else
    {
      Serial.println("Status: GAGAL ❌");
    }

    http.end();
  }
  else
  {
    Serial.println("Gagal membaca sensor DHT!");
  }

  Serial.println("Menunggu 5 detik...\n");
  delay(5000);
}
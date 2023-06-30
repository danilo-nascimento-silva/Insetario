#include "DHT.h"
#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "Iolanda"
#define WIFI_PASSWORD "liger88034696"

#define FIREBASE_HOST "https://insetario-uesc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDU4UQBRtXjAOyoo9KHRM7f1yxdjwapX4Y"

#define DHTPIN 4     
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
  
void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println("°C");
  
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println("%");

  if (!isnan(temperature) && !isnan(humidity)) {
    Firebase.setFloat("leituras/temperatura", temperature);
    Firebase.setFloat("leituras/umidade", humidity);

    if (Firebase.failed()) {
      Serial.print("Falha no envio dos dados para o Firebase: ");
    } else {
      Serial.println("Dados enviados para o Firebase com sucesso!");
    }
  }
  delay(5000);
}

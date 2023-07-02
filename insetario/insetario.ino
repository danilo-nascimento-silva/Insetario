#include "DHT.h"

#define DHTPIN 4     
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando...");
  dht.begin();
}
  
void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.print("     -     ");
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println("%");
  delay(1000);
}

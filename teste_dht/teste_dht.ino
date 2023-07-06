//********************************* Inclusion of libraries *************************************

#include <WiFi.h>                          
#include <IOXhop_FirebaseESP32.h>        
#include <ArduinoJson.h>  
#include "DHT.h"
#include <NTPClient.h>

//********************************* WIFI Configuration *****************************************

#define WIFI_SSID "Iolanda"
#define WIFI_PASSWORD "liger88034696"

//********************************* Firebase Configuration *************************************

#define FIREBASE_AUTH "AIzaSyDU4UQBRtXjAOyoo9KHRM7f1yxdjwapX4Y"
#define FIREBASE_HOST "https://insetario-uesc-default-rtdb.firebaseio.com" 

//********************************* SETTINGS DHT22 *********************************************

#define DHTPIN 4     
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);

//********************************* SETTINGS NTP CLIENT *******************************************

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);

//#################################### Function Setup #############################################

void setup(){

//********************************* Start Serial *****************************************

    Serial.begin(115200);

//********************************* Start DHT *****************************************

    dht.begin();

//********************************* Wireless Connection *****************************************

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

//********************************* Start NTP *****************************************

    ntp.begin();
    ntp.setTimeOffset(-10800);

//********************************* Declaration Variables *****************************************

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   //inicia comunicação com firebase definido anteriormente

}

void loop() {
  
  ntp.update();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  String datetime = ntp.getFormattedDate();
  
  Serial.println(datetime);
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.print("     -     ");
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println("%");

  Firebase.setFloat("temperatura", temperature);
  Firebase.setFloat("umidade", humidity);
  Firebase.setString("datetime", datetime);

  delay(1000);
}

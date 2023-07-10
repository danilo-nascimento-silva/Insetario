//********************************* Inclusion of libraries *************************************

#include <WiFi.h>                          
#include <IOXhop_FirebaseESP32.h>        
#include <ArduinoJson.h>  
#include "DHT.h"
#include <NTPClient.h>
#include <WiFiUdp.h>        

//********************************* WIFI Configuration *****************************************

#define WIFI_SSID "Iolanda"
#define WIFI_PASSWORD "liger88034696"

//#define WIFI_SSID "Automacao"
//#define WIFI_PASSWORD "127.0.0.1..."

//********************************* Firebase Configuration *************************************

#define FIREBASE_AUTH "AIzaSyDU4UQBRtXjAOyoo9KHRM7f1yxdjwapX4Y"
#define FIREBASE_HOST "https://insetario-uesc-default-rtdb.firebaseio.com" 

//********************************* SETTINGS DHT22 *********************************************

#define DHTPIN 4     
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);

float temperature, humidity;

//********************************* SETTINGS NTP CLIENT *******************************************

const char* servidorNTP = "b.ntp.br"; // Servidor NTP para pesquisar a hora
 
const int fusoHorario = -10800; // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000; // Taxa de atualização do servidor NTP em milisegundos

unsigned long b_Filtro = 0;
String date, yearValueString, monthValueString, dayValueString, hourValue, address, datetime;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);

//********************************* Variable Declaration *********************************************

#define led 5
unsigned long timeMillis;

//#################################### Function Setup #############################################

void setup(){

//********************************* Start Serial *****************************************

    Serial.begin(115200);

//********************************* Ports declaration *****************************************
    
    pinMode(led, OUTPUT);

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

    timeClient.begin();  // Iniciar cliente de aquisição do tempo

//********************************* Declaration Variables *****************************************

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   //inicia comunicação com firebase definido anteriormente

}

void loop() {
    getInf();
    dht22();

    if (millis() - timeMillis >= 300000 || timeMillis == 0){
      timeMillis = millis();
      send_info();
    }
    delay(500);
}

void send_info(){
    Firebase.setFloat(address + "temperatura", temperature);
    Firebase.setFloat(address + "umidade", humidity);
    Firebase.setString(address + "datetime", datetime);
    Serial.println("Enviado às " + hourValue);
    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
}

void dht22(){
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.print(F("°C"));
    Serial.print(F(" - "));
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println(F("%"));
}

void getInf(){
  
  timeClient.update(); // atualiza data e hora de acordo com o NTP
  
  int yearValue = timeClient.getYear();
  yearValueString = String(yearValue);
  
  int monthValue = timeClient.getMonth();
  monthValueString = "";
  if(monthValue < 10 ){  //ajustar o padrão de dígitos
    monthValueString = "0";
    monthValueString.concat(monthValue);
  }else{
    monthValueString = String(monthValue); }
  
  int dayValue = timeClient.getDate();
  dayValueString = "";
  if(dayValue < 10 ){ //ajustar o padrão de dígitos
    dayValueString = "0";
    dayValueString.concat(dayValue);
  }else{
    dayValueString = String(dayValue); }
  
  hourValue = timeClient.getFormattedTime();
  
  if(yearValue >= 2021){ 
    
    address = yearValueString;
    address.concat("/");
    address.concat(monthValueString);
    address.concat("/");
    address.concat(dayValueString);
    address.concat("/");
    address.concat(hourValue);
    address.concat("/");

    datetime = dayValueString;
    datetime.concat("/");
    datetime.concat(monthValueString);
    datetime.concat("/");
    datetime.concat(yearValueString);
    datetime.concat(" ");
    datetime.concat(hourValue);
    Serial.println(datetime);

  }
} 

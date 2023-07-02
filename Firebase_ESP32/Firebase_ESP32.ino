//********************************* Inclusion of libraries *************************************

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "DHT.h"
#include <NTPClient.h>

//********************************* libraries support ******************************************

#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h"//Provide the RTDB payload printing info and other helper functions.

//********************************* WIFI Configuration *****************************************

#define WIFI_SSID "Iolanda"
#define WIFI_PASSWORD "liger88034696"

//********************************* Firebase Configuration *************************************

// Insert Firebase project API Key
#define API_KEY "AIzaSyDU4UQBRtXjAOyoo9KHRM7f1yxdjwapX4Y"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://insetario-uesc-default-rtdb.firebaseio.com" 

//********************************* Definitions Firebase ***************************************

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//********************************* SETTINGS DHT22 *********************************************

#define DHTPIN 4     
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);

//********************************* SETTINGS NTP CLIENT *******************************************

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);

//********************************* Declaration Variables *****************************************

unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;
unsigned long count = 0;
bool signupOK = false;
int sending_time = 5000;

//#################################### Send sensor #############################################

void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

//#################################### Send sensor #############################################

void sendString(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.println(ntp.getFormattedDate());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

//#################################### Function Setup #############################################

void setup(){

//********************************* Start Serial *****************************************

  Serial.begin(115200);

//********************************* Start DHT *****************************************

  dht.begin();

//********************************* Start NTP *****************************************

  ntp.begin();
  ntp.setTimeOffset(-10800);

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

//********************************* Declaration Variables *****************************************

  
  config.api_key = API_KEY; // Assign the api key (required)
  config.database_url = DATABASE_URL;  // Assign the RTDB URL (required)

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){

  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    ntp.update();

    sendString("DataHora", ntp.getFormattedDate())
    sendFloat("Temperatura", temperature);
    sendFloat("Umidade", humidity);
  }
}

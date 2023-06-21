#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define FIREBASE_HOST "https://console.firebase.google.com/project/insetario-uesc/database/insetario-uesc-default-rtdb/data/~2F?hl=pt-br"                         
#define FIREBASE_AUTH "AIzaSyDU4UQBRtXjAOyoo9KHRM7f1yxdjwapX4Y"            
#define WIFI_SSID "Automacao"                                         
#define WIFI_PASSWORD "127.0.0.1..."                                   

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  delay(1000); 
             
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                 
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);}

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());

  config.api_key = FIREBASE_AUTH;
  config.database_url = FIREBASE_HOST;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
    }
  else{ 
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
    
  config.token_status_callback = tokenStatusCallback; 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);                                                                      
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
    if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}

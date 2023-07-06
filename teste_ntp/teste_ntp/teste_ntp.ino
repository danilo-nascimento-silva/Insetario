#include <NTPClient.h>
#include <WiFi.h> 
#include <WiFiUdp.h>        

#define WIFI_SSID "Iolanda"
#define WIFI_PASSWORD "liger88034696"

const char* servidorNTP = "b.ntp.br"; // Servidor NTP para pesquisar a hora
 
const int fusoHorario = -10800; // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000; // Taxa de atualização do servidor NTP em milisegundos

unsigned long b_Filtro = 0;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);

void setup() {
  Serial.begin(115200);  // Declarar Serial para realizar debug do código
  delay(10);
 
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

  timeClient.begin();  // Iniciar cliente de aquisição do tempo

}

void loop() {
  getInf();
  delay(1000);
}

void getInf(){
  
  timeClient.update(); // atualiza data e hora de acordo com o NTP
  
  int yearValue = timeClient.getYear();
  String yearValueString = String(yearValue);
  
  int monthValue = timeClient.getMonth();
  
  String monthValueString = "";
  if(monthValue < 10 ){  //ajustar o padrão de dígitos
  
    monthValueString = "0";
    monthValueString.concat(monthValue);
  
  }else{
  
    monthValueString = String(monthValue);
  
  }
  
  int dayValue = timeClient.getDate();
  String dayValueString = "";
  
  if(dayValue < 10 ){ //ajustar o padrão de dígitos
    
    dayValueString = "0";
    dayValueString.concat(dayValue);
  
  }else{
  
    dayValueString = String(dayValue);
  
  }
  
  String hourValue = timeClient.getFormattedTime();
  
  if(yearValue >= 2021){ //se for menor que 2021, a data é antiga e não foi atualizada
    
    String date = dayValueString;
    date.concat("_");
    date.concat(monthValueString);
    date.concat("_");
    date.concat(yearValueString);
    date.concat(" ");
    date.concat(hourValue);
    Serial.println(date); 

    
  }

}

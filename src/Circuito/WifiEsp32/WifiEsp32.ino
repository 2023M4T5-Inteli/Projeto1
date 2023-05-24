#include <WiFi.h>
#include <math.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> //permite manipular objetos e dados JSON.
#include <Wire.h>
#include <RTClib.h>
#include <UbidotsEsp32Mqtt.h>

const char* WIFI_SSID = "Inteli-COLLEGE"; // Fernanda
const char* WIFI_PASS = "QazWsx@123"; //  999818053

const char *UBIDOTS_TOKEN = "BBFF-L2UWDy9jLghHCxu8o0xL10OjOrWxcM";
const char* DEVICE_LABEL = "testevinicius";   // Replace with the device label to subscribe to
const char* VARIABLE_LABEL = "vartestevinicius"; // Replace with your variable label to subscribe to
const char* VARIABLE_LABEL2= "macvinicius";

const int PUBLISH_FREQUENCY = 1000; // Update rate in milliseconds
unsigned long timer;
Ubidots ubidots(UBIDOTS_TOKEN);

struct WiFiAP {  //informações de um ponto de acesso 
  String WIFI_SSID;
  int rssi;
  float x;
  float y;
};

unsigned int macToInt(const uint8_t* mac) {
  unsigned int result = 0;

  for (int i = 0; i < 6; i++) {
    result = (result << 8) | mac[i];
  }

  return result;
}

void estimatePosition(WiFiAP* apList, int numAPs) {
  float sumX = 0;
  float sumY = 0;
  float sumWeight = 0;

  for (int i = 0; i < numAPs; i++) {
    WiFiAP ap = apList[i];
    float weight = pow(10, (ap.rssi / -10.0)); // Peso do RSSI

    sumX += ap.x * weight;
    sumY += ap.y * weight;
    sumWeight += weight;
  }

  float estimatedX = sumX;
  // Faça algo com a posição estimada
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  Serial.println("Escaneando redes WiFi...");
  int numRedes = WiFi.scanNetworks();
  Serial.print("Redes encontradas: ");
  Serial.println(numRedes);

  if (numRedes == 0) {
    Serial.println("Nenhuma rede WiFi encontrada!");
  } else {
    Serial.println("Redes WiFi encontradas:");
    for (int i = 0; i < numRedes; i++) {
      String WIFI_SSID = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      Serial.print("SSID: ");
      Serial.print(WIFI_SSID);
      Serial.print(" | Intensidade do sinal (RSSI): ");
      Serial.print(rssi);
      Serial.println(" dBm");
    }
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  
  Serial.println("Conectado ao Wi-Fi!");

  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL); 
}


void loop() {

  // Obter informações do ponto de acesso conectado
  int rssi; // Intensidade do sinal
  byte mac[6]; // //Endereço fisíco roteador 
  WiFi.macAddress(mac);

// Imprimindo endereço MAC 
  Serial.print("Endereço MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(':');
  }
  Serial.println();

  String macString = ""; 
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 0x10) {
      macString += "0";
    }
    macString += String(mac[i], HEX);
    if (i < 5) {
      macString += ":";
    }
  }

  Serial.print("Endereço MAC formatado: ");
  Serial.println(macString);

  // Obter a intensidade do sinal do ponto de acesso conectado (em dBm)
  rssi = WiFi.RSSI();
  Serial.print("Intensidade do Sinal: ");
  Serial.print(rssi);
  Serial.println(" dBm");

  identificandoRoteador();
  //pegandoDataEHora();

  if(millis() - timer > PUBLISH_FREQUENCY){
    
    int32_t rssi = WiFi.RSSI();
    uint8_t* mac = WiFi.BSSID();
    int macint=macToInt(mac);
    rssi=rssi*(-1);
    ubidots.add(VARIABLE_LABEL, rssi);
    ubidots.add(VARIABLE_LABEL2,macint);
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL); 
  }
  ubidots.loop();


  delay(5000);
}

void identificandoRoteador() {
  String bssid = WiFi.BSSIDstr();
  
  Serial.print("BSSID do roteador: ");
  Serial.println(bssid);

}

// void callback(char *topic, byte *payload, unsigned int length)
// {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
//   for (int i = 0; i < length; i++)
//   {
//     Serial.print((char)payload[i]);
//     if ((char)payload[0] == '1')
//     {
//       digitalWrite(LED, HIGH);
//     }
//     else
//     {
//       digitalWrite(LED, LOW);
//     }
//   }
//   Serial.println();

//   identificandoRoteador();
// }


// void pegandoDataEHora() {
//   RTC_DS1307 rtc;

//   // Obter a data e hora atual
//   DateTime now = rtc.now();
//   int year = now.year();
//   int month = now.month();
//   int day = now.day();
//   int hour = now.hour();
//   int minute = now.minute();
//   int second = now.second();

//   Serial.print("Data e hora: ");
//   Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n", year, month, day, hour, minute, second);

// }

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Inteli-COLLEGE";
const char* password = "QazWsx@123";
const char* googleApiUrl = "https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyCcRPn_FOfJBpgRhx4VvCPpfzaUu1CfOnM";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  
  Serial.println("Conectado ao Wi-Fi!");


}

void loop() {
    // Obter informações do ponto de acesso conectado
  int rssi;
  byte mac[6];
  WiFi.macAddress(mac);


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

  getGeolocation(macString, rssi);

  delay(30000);
}

void getGeolocation(String macString, int rssi) {
  HTTPClient http;
  
  // Prepara os dados para a requisição à API Geolocation do Google Maps
  // Criar um objeto JSON
  StaticJsonDocument<200> doc;

  doc["considerIP"] = "false";

  JsonArray wifiArray = doc.createNestedArray("wifi");

  JsonObject wifiObj = wifiArray.createNestedObject();
  wifiObj["mac"] = macString;
  wifiObj["signal"] = rssi;

  String jsonStr;
  serializeJson(doc, jsonStr);
  
  http.begin(googleApiUrl);

  http.addHeader("Content-Type", "application/json");
  
  // Faz a requisição POST para obter a geolocalização
  int httpResponseCode = http.POST(jsonStr);
  
  if (httpResponseCode == HTTP_CODE_OK) {
    // Lê a resposta JSON
    String response = http.getString();
    
     // Analisa a resposta JSON
    StaticJsonDocument<200> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, response);

    if (error) {
      Serial.println("Falha ao analisar resposta JSON!");
      return;
    }
    
    // Extrai os dados de latitude, longitude e precisão
    float latitude = jsonDocument["location"]["lat"].as<float>();
    float longitude = jsonDocument["location"]["lng"].as<float>();
    float accuracy = jsonDocument["accuracy"].as<float>();
    
    // Imprime os dados no console serial
    Serial.print("\nLatitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
    // Serial.print("Precisão: ");
    // Serial.println(accuracy);
  }
  else {
    Serial.print("Falha na requisição HTTP. Código de resposta: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

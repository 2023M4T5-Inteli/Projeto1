#include <WiFi.h>
#include <math.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> //permite manipular objetos e dados JSON.
#include <Wire.h>
#include <RTClib.h>
#include <UbidotsEsp32Mqtt.h>

const char* WIFI_SSID = "SHARE-RESIDENTE"; // Fernanda
const char* WIFI_PASS = "Share@residente23"; //  999818053

const char *UBIDOTS_TOKEN = "BBFF-L2UWDy9jLghHCxu8o0xL10OjOrWxcM";
const char *DEVICE_LABEL = "iotrackers-rastreador";   // Label do dispositivo para onde os dados serão publicados
const char *VARIABLE_LABEL1 = "bssid"; // Label da variável para onde os dados serão publicados
const char *VARIABLE_LABEL2 = "intensidade-wifi"; // Label da variável para onde os dados serão publicados
const char *VARIABLE_LABEL3 = "localizacao"; // Label da variável para onde os dados serão publicados
const char *VARIABLE_LABEL4 = "localizacao-setor"; // Label da variável para onde os dados serão publicados

String bssid;

const int PUBLISH_FREQUENCY = 1000; // Update rate in milliseconds
unsigned long timer;
Ubidots ubidots(UBIDOTS_TOKEN);

struct WiFiAP {  //informações de um ponto de acesso 
  String WIFI_SSID;
  int rssi;
  float x;
  float y;
};

struct SetorInfo {
  int setor; // mudar para string
  double latitude;
  double longitude;
  String bssids[3];  // Alterado para um array de BSSIDs com tamanho máximo de 3
  int numBssids; // Número atual de bssids no array
};

struct Localizacao {
  double latitude;
  double longitude;
};

// Array com os setores e suas informações
SetorInfo setores[] = {
  {1, 1.1, 1.1, {"FC:5C:45:00:56:18"}, 1},
  {9, 2.2, 2.2, {"26:83:44:20:48:BF", "FC:5C:45:00:4F:C8"}, 2},
  {11, 3.3, 3.3, {"BSSID3_1", "BSSID3_2", "BSSID3_3"}, 3}
};

// Número total de setores no array
const int numSetores = sizeof(setores) / sizeof(SetorInfo);

// Função para verificar o BSSID e retornar o setor correspondente, juntamente com a latitude e longitude
SetorInfo verificarSetor(String bssid) {
  for (int i = 0; i < numSetores; i++) {
    for (int j = 0; j < setores[i].numBssids; j++) {
      if (bssid.equals(setores[i].bssids[j])) {
        return setores[i];
      }
    }
  }
  // Caso não encontre o BSSID, retorna uma estrutura vazia
  SetorInfo vazio = {0, 0.0, 0.0, {}, 0};
  return vazio;
}

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
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
}


void loop() {

  // Obter informações do ponto de acesso conectado
  int rssi; // Intensidade do sinal
  int piorRede = -70; // Pior sinal pré-determinado.
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



  // identificandoRoteador();

  if(millis() - timer > PUBLISH_FREQUENCY){

    String bssid = WiFi.BSSIDstr();

    // Imprimir o BSSID no terminal
    Serial.print("BSSID: ");
    Serial.println(bssid);

    // Verificar o setor, latitude e longitude correspondentes ao BSSID
    SetorInfo setorInfo = verificarSetor(bssid);

    // Imprimir o setor, latitude e longitude correspondentes
    if (setorInfo.setor != 0) {

      int32_t rssi = WiFi.RSSI();
      uint8_t* mac = WiFi.BSSID();
      int macint = macToInt(mac);
      String bssid = WiFi.BSSIDstr();

      Localizacao localizacao;
      localizacao.latitude = setorInfo.latitude;
      localizacao.longitude = setorInfo.longitude;

      Serial.println();
      Serial.print("BSSID: ");
      Serial.println(bssid);
      Serial.print("Setor: ");
      Serial.println(setorInfo.setor);
      Serial.print("Latitude: ");
      Serial.println(setorInfo.latitude);
      Serial.print("Longitude: ");
      Serial.println(setorInfo.longitude);
      Serial.print("Intensidade do sinal: ");
      Serial.print(rssi);
      Serial.println("dBm ");

      
      ubidots.add(VARIABLE_LABEL1,macint);
      ubidots.add(VARIABLE_LABEL2, rssi);
      ubidots.add(VARIABLE_LABEL4, setorInfo.setor);
      ubidots.publish(DEVICE_LABEL);
      timer = millis();
    } else {
      Serial.println("BSSID não encontrado nos setores.");
    };
  }
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  ubidots.loop();

  String ultimaPosicao;
  String penultimaPosicao;
  bool houveMudanca = false;

  ultimaPosicao = bssid;
  if (ultimaPosicao != penultimaPosicao) {
    houveMudanca = true;
  }

  // Atualizar a penúltima posição
  penultimaPosicao = ultimaPosicao;

  // Verificar se houve mudança de posição
  if (houveMudanca) {
    Serial.println("Houve uma mudança de posição do roteador.");
  } else {
    Serial.println("Não houve mudança de posição do roteador.");
  }

  // Verifica se o dispositivo está pegando o melhor sinal Wi-Fi
  if (rssi < piorRede ) {
    conectarMelhorWifi();
  }

  delay(5000);
}

void identificandoRoteador() {
  String bssid = WiFi.BSSIDstr();

    // Procurar o valor correspondente no dicionário
  if (bssid == "FC:5C:45:00:4F:C8") {
    bssid = "Ateliê 11";
  } else if (bssid == "FC:5C:45:00:60:98") {
    bssid = "Ateliê 9";
  } else if (bssid == "FC:5C:45:00:63:58") {
    bssid = "Honest Bar";
  } else if (bssid == "FC:5C:45:00:63:58") {
    bssid = "Atelie 5";
  } else if (bssid == "FC:5C:45:00:68:88") {
    bssid = "Itbar";
  } else if (bssid == "FC:5C:45:00:4F:D8") {
    bssid = "Biblioteca/Honest";
  } else if (bssid == "84:23:88:19:6A:B8") {
    bssid = "Lancheco";
  } else if (bssid == "FC:5C:45:00:5F:68") {
    bssid = "Refeitório";
  } else if (bssid == "FC:5C:45:00:57:48") {
    bssid = "Secretaria/Sofá do POD";
  } else {
    bssid = "Roteador não Mapeado";
  }

  // Atribuir o valor corrigido a bssid
  bssid = bssid;

  Serial.print("BSSID do roteador: ");
  Serial.println(bssid);

}

void conectarMelhorWifi() {
  int numRedes = WiFi.scanNetworks();
  int indiceMelhorRede = -1; // Valor inicial inválido para o índice da melhor rede

  // Percorrer todas as redes disponíveis
  for (int i = 0; i < numRedes; i++) {
    int potencia = WiFi.RSSI(i);
      indiceMelhorRede = i;
  }

  // Verificar se foi encontrada uma rede com potência de sinal suficientemente boa
   if (indiceMelhorRede >= 0) {
      if (WiFi.SSID(indiceMelhorRede) == "Inteli-COLLEGE" && WIFI_PASS == "QazWsx@123") {
        Serial.print("Conectando à melhor rede WiFi: ");
        Serial.println(WiFi.SSID(indiceMelhorRede));
        WiFi.begin(WiFi.SSID(indiceMelhorRede).c_str(), WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.println("Conectando ao Wi-Fi...");
        }
      } else {
          Serial.println("Senha da rede WiFi não fornecida ou rede não encontrada!");
          Serial.println(WiFi.SSID(indiceMelhorRede));
          ESP.restart();
      }
    } else {
        Serial.println("Nenhuma rede WiFi encontrada!");
        ESP.restart();
}

}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
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
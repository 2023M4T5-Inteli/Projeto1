// Incluindo bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SS_PIN 5
#define RST_PIN 15
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522 mfrc522(SS_PIN, RST_PIN);   
String content;

// Definindo as portas
const int buzzerPin = 25;
const int ledVerm = 32;
const int ledVerd = 14;
const int button = 4;
const int pins[] = {buzzerPin, ledVerm, ledVerd};
const int pinsInput[] = {button};
const int keys[] = {};
bool autorizado = false;
char* listaCartoesAutorizados[] = {"AC 55 08 00"};
int led;

void setup() {
  // Configurações iniciais
  Serial.begin(9600);  
  SPI.begin();      
  mfrc522.PCD_Init();   

  // Definindo as portas input e output
  for (int i = 0; i < 3; i++) {
    pinMode(pins[i], OUTPUT);
    pinMode(pinsInput[i], INPUT);
  }

  // Início do programa
  inicioLcd();
  lcdPrinter("Aproxime o", "Cartao");
}

void inicioLcd() {
  // Configurações do LCD
  lcd.init();
  lcd.backlight();
  lcdPrinter("IoTrackers", "Inteli | Pirelli");
  delay(5000);
  lcd.clear();
}

void loop() {
  lerCard();
} 

void lerCard() {
  // Verificando se é um cartão
  if (!mfrc522.PICC_IsNewCardPresent()) { return; }
  if (!mfrc522.PICC_ReadCardSerial()) { return; }
  lcd.clear();

  // Printando o id da tag
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  // Verificando o cartão
  verificaAcesso(content);
}

// void novoUid(){
//   byte newUid[] = NEW_UID;
//   if (mfrc522.MIFARE_SetUid(newUid, sizeof(newUid), true)) {
//     Serial.println(F("Novo UID gravado no cartão."));
//   } else {
//     Serial.println(F("Falha ao gravar novo UID no cartão."));
//   }

//   // Halt para evitar conflitos na leitura
//   mfrc522.PICC_HaltA();
//   mfrc522.PCD_StopCrypto1();
// }

void verificaAcesso(String content){
  Serial.println();
  content.toUpperCase();

  // Verificando se o cartão está autorizado
  for (const auto& elemento : listaCartoesAutorizados) {
    if (String(elemento) == content.substring(1)) {
      autorizado = true;
      acessoAutorizacao(content, autorizado);
      break;
      }
    }

    //Fornecendo resposta pro usuário
    if (autorizado == false) {
      acessoAutorizacao(content, autorizado);
    }
  lcd.clear();
  delay(250);
  lcdPrinter("Aproxime o", "Cartao");
}

void acessoAutorizacao (String content, bool autorizado) {
  Serial.print("Menssagem : ");

  // Caso o cartão esteja autorizado
  if (autorizado) {
    Serial.println("Acesso Autorizado");
    lcdPrinter("Acesso Autorizado: ", content);
    ledBuzzer("verde");
  } else {
    Serial.println("Acesso Negado");
    lcdPrinter("Acesso Negado: ", content);
    ledBuzzer("vermelho");
  }
}

void ledBuzzer(String cor) {
  // Condição para ver o comportamento do acendimento
  if (cor == "verde") {
    led = ledVerd;
    digitalWrite(led, HIGH);  
    buzzer(250);
    buzzer(250);
  } else {
    led = ledVerm;
    digitalWrite(led, HIGH);  
    buzzer(750);
  }
  digitalWrite(led, LOW); 
}


// Função para fazer barulho.
void buzzer(int time) {
  digitalWrite(buzzerPin, HIGH);
  delay(time);
  digitalWrite(buzzerPin, LOW);
  delay(time);
}

// Função para printar algo no LCD
void lcdPrinter(String message1, String message2) {
  lcd.print(message1);
  if (message2 != nullptr) {
    lcd.setCursor(0, 1);
    lcd.print(message2);
  }
}
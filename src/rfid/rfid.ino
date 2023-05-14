#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SS_PIN 5
#define RST_PIN 15
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522 mfrc522(SS_PIN, RST_PIN);   
String content;
const int buzzerPin = 25;
const int ledVerm = 32;
const int ledVerd = 14;
const int button = 4;
const int pins[] = {buzzerPin, ledVerm, ledVerd};
const int pinsInput[] = {button};
const int keys[] = {};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
    pinMode(pinsInput[i], INPUT);
  }

  pinMode(14, OUTPUT);
  inicioLcd();
  Serial.begin(9600);  
  SPI.begin();      
  mfrc522.PCD_Init();   
  aproximeCartao();
  Serial.println();

}

void inicioLcd(){
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("IoTrackers");
  lcd.setCursor(0, 1);
  lcd.print("Inteli | Pirelli");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
}
void loop() 
{
  lerCard();
} 

void lerCard(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  lcd.clear();
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
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

  if (content.substring(1) == "AC 55 08 00")
  { 
    acessoAutorizado(content);
    lcd.clear();
    delay(250);
    aproximeCartao();
  }
 
 else {
    naoAutorizado(content);
    lcd.clear();
    delay(250);
    aproximeCartao();
  }
}

void acessoAutorizado(String content){
  Serial.print("Menssagem : ");
  Serial.println("Acesso Autorizado");
  lcd.print("Acesso Autorizado: ");
  lcd.setCursor(0, 1);
  lcd.print(content);
  ledVerdeBuzzer();
  Serial.println();   
}

void naoAutorizado(String content){
  Serial.print("Menssagem : ");
  Serial.println(" Acesso Negado");
  lcd.print("Acesso Negado: ");
  lcd.setCursor(0, 1);
  lcd.print(content);
  ledVermelhoBuzzer();
  Serial.println();
}

void ledVermelhoBuzzer(){
  digitalWrite(ledVerm, HIGH);
  buzzer();
  delay(1000);
  digitalWrite(ledVerm, LOW);
}

void ledVerdeBuzzer(){
  digitalWrite(ledVerd, HIGH);  
  buzzer();
  delay(1000);
  digitalWrite(ledVerd, LOW); 
}

void buzzer(){
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
}

void aproximeCartao(){
  lcd.print("Aproxime o");
  lcd.setCursor(0, 1);
  lcd.print("Cartao");
}
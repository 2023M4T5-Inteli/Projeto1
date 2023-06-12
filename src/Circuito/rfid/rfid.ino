// Incluindo bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <Keypad.h>


//Keypad

// const uint8_t ROWS = 4;
// const uint8_t COLS = 4;

// char keys[ROWS][COLS] = {
//   {'1','2','3', 'A'},
//   {'4','5','6', 'B'},
//   {'7','8','9', 'C'},
//   {'*','0','#', 'D'}
// };  
// uint8_t colPins[COLS] = {2, 4, 17, 27}; // Pinos conectados a C1, C2, C3, C4
// uint8_t rowPins[ROWS] = {33, 35, 34, 26}; // Pinos conectados a R1, R2, R3, R4

// Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SS_PIN 5
#define RST_PIN 15
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522 mfrc522(SS_PIN, RST_PIN);   
String content;

// Definindo as portas
const int button_retirar= 34;
const int button_devolver= 33;
bool switch_acesso= false;
int value_button_retirar= 0;
int value_button_devolver= 0;
const int buzzerPin = 25;
const int ledRele= 0;
const int ledVerm = 32;
const int ledVerd = 14;
//const int button = 4;
const int pins[] = {buzzerPin, ledVerm, ledVerd};
//const int pinsInput[] = {button};

const int rele = 16;

const int triggerPin = 12;
const int echoPin = 13;
const int distanciaTablet = 7;
bool tabletDisponivel;

//const int keys[] = {};
bool autorizado = false;
char* listaCartoesAutorizados[] = {"C3 CD 05 15"};
int led;
char key;
String current = "";


void setup() {
  // Configurações iniciais
  Serial.begin(9600);  
  Wire.begin();
  SPI.begin();      
  mfrc522.PCD_Init();
     

  // Definindo as portas input e output
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
    //pinMode(pinsInput[i], INPUT);
  }
  pinMode(button_retirar, INPUT);
  pinMode(button_devolver, INPUT);

  // Configurando portas rele
  pinMode(rele, OUTPUT);

  digitalWrite(rele, LOW);
  digitalWrite(ledRele, HIGH);  
  Serial.println("Entrou rele");
  delay(3000);
  digitalWrite(rele, HIGH);
  digitalWrite(ledRele, LOW);  
  Serial.println("Saiu rele");

  // Configurando portas sensorultrassonico
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Início do programa
  inicioLcd();
  Serial.print("ENTROU SETUP");
  lcdPrinter("Aproxime o", "Cartao");
}

void inicioLcd() {
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcdPrinter("IoTrackers", "Inteli | Pirelli");
  Serial.print("ENTROU INICIO LCD");
  delay(5000);
  lcd.clear();
}

void processInput(char key) {
  if ('-' == key && current == "") {
    current = "-";
    lcd.print("-");
    return;
  }
}

void Buttons(){
  
}

void loop() {
  lerCard();
  verificaTabletNoArmario();
  Buttons();

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
    
    } else{
      autorizado = false;
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
    ativaRele();
    lcd.clear();
    switch_acesso= true;
    value_button_retirar= digitalRead(button_retirar);
    value_button_devolver= digitalRead(button_devolver);

  Serial.println(value_button_devolver);

    if(switch_acesso){
      lcdPrinter("Retirar:", "Devolver:");
      lcd.clear();
      if (value_button_retirar == HIGH){
        lcdPrinter("Retire o tablet no armário: ", "");
        delay(1000);
        lcd.clear();

      }
      if (value_button_devolver == HIGH){
        lcdPrinter("Devolva o tablet no armário: ", "");
        delay(1000);
        lcd.clear();

      }
  }
//    lcdPrinter(customkey);

  } else {
    Serial.println("Acesso Negado");
    lcdPrinter("Acesso Negado: ", content);
    ledBuzzer("vermelho");
    digitalWrite(rele, HIGH);
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

//Função para ativar o rele
void ativaRele() {
  digitalWrite(rele, LOW);
  digitalWrite(ledRele, HIGH);  
  Serial.println("Entrou rele");
  delay(3000);
  digitalWrite(rele, HIGH);
  digitalWrite(ledRele, LOW);  
  Serial.println("Saiu rele");
}

void verificaTabletNoArmario(){

  Ultrasonic ultrasonic(triggerPin, echoPin);
  float distance = ultrasonic.read();

  if (distance > distanciaTablet) {
    Serial.println("Tablet removido");
    tabletDisponivel = false;
  } else {
    Serial.println("Tablet presente");
    tabletDisponivel = true;
  }
  delay(1000);
}

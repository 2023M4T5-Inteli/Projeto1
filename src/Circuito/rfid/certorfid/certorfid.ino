// Incluindo bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <Keypad.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char tecla=NO_KEY;
bool teclaCerta=false;
bool leuCard=false;
 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

uint8_t colPins[COLS] = {33,32,17,0}; // Pinos conectados a C1, C2, C3, C4
uint8_t rowPins[ROWS] = {14,27,26,25};// Pinos conectados a R1, R2, R3, R4

// Create Keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

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
const int buzzerPin = 4;
const int ledRele= 0;
const int ledVerm = 32;
const int ledVerd = 14;
const int pins[] = {buzzerPin, ledVerm, ledVerd};

const int rele = 16;

const int triggerPin = 12;
const int echoPin = 13;
const int distanciaTablet = 10;
bool tabletDisponivel;

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
     
  pinMode(colPins[0], OUTPUT);
  pinMode(colPins[1], OUTPUT);
  pinMode(colPins[2], OUTPUT);
  pinMode(colPins[3], OUTPUT);

  // Set pin modes for keypad rows
  pinMode(rowPins[0], OUTPUT);
  pinMode(rowPins[1], OUTPUT);
  pinMode(rowPins[2], OUTPUT);
  pinMode(rowPins[3], OUTPUT);
  // Definindo as portas input e output
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
    //pinMode(pinsInput[i], INPUT);
  }
  pinMode(button_retirar, INPUT);
  pinMode(button_devolver, INPUT);

  // Configurando portas rele
  pinMode(rele, OUTPUT);


  digitalWrite(rele, HIGH);


  // Configurando portas sensorultrassonico
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Início do programa
  inicioLcd();
  Serial.println("ENTROU SETUP");
}

// Função com as configurações iniciais do LCD
void inicioLcd() {
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcdPrinter("IoTrackers", "Inteli | Pirelli");
  Serial.print("ENTROU INICIO LCD");
  delay(2000);
  lcd.clear();
}

// Função para printar no lcd
void processInput(char key) {
  if ('-' == key && current == "") {
    current = "-";
    lcd.print("-");
    return;
  }
}


// Função nativa de loop
void loop() {
  Ultrasonic ultrasonic(triggerPin, echoPin);
  float distance = ultrasonic.read();
  Serial.println(distance);
  teclaCerta=false;
  leuCard=false;
  lcd.clear();
  lcd.print("Armario:");
  while (tecla == NO_KEY) {
    tecla = customKeypad.getKey();
  }
  if (tecla!=NO_KEY){
    Serial.println(tecla);
    lcd.clear();
    lcd.print(tecla);
    if(tecla=='1'){
      teclaCerta=true;
    }    
    tecla=NO_KEY;
  }
  
  while(leuCard==false){lerCard();}
  verificaAcesso(content);
  if(teclaCerta&&autorizado){
    autorizado=true;
  }
  else{
    autorizado=false;
  }
  acessoAutorizacao(content, autorizado);
  // verificaTabletNoArmario();
  if(autorizado){
    verificaTabletNoArmario();
    if(tabletDisponivel){
      lcd.clear();
      lcd.print("Pode retirar");
      delay(1000);
      ativaRele();
    }
    else{
      lcd.print("Pode Devolver");
      delay(1000);
      ativaRele();
    }
  }
  else{
    lcd.clear();
    lcd.print("Acesso Negado");
    delay(1000);
    
  }
} 

// Função para ler o cartão
void lerCard() {

  lcd.clear();
  lcdPrinter("Aproxime o", "cartao");
  // Verificando se é um cartão
  if (!mfrc522.PICC_IsNewCardPresent()) { return; }
  if (!mfrc522.PICC_ReadCardSerial()) { return; }
  lcd.clear();

  // Printando o id da tag
  Serial.print("UID tag :");
  content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  leuCard=true;
  // Verificando o cartão
  
}

// Função para verificar a autorização do acesso
void verificaAcesso(String content){
  Serial.println();
  content.toUpperCase();

  // Verificando se o cartão está autorizado
  for (const auto& elemento : listaCartoesAutorizados) {
    if (String(elemento) == content.substring(1)) {
      autorizado = true;
      break;
    
    } else{
      autorizado = false;
    }
    
    }
    if (autorizado == false) {
      acessoAutorizacao(content, autorizado);
    }
  lcd.clear();
  delay(250);
  lcdPrinter("Aproxime o", "Cartao");
}

// Função para alterar com base no acesso
void acessoAutorizacao (String content, bool autorizado) {
  Serial.print("Menssagem : ");

  // Caso o cartão esteja autorizado
  if (autorizado) {
    lcd.clear();
    Serial.println("Acesso Autorizado");
    lcdPrinter("Acesso Autorizado: ", content);
    ledBuzzer("verde");
    lcd.clear();
    buzzer(250);
    switch_acesso= true;
    value_button_retirar= digitalRead(button_retirar);
    value_button_devolver= digitalRead(button_devolver);

  Serial.println(value_button_devolver);

  // Condição para os tablets

  } else {
    Serial.println("Acesso Negado");
    lcd.clear();
    lcdPrinter("Acesso Negado: ", content);
    ledBuzzer("vermelho");
    digitalWrite(rele, HIGH);
    buzzer(250);
    buzzer(250);
  }
}

// Função para acender o led
void ledBuzzer(String cor) {
  // Condição para ver o comportamento do acendimento
  if (cor == "verde") {
    led = ledVerd;  
    buzzer(250);
    buzzer(250);
  } else {
    led = ledVerm;  
    buzzer(750);
  }
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

// Função para ativar o rele
void ativaRele() {
  digitalWrite(rele, LOW);
  Serial.println("Entrou rele");
  lcd.clear();
  lcd.print("Aberto");
  delay(10000);
  digitalWrite(rele, HIGH);  
  Serial.println("Saiu rele");
  lcd.clear();
  lcd.print("Fechado");
  delay(1500);
  ESP.restart();
}

// Função para verificar a existência do tablet no armário
void verificaTabletNoArmario(){
  Ultrasonic ultrasonic(triggerPin, echoPin);
  float distance = ultrasonic.read();
  Serial.println(distance);
  if (distance > distanciaTablet) {
    tabletDisponivel = false;
    
  } else {
    tabletDisponivel = true;
  }
  delay(1000);
}

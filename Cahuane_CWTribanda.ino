#include <Wire.h> //Para el display
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

//I/O Digitales
#define Banda40m    A0
#define Banda20m    A1
#define Banda15m    A2
#define BotonVFO    A3
#define SMETER      A6
#define VOLTS       A7
#define BotonRIT    13
#define AD9850_CLK  12
#define AD9850_FQ   11
#define AD9850_DATA 10
#define AD9850_RST  9
#define CW_Out      8
#define Relay       7
#define SideTone    6
#define PowerCtrl   5
#define key         4
#define pinA        2
#define pinB        3
#define TIMEOUTVFO  10000 //10 Segundos

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }


//Constantes
#define DELAY_TRX 600

//Variables globales
byte llave_pos = 0;

//Globales Encoder
//static int pinA = 2;
//static int pinB = 3;
volatile byte aFlag = 0;
volatile byte bFlag = 0;
long encoderPos = 0; // desde -32,768 a 32,767
volatile byte reading = 0;

//Globales Funciones
unsigned long rx40m = 7000000; //desde 0 hasta 4.294.967.295
unsigned long rx20m = 14000000;
unsigned long rx15m = 21000000;
unsigned long tx40m = rx40m;
unsigned long tx20m = rx20m;
unsigned long tx15m = rx15m;
unsigned long oldfreq_dds = 0;
unsigned long last_key = 0;

unsigned long freq = 0;
int step_size = 100; //Esto despues deberia leerlo de la memoria
unsigned int step_rit = 10;

byte cursor_pos = 12;
bool config_flag = false;
bool rit_flag = false;
bool offset_flag = true; // true suma y false resta
bool key_flag = false;

int offset = 750;
int rit = 0;
byte potencia = 50;
unsigned int meter_counter=1;

bool flag_15m = false;
bool flag_20m = false;
bool flag_40m = false;

//Almacena ultimo movimiento VFO para guardar en la eeprom
unsigned long last_vfo_mv=0; 
unsigned int eeAddress = 0;

//Inicializaciones
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // YwRobot Arduino LCM1602 IIC V1

void setup() {
  Serial.begin(9600); // USB debug
  analogReference(EXTERNAL); //S meter con el AD8307 - 1.5v Max Out
  
  pinMode(Banda40m, INPUT);
  pinMode(Banda20m, INPUT);
  pinMode(Banda15m, INPUT);
  pinMode(BotonVFO, INPUT);
  pinMode(BotonRIT, INPUT);
  pinMode(SMETER, INPUT);
  pinMode(PinA, INPUT); 
  pinMode(PinB, INPUT);
  pinMode(key,  INPUT);
  pinMode(CW_Out, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(SideTone, OUTPUT);
  pinMode(PowerCtrl, OUTPUT);
  pinMode(AD9850_FQ, OUTPUT);
  pinMode(AD9850_CLK, OUTPUT);
  pinMode(AD9850_DATA, OUTPUT);
  pinMode(AD9850_RST, OUTPUT);

  //Inicio el AD9850
  pulseHigh(AD9850_RST);
  pulseHigh(AD9850_CLK);
  pulseHigh(AD9850_FQ);
  

  //Interrupciones
  attachInterrupt(0,PinA,RISING);
  attachInterrupt(1,PinB,RISING);

  //Leo las memorias desde la EEPROM
  
  //LCD
  lcd.begin (16,2);
  lcd.setBacklight(1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cahuane Tribanda"); 
  lcd.setCursor(0,1);
  lcd.print("CW TRx By LU1MAW");

  delay(1000);
  lcd.clear();

  //Actualiza DDS
  if(digitalRead(Banda40m) == HIGH) DDS_actualiza(rx40m + offset);
  if(digitalRead(Banda20m) == HIGH) DDS_actualiza(rx20m + offset);
  if(digitalRead(Banda15m) == HIGH) DDS_actualiza(rx15m + offset);


  //Leo los valores desde la memoria
  EEPROM.get(eeAddress,rx15m);
  eeAddress += sizeof(unsigned long);
  EEPROM.get(eeAddress,rx20m);
  eeAddress += sizeof(unsigned long);
  EEPROM.get(eeAddress,rx40m);
  eeAddress += sizeof(unsigned long);
  EEPROM.get(eeAddress,offset);
  eeAddress += sizeof(int);
  EEPROM.get(eeAddress,step_size);
  eeAddress += sizeof(int);

  //Actualizo los valores de tx
  tx15m = rx15m;
  tx20m = rx20m;
  tx40m = rx40m;  

  //Envio una V por el Sidetone
  ditdah(1);
  ditdah(1);
  ditdah(1);
  ditdah(2);
  
  Serial.println("Inicio correcto");
  
}

void loop() {
  if(digitalRead(Banda40m) == HIGH) Banda_40m();
  if(digitalRead(Banda20m) == HIGH) Banda_20m();
  if(digitalRead(Banda15m) == HIGH) Banda_15m();
  if(digitalRead(BotonVFO) == LOW && rit_flag == false) VFO_Button();
  if(digitalRead(BotonRIT) == LOW) RIT_Button();
    
  if(digitalRead(key) == LOW) {
    last_key = millis();
    keyfunc(true);   //Actualiza DDS en TX
    digitalWrite(CW_Out,HIGH); //CW output activa
    tone(SideTone,abs(offset)); //Genero Side Tone de la frecuencia correcta
    txmeter(true);
  }
  else {
    digitalWrite(CW_Out,LOW); //CW output desactivado
    noTone(SideTone);
    keyfunc(false);
    if (key_flag == false) s_meter();
    else txmeter(false);
  }
    
  if(config_flag == false) show_freq();
  else show_config();

  //Actualizo datos en la memoria eeprom
  if(millis() - last_vfo_mv >= TIMEOUTVFO) {
    EEPROM.put(eeAddress,rx15m);
    eeAddress += sizeof(unsigned long);
    EEPROM.put(eeAddress,rx20m);
    eeAddress += sizeof(unsigned long);
    EEPROM.put(eeAddress,rx40m);
    eeAddress += sizeof(unsigned long);
    EEPROM.put(eeAddress,offset);
    eeAddress += sizeof(int);
    EEPROM.put(eeAddress,step_size);
    eeAddress += sizeof(int);
  }

}


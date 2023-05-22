#include <Servo.h> //INCLUSÃO DA BIBLIOTECA NECESSÁRIA
#include <EEPROM.h>

const int pinoServo = 6; //PINO DIGITAL UTILIZADO PELO SERVO  
const int pinSpeaker = 3; //PINO DIGITAL UTILIZADO PELO COLUNA DE SON
const int pinButton1 = 8; //PINO DIGITAL UTILIZADO PELO BUTAO 1
const int pinLED1 = 2;
int posMin = 45;
int posMax = 135;
int configMode = 0;

Servo s; //OBJETO DO TIPO SERVO
int pos; //POSIÇÃO DO SERVO
 
void setup (){
  unsigned char value = 0;
  Serial.begin(9600);
  Serial.println(); Serial.println("2023 - Regulação da abertura do ar do gerador"); Serial.println();
  s.attach(pinoServo); //ASSOCIAÇÃO DO PINO DIGITAL AO OBJETO DO TIPO SERVO
  s.write(90); //INICIA O MOTOR NA POSIÇÃO 90º
  pinMode(pinSpeaker, OUTPUT);
  pinMode(pinButton1, INPUT);
  pinMode(pinLED1, OUTPUT);
  Serial.print("INFO: Reading posMin and posMax values from EEPROM. ");
  value = EEPROM.read(256);
  if ( value >= 0 && value <= 180 ) { posMin = value; Serial.print("(posMin: "); Serial.print(value); } else { Serial.println(); Serial.print("ERROR: Error reading posMin from EEPROM. Value out of range (0-180): "); Serial.println(value); }
  value = EEPROM.read(257);
  if ( value >= 0 && value <= 180 ) { posMax = value; Serial.print(", posMax: "); Serial.print(value); Serial.println(")"); } else { Serial.println(); Serial.print("ERROR: Error reading posMax from EEPROM. Value out of range (0-180): "); Serial.println(value); }
  tone(pinSpeaker,200,100);
}

void statusLED (int status ) { if ( status == 1) { digitalWrite(pinLED1, HIGH); } else { digitalWrite(pinLED1, LOW); } }

int buttonPressed1 () {

  // Return codes: 0 - no press; 1 - short press; 2 - long press;

  unsigned long startMillis;
  unsigned long currentMillis;
  const unsigned long period = 1000; //the value is a number of milliseconds, ie 1 second
  int button1Status = 0;

  button1Status = digitalRead(pinButton1); //Serial.println(button1Status);
  if ( button1Status == HIGH ) {
    tone(pinSpeaker,400,50); Serial.print("Button 1 pressed");
    startMillis = millis();
    //button1Status = digitalRead(pinButton1); Serial.println(button1Status);
    while ( button1Status == HIGH ) {
      currentMillis = millis();
      if ( currentMillis - startMillis >= period ) {
        tone(pinSpeaker,400,50); //delay(100); tone(pinSpeaker,400,50);
        while ( button1Status == HIGH ) { button1Status = digitalRead(pinButton1); }//Serial.println(button1Status); }
        Serial.println("... long press.");
        return 2;
      }
      button1Status = digitalRead(pinButton1); //Serial.println(button1Status);
    }
    Serial.println(".");
    return 1;
  } else {
    return 0;
    }
}

void configModeRoutine () {
  int configMode = 1;
  int button1Status = 0;
  unsigned char value;
  statusLED(1);
  pos = 90;
  s.write(pos);
  tone(pinSpeaker,300,50); delay(100); tone(pinSpeaker,400,50);
  Serial.println("INFO: Entering configuration mode for posMin and posMax.");

  while ( configMode == 1 ) {
    button1Status = digitalRead(pinButton1); //Serial.println(button1Status);
    switch ( buttonPressed1() ) {
      case 2:
        configMode = 0;
      case 1:
        if ( pos >= 5 ) { pos = pos - 5; }
        s.write(pos);
        while ( button1Status == 1 ) { button1Status = digitalRead(pinButton1); } //Serial.println(button1Status); }
        posMin = pos;
        break;
      default:
        break;
    }
  }

  configMode = 1;
  pos = 90;
  s.write(pos);
  while ( configMode == 1 ) {
    button1Status = digitalRead(pinButton1); //Serial.println(button1Status);
    switch ( buttonPressed1() ) {
      case 2:
        configMode = 0;
      case 1:
        if ( pos <= 175 ) { pos = pos + 5; }
        s.write(pos);
        while ( button1Status == 1 ) { button1Status = digitalRead(pinButton1); } //Serial.println(button1Status); }
        posMax = pos;
        break;
      default:
        break;
    }
  }

  Serial.print("INFO: Writing posMin and posMax to EEPROM. posMin: ");
  value = posMin; EEPROM.update(256,value); Serial.print(posMin); Serial.print(" posMax: ");
  value = posMax; EEPROM.update(257,value); Serial.println(posMax);
  tone(pinSpeaker,400,50); delay(100); tone(pinSpeaker,300,50);
  statusLED(0);
  Serial.println("INFO: Exiting configuration mode for posMin and posMax.");

}

void loop(){

  switch ( buttonPressed1() ) {
    case 2:
      configModeRoutine();
      break;
    case 1:
      break;
    default:
      break;
  }

  int sensorValue = analogRead(A0); // Posição do potênciometro
  //Serial.println(sensorValue);
  delay(100);

  pos = posMin + sensorValue / ( 1024 / ( posMax - posMin ) );
  s.write(pos);

}

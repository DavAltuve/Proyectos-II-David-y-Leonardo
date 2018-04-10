#include <Servo.h>

Servo servo;
int posServo=180;


const byte pinInt1=2;
const byte pinInt2=3;

//definicion de pins
const int motorPin1 = 8;    // 28BYJ48 In1
const int motorPin2 = 9;    // 28BYJ48 In2
const int motorPin3 = 10;   // 28BYJ48 In3
const int motorPin4 = 11;   // 28BYJ48 In4
                   
//definicion variables
int stepCounter = 0;     // contador para los pasos
int pasos=0;
 
//tablas con la secuencia de encendido (descomentar la que necesiteis)
//secuencia 1-fase
const int numSteps = 4;
const int stepsLookup[4] = { B1000, B0100, B0010, B0001 };


/*LOW to trigger the interrupt whenever the pin is low,

CHANGE to trigger the interrupt whenever the pin changes value

RISING to trigger when the pin goes from low to high,

FALLING for when the pin goes from high to low.
The Due, Zero and MKR1000 boards allows also:

HIGH to trigger the interrupt whenever the pin is high.*/



void setup() {
  //declarar pines como salida
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pinInt1),paso,RISING);
  attachInterrupt(digitalPinToInterrupt(pinInt2),pasoServo,RISING);
  servo.attach(5);
  servo.write(posServo);
}

void loop() {
  servo.write(posServo);
  delay(10);
} 

void paso(){
  noInterrupts();
  clockwise();
  interrupts();
}

void pasoServo(){
  noInterrupts();
  posServo-=5;
  interrupts();
}

void clockwise()
{
  stepCounter++;
  if (stepCounter >= numSteps) stepCounter = 0;
  setOutput(stepCounter);
  delay(20);
  pasos++;
}
 
 
void setOutput(int step)
{
  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
}

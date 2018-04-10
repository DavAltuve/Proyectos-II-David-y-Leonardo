#include <Servo.h>
//Definicion de variables:
//Servomotor
Servo servo;
int posServo=180;

//Interupciones
const byte pinInt1=2;
const byte pinInt2=3;

//definicion de pins motor de pasos
const int motorPin1 = 8;    //  In1
const int motorPin2 = 9;    //  In2
const int motorPin3 = 10;   //  In3
const int motorPin4 = 11;   //  In4
                  
int contador = 0;     // contador para los pasos dentro del ciclo definido
int pasos=0;          //contador pasos totales
const int nPasos = 4; //numero de configuraciones de pasos totales
const int defPasos[4] = { B1000, B0100, B0010, B0001 }; //configuraciones de pasos

void setup() {
  //Declaraciones y configuraciones iniciales
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
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
  //Interrupcion que se ejecuta para realizar un paso del motor
  noInterrupts();
  pasoHorario();
  interrupts();
}

void pasoServo(){
  //Interrupcion que se ejecuta para mover el servomotor
  noInterrupts();
  posServo-=5;
  interrupts();
}

void pasoHorario()
{
  //Funcion que ejecuta un paso horario.
  contador++;
  if (contador >= nPasos) contador = 0;
  salidaPaso(contador);
  delay(20);
  pasos++;
}
 
 
void salidaPaso(int paso)
{
  digitalWrite(motorPin1, bitRead(defPasos[paso], 0));
  digitalWrite(motorPin2, bitRead(defPasos[paso], 1));
  digitalWrite(motorPin3, bitRead(defPasos[paso], 2));
  digitalWrite(motorPin4, bitRead(defPasos[paso], 3));
}

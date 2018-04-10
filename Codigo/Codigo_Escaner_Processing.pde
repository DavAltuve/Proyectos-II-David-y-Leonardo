import processing.serial.*;
Serial puerto;
int paquetes=0; 
boolean sincronizado=true; 
int estado=0;
boolean convBandera=false;
byte convA;
byte convB;
int canal1;
IntList Datos;
PrintWriter Archivo;
boolean vaciar=false;
int limitMuestras=4100;
int valorAux=0;
float x,y,z;
float radio,angulo,altura=0,anguloServo=0;
float peso;
void setup(){
  puerto=new Serial(this,Serial.list()[0],115200);
  puerto.buffer(5);
  Datos=new IntList();
  Archivo=createWriter("Datos.txt");
}

void draw(){
  if(Datos.size()==limitMuestras){
    println(Datos.size());
    for(int j=0;j<limitMuestras;j++){
      /*if(Datos.get(0)>valorAux){
        valorAux=Datos.remove(0);
      }
      else{
        Datos.remove(0);
      }*/
      radio=(Datos.get(0)*cos(radians(anguloServo)))-limitMuestras;
      altura=Datos.remove(0)*sin(radians(anguloServo));
      x=radio*cos(radians(angulo))/100;
      y=radio*sin(radians(angulo))/100;
      z=altura/100;
      
      Archivo.println(x+" "+y+" "+z);
      //Archivo.println(radio);
      Archivo.flush();
      if(angulo<360.01){//360.01
        angulo+=0.17561;
      }
      else{
        angulo=0;
        anguloServo+=2.5;
      }
    }
    println("Listo");
    //println(Datos.size());
    //println(anguloServo);
    
  }
  
}

void serialEvent(Serial p){
  
      byte[] inBuffer = new byte[3];
      p.readBytes(inBuffer);
     
      for(int i=0;i<3;i++){
          if(inBuffer[i]==-14 && !sincronizado){
            sincronizado=true;
          }
          
          if(sincronizado){
            switch(estado){
              case 0:
                estado=1;
                break;
              case 1:
                if (!convBandera){
                  convA=(inBuffer[i]);
                  convBandera=true;
                }
                else{
                  convB=(inBuffer[i]);
                  //peso=bluffConversor(convA,convB);
                  //println(peso);
                  Datos.append(bluffConversor(convA,convB));
                  //Datos.append(canal1[2]);
                  //println(Datos.remove(0));
                  convBandera=false;
                  estado=0;
                }
              
                break;
            }
          }
          
      }

}

int bluffConversor(byte convA, byte convB){
  int c,d,a,b;
  int analogico;
  int[] array=new int[3];
  //digitales
  a=convA & 0x40;
  array[0]=a>>6;
  a=convA & 0x20;
  array[1]=a>>5;
  
  //analogico
  a= (convA & 0x1F);
  b= convB<<1;
  c= (a<<8);
  d=b & 0x00FF;
  d=c|d;
  analogico=d>>1;
  //array[2]=analogico;
  
  return analogico;
}
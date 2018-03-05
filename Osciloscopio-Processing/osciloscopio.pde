import processing.serial.*;
Serial puerto;
int paquetes=0; 
boolean sincronizado=false; 
int estado=0;
IntList Analog1;
int[] canal1 = new int[3];
byte convA;
byte convB;
boolean convBandera=false;
boolean flagConv; // seguridad de sincronizacion

int ancho=1280;
int alto=640;

int lastX=40;
int lastY=590;
int posX=40;
int escala=40;

void setup(){
  size(1280,640);
  puerto=new Serial(this,Serial.list()[0],115200);
  puerto.buffer(5);
  Analog1=new IntList();
  fill(255);
  ellipse(210,20,20,20);
  grip();
}

void draw(){
  if(Analog1.size()>=escala){
    if(posX<ancho-40){
      if (posX==40)
        stroke(200);
      else
        stroke(255,0,0);
        
      line(lastX,lastY,posX,Analog1.get(0));
      lastX = posX;
      lastY = Analog1.remove(0);
      posX+=escala;
    }
    else{
      grip();
      lastX=40;
      lastY=alto-40;
      posX=40;
      Analog1.clear();
    }
  }
  
  if (canal1[0]==1){
    stroke(0);
    fill(255,0,0);
    ellipse(210,20,20,20);
  }
  else{
    stroke(0);
    fill(255);
    ellipse(210,20,20,20);
  }
  
}

void serialEvent(Serial p){
  
      byte[] inBuffer = new byte[5];
      p.readBytes(inBuffer);
     
      for(int i=0;i<5;i++){
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
                  canal1=bluffConversor(convA,convB);
                  Analog1.append(canal1[2]);
                  convBandera=false;
                  estado=2;
                }
              
                break;
              case 2:
                if (!convBandera){
                  //convA=(inBuffer[i]);
                  convBandera=true;
                }
                else{
                  //convB=(inBuffer[i]);
                  //bluffConversor(convA,convB);
                  convBandera=false;
                  estado=0;
                }
                break;
            }
          }
      }
}

void keyPressed(){
  if (key=='+'){
    if (escala<80){
      escala+=10;
      grip();
      Analog1.clear();
    }
  }
  if(key=='-'){
    if(escala>10){
      escala-=10;
      grip();
      Analog1.clear();
    }
  }
}

int[] bluffConversor(byte convA, byte convB){
  int c,d,a,b;
  int analogico;
  int[] array=new int[3];
  float cm=0;
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
  cm=map(analogico,0,4096,0,3.3);
  cm=pow(cm,-1.1896);
  cm=cm*29.28766;
  print("Distancia: "+cm+"\n");
  analogico=(int)map(analogico,0,4096,alto,0);
  array[2]=analogico;
  
  return array;
}

void grip(){
  float valor=3.29;
  int x=40,y=38;
  int lineas=1200/escala;
  background(255);
  stroke(200);
  strokeWeight(2);
  line(x,40,x,height-40);
  for(int j=0;j<=lineas;j++){
    line(x,40,x,height-40);
    x+=escala;
  }
  while(y<height-40){
    line(40,y,width-40,y);
    fill(125);
    textSize(10);
    text(valor,5,y);
    valor-=0.235;
    y+=40;
  }
  fill(125);
  textSize(20);
  text("0.5ms",width/2,height-10);
}
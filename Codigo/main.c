/* ###################################################################
**     Filename    : main.c
**     Project     : Escaner
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2018-01-24, 15:20, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module conta0ins user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "DIGITAL.h"
#include "Ledini.h"
#include "Ledpeso.h"
#include "Comenzar.h"
#include "Vuelta.h"
#include "Pesar.h"
#include "ArduinoPaso.h"
#include "ArduinoServo.h"
#include "AD1.h"

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
unsigned int estado=Esperar;
unsigned char trama[3]={0xF1,0x00,0x00};
unsigned int tamano=3;
unsigned char convA,convB;
unsigned int Adc=0,Adcaux;
unsigned int posServo=1;
bool digital1=0;
int muestras=0,pasos=0;
int limitPasos=2050,limitServo=2;
void main(void)
{
  /* Write your local variable definition here */
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/

	PE_low_level_init();
  
  /* Write your code here */
  AD1_Start();
  for(;;){
	  switch(estado){
	  	  case Esperar:
	  		  break;
	  	  case Medir:
	  		  //Medir infrarojo y enviar el valor
	  		  while(muestras<10){
	  			  AD1_MeasureChan(TRUE,1);
	  			  AD1_GetChanValue16(1,&Adcaux);
	  			  Adcaux=Adcaux>>4;
	  			  Adc+=Adcaux;
	  			  muestras+=1;
	  		  }
	  		  muestras=0;
	  		  Adc=Adc/10;
	  		  digital1=DIGITAL_GetVal();
	  		//Protocolo envio
			  convB=Adc & 0x007F;
			  convA=Adc>>7;
			  convA=convA & 0x1F;
			  Vuelta_SetVal();
			  if (digital1){
				  convA=convA|0x40;
				  Vuelta_ClrVal();
			  }
			  trama[0]=0xf1;
			  trama[1]=convA;
			  trama[2]=convB;
			  //AS1_SendBlock(trama,3,&tamano);
			  //Cpu_Delay100US(10);
			  
			  estado=Mover;
	  		  break;
	  	  case Mover:
	  		  //salta a uno de dos estados o el 0 o el 1;
	  	      if(pasos==limitPasos){ //2050
	  	    	pasos=0;
	  	    	if(posServo==limitServo){
	  	    	  estado=Esperar;
	  	    	  AS1_SendBlock(trama,3,&tamano);
	  	    	  Cpu_Delay100US(20);
	  	    	  posServo=0;
	  	    	  Comenzar_Enable();
	  	    	}
	  	    	else{
	  	    	  estado=Medir;
	  	    	  //AS1_SendBlock(trama,3,&tamano);
	  	    	  ArduinoServo_NegVal();
	  	    	  Cpu_Delay100US(100);
	  	    	  ArduinoServo_NegVal();
	  	    	  posServo+=1;
	  	    	}
	  	      }
	  	      else{
	  	    	estado=Medir;
	  	    	AS1_SendBlock(trama,3,&tamano);
	  	    	Cpu_Delay100US(20);
		  		  Ledini_SetVal();
		  	      ArduinoPaso_NegVal();
		  	      Cpu_Delay100US(100);
		  	      ArduinoPaso_NegVal();
		  	      pasos+=1;
	  	      }
	  	     //Cpu_Delay100US(100);
	  		  break;
	  	  case Pesar:
	  		  estado=Esperar;
	  		  //pesar y envio el valor
	  		  AD1_MeasureChan(TRUE,1);
	  		  AD1_GetChanValue16(1,&Adc);
	  		  Adc=Adc>>4;
	  		  
	  		 convB=Adc & 0x007F;
	  		 convA=Adc>>7;
	  		 convA=convA & 0x1F;
	  		 
	  	
	  		 trama[1]=convA;
	  		 trama[2]=convB;
	  		 
	  		 AS1_SendBlock(trama,3,&tamano);
	  	     Cpu_Delay100US(20);
	  	     
	  		 Ledpeso_SetVal();
	  		  Pesar_Enable();
	  		  break;
	  		  	  
	  }
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
** 
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
 

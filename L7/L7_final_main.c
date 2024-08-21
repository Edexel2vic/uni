/* Main.c 
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "GLCD.h"
#include "splash.h"
#define _XTAL_FREQ 8000000  

typedef enum {
   Defecto = 0,
   Arriba,
   Abajo,
   Izquierda,
   Derecha,
   Select
}state_t;

state_t estado = Defecto;
char lletra_llegida = 0;
char* buf[16];
int head = 0;
int tail = 0;
int pos = 0;

void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0') 
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   };
}

void configUSART() {
   TXSTA1bits.TX9 = 0;
   TXSTA1bits.TXEN = 1;
   TXSTA1bits.SYNC = 0;
   TXSTA1bits.BRGH = 1;
   RCSTA1bits.SPEN = 1;
   RCSTA1bits.CREN = 1;
   RCSTA1bits.RX9 = 0;
   BAUDCON1bits.BRG16 = 1;
   SPBRG1 = 16;
}

void configPIC() {
   ANSELA=0x00; 
   ANSELB=0x00;                  
   ANSELC=0x00;                  
   ANSELD=0x00;
   ANSELE=0x02;
   TRISA=0x00;
   TRISB=0x00;
   TRISC=0xDF;
   TRISD=0x00;
   TRISEbits.TRISE0 = 0;
   TRISEbits.TRISE1 = 1;
   PORTD=0x00;
   PORTB=0x00;  
   
   /*Configuració de l'USART*/
   configUSART();
   
   /*Gestió d'interrupcions*/
   INTCONbits.GIE = 1;
   INTCONbits.PEIE = 1;
   
   GLCDinit();		   //Inicialitzem la pantalla
   clearGLCD(0,7,0,127);   //Esborrem pantalla
   setStartLine(0);        //Definim linia d'inici
}

void interrupt service_routine_HP(void) {
   if (PIR1bits.RCIF && PIE1bits.RCIE) {
	PIE1bits.RCIE = 0;
        lletra_llegida = RCREG;
	switch (lletra_llegida) {
	 case 'w':
	    estado = Arriba;
	    break;
	 case 'x':
	    estado = Abajo;
	    break;
	 case 'a':
	    estado = Izquierda;
	    break;
	 case 'd':
	    estado = Derecha;
	    break;
	 case 's':
	    estado = Select;
	    break;
	 default:
	    estado = Defecto;
	    break;
	}
	char buf[2];
	sprintf(buf,"%c\n",lletra_llegida);
	writeTxt(0,0,buf);
        PIR1bits.RCIF = 0;
        PIE1bits.RCIE = 1;
    }
   if (PIR1bits.TXIF && PIE1bits.TXIE) { // Check if USART transmit interrupt flag is send
      char dataToSend = buf[tail][pos++];
      if (dataToSend) TXREG = dataToSend;
      else {
	 ++tail;
	 pos = 0;
      }
      if (tail == head) PIE1bits.TXIE = 0;	//Quan s'han enviat totes les dades deshabilitem les interrupcions perquè si no ho féssim estería tota l'estona entrant al RSI
      PIR1bits.TXIF = 0;
   }
}

void main(void) {
   configPIC();
   for (char i = 0; i < 8; ++i) {	//pantalla splash
      for (char j = 0; j < 128; ++j) {
	 writeByte(i,j,bitmap[i*128+j]);
      }
   }
   __delay_ms(2000);
   clearGLCD(0,7,0,127);
   
   buf[head++]="L7 Projecte\r\n";	
   buf[head++]="Victor Llorens\r\n";
   buf[head++]="Victor Ramirez\r\n";
   
   PIE1bits.TXIE = 1; //Habilitem la transmissió per a enviar les dades
   
   writeTxt(0,1,"L7 Projecte\n");
   writeTxt(3,6,"Victor Llorens\n");
   writeTxt(5,6,"Victor Ramirez\n");
   __delay_ms(2000);
   clearGLCD(0,7,0,127);
  
   /*Activació de les interrupcions*/
   PIE1bits.RCIE = 1;	//ara podem rebre rades per l'USART
   while (1) {
      if (RCSTA1bits.FERR == 1) RCSTA1bits.FERR = 0;
      if (RCSTA1bits.OERR == 1) { RCSTA1bits.CREN = 0; RCSTA1bits.CREN = 1; }
   }
}

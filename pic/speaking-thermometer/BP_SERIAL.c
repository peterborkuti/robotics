/*
startbit : 0
stopbit : 1
0 : high level
1 : low level
when the line is not used, set it to LOW
*/

#include "BP_SERIAL.h"

void putch(unsigned char chr) {
char i;
unsigned int data;

  data = 512+2*chr+0; //1 start bit (0 value)
			  //8 data bit
			  //1 stop bit (0 value)
    //SET 10 in for cicle!

   // 8 output bits
  //DEBUG_LED=1;
   for (i=0;i<10;i++) { // lsb 1st  8 bits
	  SERIAL_OUT =  !(data & 0x01);
 	  data = ((data | 0x01) >> 1);
      __delay_us(810); // 1200 bps = 0.83 msec/bit, 8N1, no handshake.
                       // if interrupts are off, use 800 usec for waiting
   }
  //SERIAL_OUT = 0; // stop bit
  //DEBUG_LED = 0;
   
}



void putString(const char s[]){
    char i = 0;
    while ((s[i]!='\0') && (i<255)) putch(s[i++]);
}

void putStringLn(const char s[]){
    putString(s);
    putch('\n');
}

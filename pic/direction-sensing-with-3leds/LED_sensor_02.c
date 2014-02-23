/************************************************************************
*                                                                       *
*   Filename:      LED_sensor_02.c                                       *
*   Date:          2012.02.29                                           *
*   File Version:  1.0                                                  *
*                                                                       *
*   Author:        Peter Borkuti                                        *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Mid-range PIC                                        *
*   Processor:     16F684                                               *
*   Compiler:      HI-TECH C                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: delay.h                                             *
*                                                                       *
*   History:                                                            *
*   ver  description                                                    *
*     1  initial version                                                *
*                                                                       *
*                                                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    LED as photodiode                                   *
*                   the measured values put to serial                   *
*   Using 3 LEDS as photodiode with the "discharge" method.             *
*   See doc:                                                            *
*      MITSUBISHI ELECTRIC RESEARCH LABORATORIES                        *
*      http://www.merl.com                                              *
*      Very Low-Cost Sensing and Communication                          *
*      Using Bidirectional LEDs                                         *
*      Paul Dietz, William Yerazunis, Darren Leigh                      *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
;   Compares two photoresistor's value and  light a led, if             *
;   the first is lighted harder than the second                         *
;                                                                       *
;                               PIC16F684                               *
;                          +3V O    ?    O GND                          *
;                          RA5 O         O RA0                          *
;                          RA4 O         O RA1                          *
;                          RA3 O         O RA2                          *
;  |--330R------LED1------ RC5 O         O RC0 ----LEDCOMMON            *
;  +--330R------LED2------ RC4 O         O RC1                          *
;  +--330R------LED3------ RC3 O         O RC2 ----SERIAL_OUT           *
;  |                                                                    *
*  +----->LEDCOMMON                                                     *
*          LED1 on (emitting)  : RC5(OUTPUT) LOW     , RC0(OUTPUT) HIGH *
*          LED1 reverse_biased : RC5(OUTPUT) HIGH    , RC0(OUTPUT) LOW  *
*          LED1 discharge      : RC5(INPUPUT) reading, RC0(OUTPUT) LOW  *
*                                                                       *
*   WARNING! PULL DOWN the Pickit2 before testing!!!!                   *
************************************************************************/

#include <htc.h>
#include "delay.h"

#define SERIAL_DEBUG

//bits_on(x,0b0001);      //now x=0b1011
//bits_off(x,0b0011);     //now x=0b1000 */
#define bits_on(var,mask) var |= mask
#define bits_off(var,mask) var &= ~0 ^ mask

//if (!testbit(x,0)) b();  //function b() gets executed
#define testbit_on(data,bitno) ((data>>bitno)&0x01)

//unsigned int x;
#define hibyte(x)       (unsigned char)(x>>8)
#define lobyte(x)       (unsigned char)(x & 0xFF)

//#define PIC12F675

// Config: ext reset, no code protect, no watchdog, 4MHz int clock
//__CONFIG(MCLRE_ON & CP_OFF & WDTE_OFF & FOSC_INTRCIO);

#ifdef PIC12F675
	//for 12F675
	// Config: ext reset, no code protect, no watchdog, 4MHz int clock
	__CONFIG(MCLRE_ON & CP_OFF & WDTE_OFF & FOSC_INTRCIO);
	#define ANA_IN		GP1
	#define LED1 		GP0
	#define LED2 		GP2
	#define SERVO		GP3 
	#define	DEBUG_LED 	GP4
	#include "SETUP_PIC12F675.c"
#else
	//for 16F684
	// Config: ext reset, no code protect, no watchdog, 4MHz int clock
	__CONFIG(MCLRE_ON & CP_OFF & WDTE_OFF & FOSC_INTOSCIO);
	#define LED1 		RC5
	#define LED2 		RC4
	#define LED3 		RC3
	#define LEDCOMMON	RC0
	#define SERIAL_OUT	RC2
	#include "SETUP_PIC16F684.c"
#endif

#define led_emitting(x) bits_off(TRISC,0b00111000);LEDCOMMON=1;LED1=1;LED2=1;LED3=1;x=0;
#define leds_emitting bits_off(TRISC,0b00111000);LEDCOMMON=1;LED1=0;LED2=0;LED3=0;
#define leds_reverse_biased bits_off(TRISC,0b00111000);LEDCOMMON=0;LED1=1;LED2=1;LED3=1;
#define leds_discharge LEDCOMMON=0;bits_on(TRISC,0b00111000);

void init_vars(){
	SERIAL_OUT=0;
	LED1=0;
	LED2=0;
	LED3=0;
}
/*
startbit : 0
stopbit : 1
0 : high level
1 : low level
when the line is not used, set it to LOW
*/

void putch(unsigned char chr) {                     
unsigned short mask=1;
char i;                                         
unsigned short data;                                               
                                                                 
   data = 512+(chr<<1)+0; //1 start bit (0 value)
						  //8 data bit
						  //1 stop bit (1 value)
                                                   
   // 8 output bits                                             
   for (i=0;i<10;i++) { // lsb 1st  8 bits
	  if (data & 0x01) {
		SERIAL_OUT=0;
	  } else {
		SERIAL_OUT=1;
	  }                        
 	  data = (data >> 1);                                                                                 
      DelayBigUs(25); // 1200 bps, 8N1, no handshake.     
   }

   SERIAL_OUT=0;
}

void serialTest1(){
	putch(0b00000000);//0
	putch(0b01010101);//85
	putch(0b10101010);//170
	putch(0b11111111);//255
	putch(0b11111000);//248

	for (;;);
}


void serialTest2(){
	for (unsigned c = 255; c!=0; c--) {
		putch(c);
	}
    
    for (;;);
}

void serial_startDebug(){
	putch(0);
	putch(255);
	putch(0);
}
void serial_debug(unsigned char hi, unsigned char lo){
	putch(0);
	putch(0);
	putch(hi);
	putch(lo);
}

void main()
{

	init_vars();
	setup();
	
	/*
	for (;;){
		DelayS(1);
		DEBUG_LED^=1;
		LED1^=1;
		LED2^=1;
	}
	*/

	//serialTest2();

	//fill with default values
	unsigned int counter = 0;
	unsigned int cLed1 = 0;
	unsigned int cLed2 = 0;
	unsigned int cLed3 = 0;
	unsigned char tmp = 0;

 	for (;;){
            leds_emitting;
            //DelayMs(250);
            //led_emitting(LED2);
            //DelayMs(250);
            //  led_emitting(LED3);
            DelayMs(250);

            leds_reverse_biased;
            leds_discharge;
            counter=1;
            cLed1=0;
            cLed2=0;
            cLed3=0;
            while (LED1 || LED2 || LED3) {
                    counter++;
                    DelayMs(1);
                    if (! LED1 && cLed1==0 || counter==0) cLed1=counter-1;
                    if (! LED2 && cLed2==0 || counter==0) cLed2=counter-1;
                    if (! LED3 && cLed3==0 || counter==0) cLed3=counter-1;
                    if (counter==0) break;
            }
            serial_startDebug();
            putch((hibyte(cLed1))); putch(lobyte(cLed1));
            putch((hibyte(cLed2))); putch(lobyte(cLed2));
            putch((hibyte(cLed3))); putch(lobyte(cLed3));

/*
		adValue = tmp*256 + ADRESL;

		#ifdef SERIAL_DEBUG
			serial_debug(hibyte(adValue),lobyte(adValue));
		#endif

		if (adValue-THRESHOLD>ADC_MIDDLE_VALUE) motorLeft();
		if (adValue+THRESHOLD<ADC_MIDDLE_VALUE) motorRight();

		DelayMs(CONTROL_OFF_TIME);
        DEBUG_LED^=1;
*/
	}
}



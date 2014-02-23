/************************************************************************
*                                                                       *
*   Filename:      dht11_07                                             *
*   Date:          2012.10.20                                           *
*   File Version:  1.1                                                  *
*                                                                       *
*   Author:        Peter Borkuti                                        *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Mid-range PIC                                        *
*   Processor:     16F684                                               *
*   Compiler:      xc8                                                  *
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
*   Description:    reading dht11 and sending data via serial           *
*                                                                       *
*************************************************************************
*************************************************************************
*   Credits                                                             *
*   http://embedded-lab.com/blog/?p=4333                                *
*                                                                       *
*************************************************************************
;                               PIC16F684                               *
;                          +3V O    ?    O GND                          *
;              DEBUG LED   RA5 O         O RA0  ICSPDAT                 *
;                          RA4 O         O RA1  ICSPCLK                 *
;                   MCLR   RA3 O         O RA2                          *
;              DHT11_DATA  RC5 O         O RC0  SERIAL OUT              *
;              WTV020_BUSY RC4 O         O RC1  WTV020_RESET            *
;              WTV020_DATA RC3 O         O RC2  WTV020_CLK              *
;                                                                       *
************************************************************************/

#define DHT11_DATA      RC5
#define DEBUG_LED	RA5

//#define SERIAL_DEBUG    1
#define SERIAL_OUT	RC0

#define WTV020_DATA     RC3
#define WTV020_RESET    RC1
#define WTV020_CLK      RC2
#define WTV020_BUSY     RC4

// PIC12F675 Configuration Bit Settings

#include <xc.h>
#define _XTAL_FREQ 8000000

// 12F675
// __CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_ON & MCLRE_ON & BOREN_ON & CP_OFF & CPD_OFF);

// PIC16F684 Configuration Bit Settings
__CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & IESO_ON & FCMEN_ON);

//#include <htc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <pic16f684.h>
//#include "delay.c"
#include "SETUP_PIC16F684.c"

uint8_t bits[5];
uint8_t humidity = 0;
uint8_t temperature = 0;

#ifdef SERIAL_DEBUG
#include "BP_SERIAL.c"
#endif

#include "BP_DHT11.c"

#include "BP_WTV020SDMINI.c"

void init_vars(){
	SERIAL_OUT = 0;
	DEBUG_LED = 0;
        WTV020_DATA = 1;
        WTV020_RESET = 1;
        WTV020_CLK = 1;
        DHT11_DATA = 1;
}

// http://playground.arduino.cc/Main/DHT11Lib
// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
uint8_t dewPointFast(uint8_t celsius, uint8_t humidity)
{
        int a = 17;
        int b = 238;
        int temp = (a * celsius) / (b + celsius) + log(humidity/100);
        int Td = (b * temp) / (a - temp);
        return Td;
}

void sayNumber(uint8_t num){
    if (num>=100) {
        WTV020_sendCommand(100);
        return;
    }
    if (num<=30) {
        WTV020_sendCommand(num);
        return;
    }

    uint8_t dec = num / 10;
    uint8_t ones= num % 10;

    if (dec>0) {
        WTV020_sendCommand(100+dec);
    }

    WTV020_sendCommand(ones);

}

void testSay(){
    for (uint8_t i = 1; i<31; i++) {
        sayNumber(i);
    }
    for (uint8_t i = 40; i<101; i+=10) {
        sayNumber(i);
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    uint8_t otemp = 0;
    uint8_t dewpoint = 0;

    init_vars();
    pic_init();
    WTV020_reset();
    WTV020_sendCommand(WTV020_VOLUME_MAX);
    DEBUG_LED = 0;

    while (1) {
        DEBUG_LED = 0;
        __delay_ms(2000);
        DEBUG_LED = 1;
        DHT11_READ();

        //if (otemp != temperature ) {
            WTV020_sendCommand(201);
            sayNumber(temperature);
            WTV020_sendCommand(202);

            WTV020_sendCommand(203);
            sayNumber(humidity);
            WTV020_sendCommand(204);

            /*
            dewpoint = dewPointFast(temperature, humidity);
            WTV020_sendCommand(205);
            sayNumber(dewpoint);
            WTV020_sendCommand(202);
            */
        //}

        otemp = temperature;
   
    }
    return;
}


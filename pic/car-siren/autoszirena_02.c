/* 
 * File:   autoszirena_02.c
 * Author: Peter
 *
 * Created on December 8, 2012, 9:23 PM
 */

/*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
;   Compares two photoresistor's value and  light a led, if             *
;   the first is lighted harder than the second                         *
;                                                                       *
;                               PIC12F675                               *
;                          +3V O    ?    O GND                          *
;                          GP5 O         O GP0  ICSPDAT                 *
;             DEBUG_LED    GP4 O         O GP1  ICSPCLK                 *
;             MCLR         GP3 O         O GP2                          *
************************************************************************/


#define _XTAL_FREQ 4000000

// PIC12F675 Configuration Bit Settings

#include <xc.h>

__CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_ON & MCLRE_ON & BOREN_ON & CP_OFF & CPD_OFF);

#include <stdio.h>
#include <stdlib.h>
#include <pic12f675.h>
#include "SETUP_PIC12F675.c"

#define DEBUG_LED	GP4

/*
 * 
 */

void sound(int d, int p){
    for (volatile int i=1; i<d; i++){
        DEBUG_LED = 1;
        for (volatile int z=0; z<p; z++) {__delay_us(1); }
        DEBUG_LED = 0;
        for (volatile int z=0; z<p; z++) {__delay_us(1); }
    }
}


int main(int argc, char** argv) {
    pic_setup();
    volatile int i=0;
    int div=100  ;
    int gate1=300;

    int gate2=1000;
    int t1 = gate1;
    int t2 = gate2-t1;

 while (1) {
     sound(300,30);
     sound(150,45);
  }



     while (1) {
        DEBUG_LED = 1;
        for (i=0; i<t1; i+=1) { __delay_us(1); }

        DEBUG_LED = 0;
        for (i=0; i<t2; i+=1) { __delay_us(1); }

        t1+=div;
        t2-=div;

        if (t1>gate2 || t1<gate1) {
            div=-div;;
        }

    }

    return (EXIT_SUCCESS);
}


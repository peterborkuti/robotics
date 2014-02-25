/* 
 * File:   GLOBALS.h
 * Author: peter
 *
 * Created on February 25, 2014, 10:17 PM
 */
/*
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

#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DHT11_DATA      RC5
#define DEBUG_LED       RA5

//#define SERIAL_DEBUG    1
#define SERIAL_OUT      RC0

#define WTV020_DATA     RC3
#define WTV020_RESET    RC1
#define WTV020_CLK      RC2
#define WTV020_BUSY     RC4

#define _XTAL_FREQ 8000000

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */


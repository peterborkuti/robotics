/* 
 * File:   BP_DHT11.h
 * Author: Peter Borkuti
 *
 * Created on February 25, 2014, 9:55 PM
 */

/*

 DHT11 tempperature and humidity sensor

  FRONT view
      _______
      |     |
      |     |
      |     |
      |_____|
       1 2 3 4
       + |   GND
         |
   + --5k+---DATA OUT


 */

#ifndef BP_DHT11_H
#define	BP_DHT11_H

#include <xc.h>
#include <stdint.h>
#include <pic16f684.h>

#include "GLOBALS.h"

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t bits[5];
uint8_t humidity = 0;
uint8_t temperature = 0;

uint16_t DHT11_READ_BYTES();
void DHT11_READ();

#ifdef	__cplusplus
}
#endif

#endif	/* BP_DHT11_H */
/* 
 * File:   BP_WTV020SDMINI.h
 * Author: Peter Borkuti
 *
 * Created on February 25, 2014, 10:04 PM
 */

/*
 WTV020-SD-MINI
 TWO LINE SERIAL MODE

1  RESET 1      o     16  VDD+
2        2            15  BUSY
3        3            14
4  SPK+  4            13
5  SPK-  5            12
6        6            11
7  CLOCK 7            10  DATAINPUT
8  GND   8             9



*/

#ifndef BP_WTV020SDMINI_H
#define	BP_WTV020SDMINI_H

#include "GLOBALS.h"
#include <xc.h>
#include <pic16f684.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define LOW     0
#define HIGH    1
#define WTV020_VOLUME_MAX   0xFFF7

void WTV020_sendCommand(uint16_t command);
void WTV020_reset();

#ifdef	__cplusplus
}
#endif

#endif	/* BP_WTV020SDMINI_H */


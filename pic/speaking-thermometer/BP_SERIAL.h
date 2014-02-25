/* 
 * File:   BP_SERIAL.h
 * Author: peter
 *
 * Created on February 25, 2014, 10:10 PM
 */

/*
startbit : 0
stopbit : 1
0 : high level
1 : low level
when the line is not used, set it to LOW
*/

#ifndef BP_SERIAL_H
#define	BP_SERIAL_H

#include "GLOBALS.h"
#include <xc.h>
#include <pic16f684.h>

#ifdef	__cplusplus
extern "C" {
#endif

void putch(unsigned char chr);
void putString(const char s[]);
void putStringLn(const char s[]);

#ifdef	__cplusplus
}
#endif

#endif	/* BP_SERIAL_H */


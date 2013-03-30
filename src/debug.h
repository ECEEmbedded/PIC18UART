#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#include "maindefs.h"

/*
 * ##################################################
 * DebugPrint
 *  Output lights to first four ports on PORT A
 * ##################################################
 */
void DebugPrint(unsigned char out);

// Prints 0x0, upper nibble, lower nibble, 0x0F.
// The 0 and F are to surround it for ease in spotting on the logic analizer
void DebugPrintByte(unsigned char out);

void DebugPrintString(unsigned char *msg, unsigned char len);

/*
 * ##################################################
 * DebugSignal
 * Just display some lights, good for startup test
 * ##################################################
 */
void DebugSignal();


#endif

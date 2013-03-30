#include "debug.h"

void DebugPrint(unsigned char out) {
    //Convert any numbers to just 1 or 0

    LATA = out & 0x0F;
}

void DebugPrintByte(unsigned char out) {
    unsigned char tmpH = (out >> 4) & 0x0F;
    unsigned char tmpL = out & 0x0F;
    LATA = 0x0F;
    LATA = tmpH;
    LATA = 0x0F;
    LATA = tmpL;
    LATA = 0x0F;
    LATA = 0x00;
}

void DebugPrintString(unsigned char *msg, unsigned char len) {
    char i;
    for (i=0;i<len;++i){
        DebugPrintByte(msg[i]);
    }
}
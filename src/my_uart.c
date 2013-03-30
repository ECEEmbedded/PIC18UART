#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#include <timers.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"
#include "debug.h"

static uart_comm *uc_ptr;

void start_UART_send(unsigned char len, unsigned char * msg) {
    for (uc_ptr->outbuflen = 0; uc_ptr->outbuflen < len; ++uc_ptr->outbuflen) {
        uc_ptr->outbuffer[uc_ptr->outbuflen] = msg[uc_ptr->outbuflen];
    }

    uc_ptr->outbufind = 0;

#ifdef __USE18F26J50
    Write1USART(uc_ptr->outbuffer[0]);
#else
    PIE1bits.TXIE = 1;
#endif
}

void init_uart_snd_rcv(uart_comm *uc) {
    uc_ptr = uc;
    uc_ptr->buflen = 0;
//    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
//            USART_CONT_RX & USART_BRGH_LOW, 0x19);

    // Interrupts
    INTCONbits.PEIE = 1;
    PIE1bits.TXIE = 1;
    PIE1bits.RCIE = 1;
    TXSTAbits.TXEN = 1;

    TRISCbits.TRISC7 = 1; // Tx = output pin
    TRISCbits.TRISC6 = 0; // Rx = input pin
    
    // Baud Rate
    TXSTAbits.BRGH = 1;     // High baud rate
    BAUDCONbits.BRG16 = 0;  // 8 bit SPBRG
    SPBRG = 0x65;           // With current clock 19200 baud rate
    
    TXSTAbits.SYNC = 0; // Asychronous
    RCSTAbits.SPEN = 1; // Serial Port Enable
    CREN = 1;
}


void uart_send_int_handler(void) {
    if (uc_ptr->outbufind < uc_ptr->outbuflen) {
#ifdef __USE18F26J50
        Write1USART(uc_ptr->outbuffer[uc_ptr->outbufind]);
#else
        TXREG = uc_ptr->outbuffer[uc_ptr->outbufind];
#endif
        ++uc_ptr->outbufind;
    }
    else { // End of message
        PIE1bits.TXIE = 0;
    }
}

void uart_recv_int_handler() {
    if (DataRdyUSART()) {

        uc_ptr->buffer[uc_ptr->buflen] = RCREG;

        // Frame allignment check
        if (uc_ptr->buflen == 0 && uc_ptr->buffer[0] != I2C_MOTOR_ADDRESS) {
            uc_ptr->buflen = 0;
            return;
        }

        uc_ptr->buflen++;

        if (uc_ptr->buflen == MAXUARTBUF) {
            ToMainLow_sendmsg(uc_ptr->buflen, MSGT_UART_DATA, (void *) uc_ptr->buffer);
            uc_ptr->buflen = 0;
        };

        if (RCSTAbits.OERR == 1) {
            // we've overrun the USART and must reset
            // send an error message for this
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
            ToMainLow_sendmsg(0, MSGT_OVERRUN, (void *) 0);
        }
    }
}

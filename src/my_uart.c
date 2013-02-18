#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"

static uart_comm *uc_ptr;

void start_UART_send(unsigned char len, unsigned char * msg) {
    for (uc_ptr->outbuflen = 0; uc_ptr->outbuflen < len; ++uc_ptr->outbuflen) {
        uc_ptr->outbuffer[uc_ptr->outbuflen] = msg[uc_ptr->outbuflen];
    }
    uc_ptr->outbuflen = len;

    uc_ptr->outbufind = 1;

#ifdef __USE18F26J50
    Write1USART(uc_ptr->outbuffer[0]);
#else
    WriteUSART(uc_ptr->outbuffer[0]);
#endif

}

void uart_recv_int_handler() {
#ifdef __USE18F26J50
    if (DataRdy1USART()) {
        uc_ptr->buffer[uc_ptr->buflen] = Read1USART();
#else
    if (DataRdyUSART()) {
        uc_ptr->buffer[uc_ptr->buflen] = ReadUSART();
#endif

        uc_ptr->buflen++;
        // check if a message should be sent
        if (uc_ptr->buflen == MAXUARTBUF) {
            ToMainLow_sendmsg(uc_ptr->buflen, MSGT_UART_DATA, (void *) uc_ptr->buffer);
            uc_ptr->buflen = 0;
        }
    }
#ifdef __USE18F26J50
    if (USART1_Status.OVERRUN_ERROR == 1) {
#else
    if (USART_Status.OVERRUN_ERROR == 1) {
#endif
        // we've overrun the USART and must reset
        // send an error message for this
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        ToMainLow_sendmsg(0, MSGT_OVERRUN, (void *) 0);
    }
}

void init_uart_snd_rcv(uart_comm *uc) {
    uc_ptr = uc;
    uc_ptr->buflen = 0;

    // configure the hardware USART device
#ifdef __USE18F26J50
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
        USART_CONT_RX & USART_BRGH_LOW, 0x19);
#else
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
            USART_CONT_RX & USART_BRGH_LOW, 0x19);
#endif
}


void uart_send_int_handler(void) {
    if (uc_ptr->outbufind < uc_ptr->outbuflen) {
#ifdef __USE18F26J50
        Write1USART(uc_ptr->outbuffer[uc_ptr->outbufind]);
#else
        WriteUSART(uc_ptr->outbuffer[uc_ptr->outbufind]);
#endif
        ++uc_ptr->outbufind;
        INTCONbits.PEIE = 1;
    }
}
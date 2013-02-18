#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"

#define MAXUARTBUF 8
#if (MAXUARTBUF > MSGLEN)
#define MAXUARTBUF MSGLEN
#endif
typedef struct __uart_comm {
	unsigned char buffer[MAXUARTBUF];
	unsigned char	buflen;
        unsigned char outbuffer[MAXUARTBUF];
        unsigned char   outbuflen;
        unsigned char   outbufind;
} uart_comm;

void init_uart_snd_rcv(uart_comm *);
void start_UART_send(unsigned char len, unsigned char * msg);
void uart_recv_int_handler(void);
void uart_send_int_handler(void);

#endif

#ifndef __my_i2c_h
#define __my_i2c_h

#include "messages.h"

#define MAXI2CBUF MSGLEN
typedef struct __i2c_comm {
	unsigned char buffer[MAXI2CBUF];
	unsigned char	buflen;
	unsigned char	event_count;
	unsigned char	status;
	unsigned char	error_code;
	unsigned char	error_count;
	unsigned char outbuffer[MAXI2CBUF];
	unsigned char outbuflen;
	unsigned char outbufind;
	unsigned char slave_addr;
} i2c_comm;

typedef struct __msgarr {
	unsigned char	length;
	unsigned char data[MSGLEN];
} i2cmsg;

#define I2C_IDLE 0x5
#define I2C_STARTED 0x6
#define	I2C_RCV_DATA 0x7
#define I2C_SLAVE_SEND 0x8

#define I2C_ERR_THRESHOLD 1
#define I2C_ERR_OVERRUN 0x4
#define I2C_ERR_NOADDR 0x5
#define I2C_ERR_NODATA 0x6
#define I2C_ERR_MSGTOOLONG 0x7
#define I2C_ERR_MSG_TRUNC 0x8

#define I2C_REG_COUNT 4
i2cmsg i2c_addressable_registers[I2C_REG_COUNT]; // 0 is 0xA8, 1 is 0xA9, ect.

void init_i2c(i2c_comm *);
void i2c_int_handler(void);
void start_i2c_slave_reply(/*unsigned char length,unsigned char *msg*/);
void i2c_configure_slave(unsigned char);
void i2c_configure_master(unsigned char);
unsigned char i2c_master_send(unsigned char,unsigned char *);
unsigned char i2c_master_recv(unsigned char);

#endif
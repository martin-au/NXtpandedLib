#ifndef __I2C_H__
#define __I2C_H__

#include "mytypes.h"

#define I2C_N_PORTS 4

#define I2C_ERR_INVALID_PORT -1
#define I2C_ERR_BUSY -2
#define I2C_ERR_FAULT -3
#define I2C_ERR_INVALID_LENGTH -4
#define I2C_ERR_BUS_BUSY -5

// Options
#define I2C_LEGO_MODE 1
#define I2C_ALWAYS_ACTIVE 2
#define I2C_NO_RELEASE 4
#define I2C_HIGH_SPEED 8

int i2c_enable(int port, int mode);
void i2c_disable(int port);
void i2c_disable_all(void);

void i2c_init(void);

int i2c_status(int port);
int i2c_start(int port, 
              U32 address, 
              U8 *data,
              int write_len,
              int read_len);

int i2c_complete( int port, U8 *data, U32 nbytes);
void i2c_test(void);

/*Backward*/

/* backward */
int
i2c_busy(int port)
{
	return i2c_status(port);
}

int i2c_start_transaction(int port, 
                      U32 address, 
                      int internal_address, 
                      int n_internal_address_bytes, 
                      U8 *data, 
                      U32 nbytes,
                      int write)
{
	U8 send_data[1] = {internal_address};
	
	int ret = i2c_start(port, address, send_data, 1, nbytes);
	
	if(ret < 0)
		return ret;
	
	while(i2c_status(port) != 0);
	return i2c_complete(port, data, nbytes);
}

#endif

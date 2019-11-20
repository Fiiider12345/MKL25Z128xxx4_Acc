/*
 * LIBRARYI2C.h
 *
 *  Created on: Nov 15, 2019
 *      Author: marek
 */

#ifndef ACCELEROMETER_LIBRARYI2C_H_
#define ACCELEROMETER_LIBRARYI2C_H_

#include <fsl_i2c.h>

class LIBRARY_I2C {
private:
	I2C_Type* I2C_base;
	i2c_master_config_t master_config;
	i2c_master_transfer_t master_xfer;
public:
	LIBRARY_I2C(I2C_Type* I2C_base, uint32_t frequency_i2c);
	virtual ~LIBRARY_I2C();

	void i2c_read(uint8_t s_addr, uint8_t reg_addr, uint8_t* data, uint8_t len);
	void i2c_write(uint8_t s_addr, uint8_t reg_addr, uint8_t* data, uint8_t len);
};

#endif /* ACCELEROMETER_LIBRARYI2C_H_ */

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
	I2C_Type* I2C_base;	///I2C base pointer
	i2c_master_config_t master_config;///A pointer to the master configuration structure
	i2c_master_transfer_t master_xfer;	///I2C master transfer structure

public:
	/**
	 * Constructor, I2C communication init
	 * @param I2C_base - I2C base pointer
	 * @param frequency_i2c - I2C peripheral clock frequency in Hz
	 */
	LIBRARY_I2C(I2C_Type* I2C_base, uint32_t frequency_i2c);

	/**
	 * Destructor
	 */
	virtual ~LIBRARY_I2C();

	/**
	 * Master receives from the slave.
	 * @param s_addr - slave address
	 * @param reg_addr - subaddress
	 * @param data - data
	 * @param len - lenght of data
	 */
	void i2c_read(uint8_t s_addr, uint8_t reg_addr, uint8_t* data, uint8_t len);

	/**
	 * Master transmits to the slave.
	 * @param s_addr - slave address
	 * @param reg_addr - subaddress
	 * @param data - data
	 * @param len - lenght of data
	 */
	void i2c_write(uint8_t s_addr, uint8_t reg_addr, uint8_t* data,
			uint8_t len);
};

#endif /* ACCELEROMETER_LIBRARYI2C_H_ */

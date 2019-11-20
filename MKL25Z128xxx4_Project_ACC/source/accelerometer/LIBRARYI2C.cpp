/*
 * LIBRARYI2C.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: marek
 */

#include <accelerometer/LIBRARYI2C.h>

//i2c communication init
LIBRARY_I2C::LIBRARY_I2C(I2C_Type* I2C_base, uint32_t frequency_i2c) {
	// TODO Auto-generated constructor stub ..
	this->I2C_base = I2C_base;
	I2C_MasterGetDefaultConfig(&master_config);
	I2C_MasterInit(I2C_base, &master_config, frequency_i2c);

	memset(&master_xfer, 0, sizeof(master_xfer));
}

LIBRARY_I2C::~LIBRARY_I2C() {
	// TODO Auto-generated destructor stub
}

//(slave address, subaddres, data, data len)
void LIBRARY_I2C::i2c_read(uint8_t s_addr, uint8_t reg_addr, uint8_t* data, uint8_t len) {
	master_xfer.slaveAddress=s_addr;
	master_xfer.direction = kI2C_Read;
	master_xfer.subaddress = reg_addr;
	master_xfer.subaddressSize = 1;
	master_xfer.data = data;
	master_xfer.dataSize = len;
	master_xfer.flags = kI2C_TransferDefaultFlag;
	I2C_MasterTransferBlocking(I2C_base, &master_xfer);
}

//(slave address, subaddres, data, data len)
void LIBRARY_I2C::i2c_write(uint8_t s_addr, uint8_t reg_addr, uint8_t* data, uint8_t len) {
	master_xfer.slaveAddress=s_addr;
	master_xfer.direction = kI2C_Write;
	master_xfer.subaddress = reg_addr;
	master_xfer.subaddressSize = 1;
	master_xfer.data = data;
	master_xfer.dataSize = len; //+1
	master_xfer.flags = kI2C_TransferDefaultFlag;
	I2C_MasterTransferBlocking(I2C_base, &master_xfer);
}

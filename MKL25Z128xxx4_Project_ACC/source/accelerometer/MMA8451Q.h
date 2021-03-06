/* Copyright (c) 2010-2011 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MMA8451Q_H
#define MMA8451Q_H

#include <accelerometer/LIBRARYI2C.h>
#include <math.h>

#define REG_WHO_AM_I      0x0D	///Device ID (0x1A)
#define REG_CTRL_REG_1    0x2A	///ODR = 800 Hz, standby mode.
#define REG_CTRL_REG_4    0x2D	///Interrupt enable register
#define REG_CTRL_REG_5    0x2E	///Interrupt pin (INT1/INT2) map
#define REG_OUT_X_MSB     0x01	///[7:0] are eight MSBs of 14-bit sample.
#define REG_OUT_Y_MSB     0x03	///[7:0] are eight MSBs of 14-bit realtime sample
#define REG_OUT_Z_MSB     0x05	///[7:0] are eight MSBs of 14-bit realtime sample
#define REG_FF_MT_CONFIG  0x15	///Freefall/motion functional block configuration
#define REG_FF_MT_THS     0x17	///Freefall/motion threshold register
#define REG_FF_MT_COUNT   0x18	///Freefall/motion debounce counter
#define REG_FF_MT_SRC     0x16	///Freefall/motion event source register

#define PI 3.141592653589793	///Value of PI
#define UINT14_MAX        16383

class MMA8451Q {
public:
	/**
	 * MMA8451Q constructor - activate the i2c peripheral, acc, freefall init
	 * @param sda SDA pin
	 * @param sdl SCL pin
	 * @param addr addr of the I2C peripheral
	 */
	MMA8451Q(uint8_t my_i2c_addr);

	/**
	 * MMA8451Q destructor
	 */
	~MMA8451Q();

	/**
	 * Get the value of the WHO_AM_I register
	 *
	 * @returns WHO_AM_I value
	 */
	uint8_t getWhoAmI();

	/**
	 * Get X axis acceleration
	 *
	 * @returns X axis acceleration
	 */
	float getAccX();

	/**
	 * Get Y axis acceleration
	 *
	 * @returns Y axis acceleration
	 */
	float getAccY();

	/**
	 * Get Z axis acceleration
	 *
	 * @returns Z axis acceleration
	 */
	float getAccZ();

	/**
	 * Get XYZ axis acceleration
	 *
	 * @param res array where acceleration data will be stored
	 */
	void getAccAllAxis(float * res);

	/**
	 * High pass filter
	 * @param data - pointer to data handler
	 * @param current - pointer to current data
	 * @param previous - pointer to previous data
	 * @param dT_us - sampling period
	 * @param fc_Hz - break frequency
	 */
	void high_pass_filter(float* data, float* current, float* previous,
			int dT_us, int fc_Hz);

	/**
	 * Low pass filter
	 * @param data - pointer to data handler
	 * @param current - pointer to current data
	 * @param dT_us - sampling period
	 * @param fc_Hz - break frequency
	 */
	void low_pass_filter(float* data, float* current, int dT_us, float fc_Hz);

	/**
	 * Tilt sensing of axis X
	 * @param low_pass_data - pointer to low pass data handler
	 * @return a Tilt sensing value of axis X
	 */
	float getNaklonX(float* low_pass_data);

	/**
	 * Tilt sensing of axis Y
	 * @param low_pass_data - pointer to low pass data handler
	 * @return a Tilt sensing value of axis Y
	 */
	float getNaklonY(float* low_pass_data);

	/**
	 * Freefall init
	 */
	void freeFallInit();

	/**
	 * Freefall detection
	 */
	void freeFallDetect();

private:
	LIBRARY_I2C* my_i2c;	///pointer to LIBRARY_I2C class
	uint8_t my_i2c_addr;	///address of i2c

	/**
	 * Master receives from the slave.
	 * @param reg_addr - subaddress
	 * @param data - data
	 * @param len - lenght of data
	 */
	void readRegs(uint8_t reg_addr, uint8_t * data, int len);

	/**
	 * Master transmits to the slave.
	 * @param reg_addr - subaddress
	 * @param data - data
	 * @param len - lenght of data
	 */
	void writeRegs(uint8_t reg_addr, uint8_t * data, int len);

	/**
	 * Get value from acc axis
	 * @param addr - address of i2c
	 * @return value from acc axis
	 */
	int16_t getAccAxis(uint8_t addr);

};

#endif

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

#include <math.h>
#include "MMA8451Q.h"

#define REG_WHO_AM_I      0x0D	//Device ID (0x1A)
#define REG_CTRL_REG_1    0x2A	//ODR = 800 Hz, standby mode.
#define REG_CTRL_REG_4    0x2D	//Interrupt enable register
#define REG_CTRL_REG_5    0x2E	//Interrupt pin (INT1/INT2) map
#define REG_OUT_X_MSB     0x01	//[7:0] are eight MSBs of 14-bit sample.
#define REG_OUT_Y_MSB     0x03	//[7:0] are eight MSBs of 14-bit realtime sample
#define REG_OUT_Z_MSB     0x05	//[7:0] are eight MSBs of 14-bit realtime sample
#define REG_FF_MT_CONFIG  0x15	//Freefall/motion functional block configuration
#define REG_FF_MT_THS     0x17	//Freefall/motion threshold register
#define REG_FF_MT_COUNT   0x18	//Freefall/motion debounce counter
#define REG_FF_MT_SRC     0x16	//Freefall/motion event source register

#define PI 3.141592653589793
#define UINT14_MAX        16383

//acc, freefall init
MMA8451Q::MMA8451Q(uint8_t my_i2c_addr) {
    // activate the peripheral
	this->my_i2c_addr=my_i2c_addr;
	my_i2c = new LIBRARY_I2C(I2C0, CLOCK_GetBusClkFreq());

    uint8_t data[1] = {0x01};
    //ACC WAKE - SYSMOD = 01
    writeRegs(REG_CTRL_REG_1, data, 1);

    freeFallInit();
}

MMA8451Q::~MMA8451Q() {
	delete my_i2c;
}

uint8_t MMA8451Q::getWhoAmI() {
    uint8_t who_am_i = 0;
    readRegs(REG_WHO_AM_I, &who_am_i, 1);
    return who_am_i;
}

float MMA8451Q::getAccX() {
    return (float(getAccAxis(REG_OUT_X_MSB))/4096.0);
}

float MMA8451Q::getAccY() {
    return (float(getAccAxis(REG_OUT_Y_MSB))/4096.0);
}

float MMA8451Q::getAccZ() {
    return (float(getAccAxis(REG_OUT_Z_MSB))/4096.0);
}

void MMA8451Q::getAccAllAxis(float * res) {
    res[0] = getAccX();
    res[1] = getAccY();
    res[2] = getAccZ();
}

int16_t MMA8451Q::getAccAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2];
    readRegs(addr, res, 2);

    acc = (res[0] << 6) | (res[1] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;

    return acc;
}

//read acc regs : subaddres, data, data len
void MMA8451Q::readRegs(uint8_t reg_addr, uint8_t * data, int len) {
    my_i2c->i2c_read(my_i2c_addr, reg_addr, data, len);
}

//write acc regs : subaddres, data, data len
void MMA8451Q::writeRegs(uint8_t reg_addr, uint8_t * data, int len) {
	my_i2c->i2c_write(my_i2c_addr, reg_addr, data, len);
}

//hornopriepustny filter : high_pass_data, current, previous, PERIODA, 1
void MMA8451Q::high_pass_filter(float* data, float* current, float* previous, int dT_us, int fc_Hz) {
	float alfa = (float)1 / (2*PI * ((float)dT_us/1000000) * (float)fc_Hz + (float)1 );
	//high-pass filter for X
	data[0]= alfa*data[0] + (alfa * (current[0]-previous[0]) );
	//high-pass filter for Y
	data[1]= alfa*data[1] + (alfa * (current[1]-previous[1]) );
	//high-pass filter for Z
	data[2]= alfa*data[2] + (alfa * (current[2]-previous[2]) );

	/**
	 * posum vzoriek vzorka(i-1) = vzorka(i)
	 * v pripade nutnosti zachovania vzoriek odstrániť!
	 */

	previous[0]=current[0];
	previous[1]=current[1];
	previous[2]=current[2];

}

//dolnopriepustny filter : down_pass_data, current, PERIODA, 0.5
void MMA8451Q::low_pass_filter(float* data, float* current, int dT_us, float fc_Hz) {
	float alfa = ( 2*PI * ((float)dT_us/1000000) * (float)fc_Hz ) / ( 2*PI * ((float)dT_us/1000000) * (float)fc_Hz + (float)1 );

	//down-pass filter for X
	data[0]= alfa*current[0] + (1-alfa)*data[0];
	//down-pass filter for Y
	data[1]= alfa*current[1] + (1-alfa)*data[1];
	//down-pass filter for Z
	data[2]= alfa*current[2] + (1-alfa)*data[2];
}

//naklon osi X : down_pass_data
float MMA8451Q::getNaklonX(float* low_pass_data){
	return (360/(2*PI)) * atan((low_pass_data[0])/(sqrt( pow( low_pass_data[1],2) + pow(low_pass_data[2],2) )));
}

//naklon osi Y : down_pass_data
float MMA8451Q::getNaklonY(float* low_pass_data){
	return (360/(2*PI)) * atan((low_pass_data[1])/(sqrt( pow( low_pass_data[0],2) + pow(low_pass_data[2],2) )));
}

//freefall initialization
void MMA8451Q::freeFallInit() {
	uint8_t data=0x20;
	writeRegs(REG_CTRL_REG_1, &data, 1);	//Set the device in 50 Hz ODR, Standby
	data=0xB8;
	writeRegs(REG_FF_MT_CONFIG, &data, 1);	//Configuration Register set for Freefall Detection enabling “AND” condition, OAE = 0, Enabling X,	Y, Z and the Latch
	data=0x03;
	writeRegs(REG_FF_MT_THS, &data, 1);		//Threshold Setting Value for the resulting acceleration < 0.2g
	data=0x06;
	writeRegs(REG_FF_MT_COUNT, &data, 1);	//Set the debounce counter to eliminate false positive readings for 50Hz sample rate with a	requirement of 120 ms timer, assuming Normal Mode.
	data=0x04;
	writeRegs(REG_CTRL_REG_4, &data, 1);	//Enable Motion/Freefall Interrupt Function in the System (CTRL_REG4)
	data=0x00;
	writeRegs(REG_CTRL_REG_5, &data, 1);	//Route the Motion/Freefall Interrupt Function to INT2 hardware pin (CTRL_REG5)

	readRegs(REG_CTRL_REG_1, &data, 1);		//Put the device in Active Mode, 50 Hz
	data |= 0x01;
	writeRegs(REG_CTRL_REG_1, &data, 1);	//Put the device in Active Mode, 50 Hz
}

//freefall detection
void MMA8451Q::freeFallDetect() {
	uint8_t data;
	readRegs(REG_FF_MT_SRC, &data, 1);		//This register keeps track of the acceleration event which is triggering  the event flag.
}

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

#include "LIBRARYI2C.h"

class MMA8451Q
{
public:
  /**
  * MMA8451Q constructor
  *
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
     * dT_us - perioda vzorkovania v us
     * fc_hz - zlomova frekvencia v Hz
     */
    void high_pass_filter(float* data, float* current, float* previous, int dT_us, int fc_Hz);
    void low_pass_filter(float* data, float* current, int dT_us, float fc_Hz);
    float getNaklonX(float* low_pass_data);
    float getNaklonY(float* low_pass_data);
    void freeFallInit();
    void freeFallDetect();

private:
  LIBRARY_I2C* my_i2c;
  uint8_t my_i2c_addr;
  void readRegs(uint8_t reg_addr, uint8_t * data, int len);
  void writeRegs(uint8_t reg_addr, uint8_t * data, int len);
  int16_t getAccAxis(uint8_t addr);

};

#endif

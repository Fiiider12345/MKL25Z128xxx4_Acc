/*
 * CircularBufferUart.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

/*
 * this source is from https://github.com/hudikm/TPRIS_CIRCULAR_BUFFER.git
 */

#ifndef COMMUNICATION_CIRCULARBUFFERUART_H_
#define COMMUNICATION_CIRCULARBUFFERUART_H_

#include <fsl_lpsci.h>
#include <communication/CircularBuffer.h>

#define _bufferUartSize 150
#define BAUDRATE 57600
//#define BAUDRATE 230400
extern buffer_t cBuffer;

/**
 * function read one bytes from received buffer
 * @param pointer to variable where received char will by return
 * @return 0 when not success (empty buffer/ nothing to read) not 0 when success received char (not empty buffer)
 */
uint8_t get_char(uint8_t* ch);

/**
 * function initialize circular buffer and interupt from UART
 */
void CBufferUart_Init();

#endif /* COMMUNICATION_CIRCULARBUFFERUART_H_ */

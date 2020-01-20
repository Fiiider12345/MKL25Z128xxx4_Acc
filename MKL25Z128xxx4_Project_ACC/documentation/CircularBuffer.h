/*
 * CircularBuffer.h
 *
 *  Created on: Nov 25, 2019
 *      Author: marek
 */

/*
 * Zdroj
 * https://github.com/hudikm/TPRIS_CIRCULAR_BUFFER
 */

#ifndef COMMUNICATION_CIRCULARBUFFER_H_
#define COMMUNICATION_CIRCULARBUFFER_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/**
 * Enum of buffer_t condition
 */
enum {
	BUFF_EMPTY, BUFF_FREE, BUFF_FULL
};

/**
 * buffer_t struct
 */
typedef struct {
	uint8_t *buf;
	uint8_t *head, *tail;
	size_t size;
	uint8_t full;
} buffer_t;

/**
 * Copy data in the buffer until is full
 * @param buff - circular buffer handle
 * @param data_pt - pointer to data buffer
 * @param count -  size of data
 * @return number of copied bytes
 */
int16_t bufferWrite(buffer_t* buff, uint8_t* element, uint16_t count);

/**
 * Retrieve a value from the buffer
 * @param buff - pointer to buffer_t
 * @param data_pt - pointer to data handler
 * @param count - number red bytes
 * @return number red bytes
 */
int16_t bufferRead(buffer_t* buff, uint8_t* element, uint16_t count);

/**
 * Initialize the circular buffer handler
 * @param buff - pointer to buffer_t to init
 * @param bufferPt - pointer to bufferPt
 * @param size size of buffer_t
 */
void bufferInit(buffer_t* buff, uint8_t* bufferPt, uint16_t size);

/**
 * Returns the maximum capacity of the buffer
 * @param buff - pointer to buffer_t
 * @return the maximum capacity of the buffe
 */
size_t bufferCapacity(buffer_t* buff);

/**
 * Returns the current number of free bytes in the buffer
 * @param rb - pointer to buffer_t
 * @return the current number of free bytes in the buffer
 */
size_t bufferBytesFree(const buffer_t *rb);

#endif /* COMMUNICATION_CIRCULARBUFFER_H_ */

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

enum {
	BUFF_EMPTY, BUFF_FREE, BUFF_FULL
};

typedef struct {
	uint8_t *buf;
	uint8_t *head, *tail;
	size_t size;
	uint8_t full;
} buffer_t;

int16_t bufferWrite(buffer_t* buff, uint8_t* element, uint16_t count);
int16_t bufferRead(buffer_t* buff, uint8_t* element, uint16_t count);
void bufferInit(buffer_t* buff, uint8_t* bufferPt, uint16_t size);
size_t bufferCapacity(buffer_t* buff);

size_t bufferBytesFree(const buffer_t *rb);

#endif /* COMMUNICATION_CIRCULARBUFFER_H_ */

/*
 * Protocol.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#ifndef COMMUNICATION_PROTOCOL_H_
#define COMMUNICATION_PROTOCOL_H_

#include <stdio.h>
#include <communication/CircularBufferUart.h>
#include "fsl_debug_console.h"

#define FALSE 0
#define TRUE 1

#define MESSAGE_BUFFER_SIZE 256 ///defines maximum size of useful data
#define START_MESSAGE 0xA0 ///defines start byte
#define MY_ADDRESS 0x01 ///my address on net
#define PROTECT_MODE FALSE /// define that be used ACK
#define MAX_MESSAGE_SIZE MESSAGE_BUFFER_SIZE+6 /// define maximum size of packet

class Protocol {
public:
	/**
	 * Constructor
	 */
	Protocol();

	/**
	 * Destructor
	 */
	virtual ~Protocol();

	/**
	 * send message (packet) to destination
	 * @param destination
	 * @param data is pointer to data for send
	 * @param datalen is length of data to send
	 */
	void sendMessage(uint8_t destination, uint8_t* data, uint8_t datalen);

	/**
	 * provide send comand (only one data bytes) to destination
	 * @param destination
	 * @param comand one bytes, which is comand for some element
	 */
	void sendComand(uint8_t destination, uint8_t comand);

	/**
	 *read packet from serial line
	 *@param data pointer to buffer when received data should by save
	 *@param lendgth pointer to variable where length of received data will returned (if length==0 => no data received)
	 *@return 1 when success receive packet else return 0
	 */
	uint8_t receivedMessage(uint8_t* data, uint8_t* lenght);

	/**
	 * read data frame from received packet
	 *@param data pointer to buffer when received data frame should by save
	 *@param lendgth pointer to variable where length of received data will returned (if length==0 => no data received)
	 *@return 1 when success receive packet else return 0
	 */
	uint8_t receivedMessage(uint8_t* data, uint8_t* source_address,
			uint8_t* lenght);

private:

	/**
	 * wait for success receive one byte (blocking read)
	 * @param pointer where received char will return
	 */
	void get_char_blocking(uint8_t* ch);

	/**
	 * function received ACK
	 * @return 1 when success received ACK, 0 when failure receive ACK
	 */
	uint8_t receiveACK();

	/**
	 * function calculate CRC
	 * @param pointer data data for calculate CRC (data on index 2 skipped)
	 * @param length of data
	 */
	uint8_t getCRC(uint8_t* data, uint8_t len);

};

#endif /* COMMUNICATION_PROTOCOL_H_ */

/*
 * Protocol.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#include <communication/Protocol.h>

Protocol::Protocol() {
}

Protocol::~Protocol() {
}

void Protocol::sendComand(uint8_t destination, uint8_t comand) {
	sendMessage(destination, &comand, 1);
}

void Protocol::sendMessage(uint8_t destination, uint8_t* data,
		uint8_t datalen) {

	uint8_t message[MESSAGE_BUFFER_SIZE + 6];
	message[0] = START_MESSAGE;
	message[1] = destination;
	message[2] = MY_ADDRESS;
	message[3] = datalen;

	for (int i = 0; i < datalen; i++) {
		message[4 + i] = data[i];
	}

	message[datalen + 4] = getCRC(&(message[1]), datalen + 2);
	do {
		for (int i = 0; i < datalen + 5; i++) {
			DbgConsole_Putchar(message[i]);
			//	PRINTF("%d\n\r", message[i]); //kontrolny vypis odriadkovany do putty
		}
	} while (!receiveACK()); // if not received ACK try again send message
}

uint8_t Protocol::receiveACK() {
	if (!PROTECT_MODE)
		return TRUE;
	return FALSE;
}

void Protocol::get_char_blocking(uint8_t* ch) {
	while (!get_char(ch))
		;
}

uint8_t Protocol::receivedMessage(uint8_t* data, uint8_t* length) {
	if (!get_char(data) || ((*data) != START_MESSAGE))
		return 0;

	get_char_blocking(++data); //destination
	get_char_blocking(++data); //source
	get_char_blocking(++data); //length
	*length = *data;
	for (uint8_t i = 0; i < (*length); i++) {
		get_char_blocking(++data);
	}
	return 1;
}

uint8_t Protocol::receivedMessage(uint8_t* data, uint8_t* source_address,
		uint8_t* lenght) {

	uint8_t pomData[MAX_MESSAGE_SIZE];

	if (!receivedMessage(pomData, lenght))
		return 0;

	*source_address = pomData[2];
	uint8_t* pomPointer = pomData + 4;
	for (uint8_t i = 0; i < (*lenght); i++) {
		*(data++) = *(pomPointer++);
	}

	return 1;
}

uint8_t Protocol::getCRC(uint8_t* data, uint8_t len) {

	const uint8_t CRC_TABLE[] = { 0, 94, 188, 226, 97, 63, 221, 131, 194, 156,
			126, 32, 163, 253, 31, 65, 157, 195, 33, 127, 252, 162, 64, 30, 95,
			1, 227, 189, 62, 96, 130, 220, 35, 125, 159, 193, 66, 28, 254, 160,
			225, 191, 93, 3, 128, 222, 60, 98, 190, 224, 2, 92, 223, 129, 99,
			61, 124, 34, 192, 158, 29, 67, 161, 255, 70, 24, 250, 164, 39, 121,
			155, 197, 132, 218, 56, 102, 229, 187, 89, 7, 219, 133, 103, 57,
			186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154, 101, 59, 217,
			135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36, 248, 166,
			68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185, 140,
			210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
			17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236,
			14, 80, 175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12,
			82, 176, 238, 50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18,
			145, 207, 45, 115, 202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180,
			234, 105, 55, 213, 139, 87, 9, 235, 181, 54, 104, 138, 212, 149,
			203, 41, 119, 244, 170, 72, 22, 233, 183, 85, 11, 136, 214, 52, 106,
			43, 117, 151, 201, 74, 20, 246, 168, 116, 42, 200, 150, 21, 75, 169,
			247, 182, 232, 10, 84, 215, 137, 107, 53 };

	uint8_t crc = 0; //CRC_TABLE[data[0]];
	//crc = CRC_TABLE[crc xor data[1]];

	for (int i = 0; i <= len; i++) {
		if (i != 2)
			crc = CRC_TABLE[(crc xor data[i])];
	}

	return crc;
}


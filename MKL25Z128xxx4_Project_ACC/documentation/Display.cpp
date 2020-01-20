/*
 * Display.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#include <elevator/Display.h>

Display::Display() {
	direction = direction_none;
	position = 0xFF;
}

Display::~Display() {
}

uint8_t Display::controlDisplay(uint8_t direction_p, uint8_t position_p) {
	if ((position_p == position) && (direction_p == direction))
		return 0;

	position = position_p;
	direction = direction_p;

	switch (position) {
	case floor_P:
		position_p = 'P';
		break;
	case floor_1:
		position_p = '1';
		break;
	case floor_2:
		position_p = '2';
		break;
	case floor_3:
		position_p = '3';
		break;
	case floor_4:
		position_p = '4';
		break;
	default:
		position_p = 'N';
		break;
	}

	uint8_t data[] = { direction, position_p };
	comunicator.sendMessage(ELEVATOR_DISPLAY, data, 2);
	return 1;
}


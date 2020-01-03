/*
 * Display.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_DISPLAY_H_
#define ELEVATOR_DISPLAY_H_

#include <communication/Comunicator.h>
#include <stdio.h>

#define ELEVATOR_DISPLAY 0x30 ///defines address of INFORMATION DISPLAY

/**
 * enum for floors
 */
enum floor_command {
	floor_P, floor_1, floor_2, floor_3, floor_4
};

/**
 * direction of movement elevator
 */
enum direction_command {
	direction_up = 0x01, //up
	direction_down, //down
	direction_none, //stop
	direction_pauseUp,
	direction_pauseDown
};

/**
 * represent object of information display
 */
class Display {
private:
	Comunicator comunicator; /// use for communicate
	uint8_t position; ///information about actually visible position
	uint8_t direction; ///information about actually visible arrow
public:

	/**
	 * Display constructor
	 * initialize information display
	 */
	Display();

	/**
	 * Display destructor
	 */
	virtual ~Display();

	/**
	 * set display
	 * @param direction_p define which arrow will be set
	 * @param floor char which will be set to display
	 */
	uint8_t controlDisplay(uint8_t direction_p, uint8_t floor);
};

#endif /* ELEVATOR_DISPLAY_H_ */

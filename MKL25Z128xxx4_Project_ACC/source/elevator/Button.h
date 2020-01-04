/*
 * Button.h
 *
 *  Created on: Jan 4, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_BUTTON_H_
#define ELEVATOR_BUTTON_H_

#include <stdio.h>
#include <elevator/Led.h>
#include <elevator/Elevator.h>

#define FALSE 0
#define TRUE 1

/**
 * defines addresses of buttons
 */
#define BUTTON_HALL_4 0xC4
#define BUTTON_HALL_3 0xC3
#define BUTTON_HALL_2 0xC2
#define BUTTON_HALL_1 0xC1
#define BUTTON_HALL_P 0xC0
#define BUTTON_CABIN_4 0xB4
#define BUTTON_CABIN_3 0xB3
#define BUTTON_CABIN_2 0xB2
#define BUTTON_CABIN_1 0xB1
#define BUTTON_CABIN_P 0xB0

/**
 * represent buttons on floors and in elevator
 */
class Button {
private:
	Led led; ///object of led
	Elevator* elevator; ///pointer to object elevator
public:
	/**
	 * Button constructor
	 * @param pElevator pointer to elevator
	 */
	Button(Elevator* pElevator);

	/**
	 * Button destructor
	 */
	virtual ~Button();

	/**
	 * control if some button was been press, if yes do reaction
	 */
	uint8_t controlButtons();
};

#endif /* ELEVATOR_BUTTON_H_ */

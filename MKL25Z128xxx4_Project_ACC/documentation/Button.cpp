/*
 * Button.cpp
 *
 *  Created on: Jan 4, 2020
 *      Author: marek
 */

#include <elevator/Button.h>

Button::Button(Elevator* pElevator) {
	elevator = pElevator;
}

Button::~Button() {
}

uint8_t Button::controlButtons() {
	uint8_t floorButton;
	switch (elevator->getReceivedSource()) {
	case BUTTON_HALL_P:
	case BUTTON_CABIN_P:
		led.set_led_onFloor(floor_P, TRUE);
		floorButton = floor_P;
		break;
	case BUTTON_HALL_1:
	case BUTTON_CABIN_1:
		led.set_led_onFloor(floor_1, TRUE);
		floorButton = floor_1;
		break;
	case BUTTON_HALL_2:
	case BUTTON_CABIN_2:
		led.set_led_onFloor(floor_2, TRUE);
		floorButton = floor_2;
		break;
	case BUTTON_HALL_3:
	case BUTTON_CABIN_3:
		led.set_led_onFloor(floor_3, TRUE);
		floorButton = floor_3;
		break;
	case BUTTON_HALL_4:
	case BUTTON_CABIN_4:
		led.set_led_onFloor(floor_4, TRUE);
		floorButton = floor_4;
		break;
	default:
		return 1;
		break;
	}

	if (elevator->getFloor() == floorButton && !elevator->getInMove()) {
		elevator->getTimer().setChannelTimer(timerChannel_blockDoor,
				BLOCK_DOOR_TIME_MS);
		led.set_led_onFloor(floorButton, FALSE);
		elevator->getCabin().OpenDoor();
	} else {
		elevator->set_floorRequest(floorButton);
	}
	return 0;
}


/*
 * Switch.cpp
 *
 *  Created on: Jan 4, 2020
 *      Author: marek
 */

#include <elevator/Switch.h>

Switch::Switch(Elevator* pElevator) {
	elevator = pElevator;
}

Switch::~Switch() {
}

uint8_t Switch::controlFloors() {

	uint8_t newFloor = 0xFF;
	switch (elevator->getReceivedSource()) {
	case LIMIT_SWITCH_P:
		newFloor = floor_P;
		if (elevator->is_floorRequest(newFloor)) {
			led.set_led_onFloor(floor_P, FALSE);
		}
		break;
	case LIMIT_SWITCH_1:
		newFloor = floor_1;
		if (elevator->is_floorRequest(newFloor)) {
			led.set_led_onFloor(floor_1, FALSE);
		}
		break;
	case LIMIT_SWITCH_2:
		newFloor = floor_2;
		if (elevator->is_floorRequest(newFloor)) {
			led.set_led_onFloor(floor_2, FALSE);
		}
		break;
	case LIMIT_SWITCH_3:
		newFloor = floor_3;
		if (elevator->is_floorRequest(newFloor)) {
			led.set_led_onFloor(floor_3, FALSE);
		}
		break;
	case LIMIT_SWITCH_4:
		newFloor = floor_4;
		if (elevator->is_floorRequest(newFloor)) {
			led.set_led_onFloor(floor_4, FALSE);
		}
		break;
	}

	if (newFloor != 0xFF) {

		if (newFloor != elevator->getFloor()) {
			elevator->setFloor(newFloor);
			elevator->getDisplay().controlDisplay(elevator->getDirection(), elevator->getFloor());
			if (elevator->is_floorRequest (elevator->getFloor())) {
				led.set_led_onFloor(elevator->getFloor(), FALSE);
			}
		}
	}
	return 0;
}


/*
 * Elevator.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#include <elevator/Elevator.h>

Elevator::Elevator() {
	floorRequestsFlags = 0; //clear all requests flags

	watchdog = new Watchdog(&timer, WATCHDOG_PERIODE_TIME_MS,
			timerChannel_watchdog);
	engine = new Engine(&timer);

	inMove = FALSE;

	position = engine->getPosition();
	floor = (uint8_t) round(position);
	direction = direction_none;

	display.controlDisplay(direction, floor);
	lastFloor = 0xFF;

	watchdog->timeOutReset();

	timer.startTimer();
	watchdog->timeOutReset();

	set_led_onFloor(floor_P, FALSE);
	set_led_onFloor(floor_1, FALSE);
	set_led_onFloor(floor_2, FALSE);
	set_led_onFloor(floor_3, FALSE);
	set_led_onFloor(floor_4, FALSE);
}

Elevator::~Elevator() {
	delete watchdog;
}

uint8_t Elevator::controlWatchdog() {
	return watchdog->controlWatchdog();
}

uint8_t Elevator::controlInput() {
	successReceive = comunicator.receivedMessage(inData, &receivedSource,
			&receivedLenght);
	return successReceive;
}

uint8_t Elevator::controlButtons() {
	uint8_t floorButton;
	switch (receivedSource) {
	case BUTTON_HALL_P:
	case BUTTON_CABIN_P:
		set_led_onFloor(floor_P, TRUE);
		floorButton = floor_P;
		break;
	case BUTTON_HALL_1:
	case BUTTON_CABIN_1:
		set_led_onFloor(floor_1, TRUE);
		floorButton = floor_1;
		break;
	case BUTTON_HALL_2:
	case BUTTON_CABIN_2:
		set_led_onFloor(floor_2, TRUE);
		floorButton = floor_2;
		break;
	case BUTTON_HALL_3:
	case BUTTON_CABIN_3:
		set_led_onFloor(floor_3, TRUE);
		floorButton = floor_3;
		break;
	case BUTTON_HALL_4:
	case BUTTON_CABIN_4:
		set_led_onFloor(floor_4, TRUE);
		floorButton = floor_4;
		break;
	default:
		return 1;
		break;
	}

	if (floor == floorButton && !inMove) {
		timer.setChannelTimer(timerChannel_blockDoor, BLOCK_DOOR_TIME_MS);
		set_led_onFloor(floorButton, FALSE);
		cabin.OpenDoor();
	} else {
		set_floorRequest(floorButton);
	}
	return 0;
}

uint8_t Elevator::controlFloors() {

	uint8_t newFloor = 0xFF;
	switch (receivedSource) {
	case LIMIT_SWITCH_P:
		newFloor = floor_P;
		if (is_floorRequest(newFloor)) {
			set_led_onFloor(floor_P, FALSE);
		}
		break;
	case LIMIT_SWITCH_1:
		newFloor = floor_1;
		if (is_floorRequest(newFloor)) {
			set_led_onFloor(floor_1, FALSE);
		}
		break;
	case LIMIT_SWITCH_2:
		newFloor = floor_2;
		if (is_floorRequest(newFloor)) {
			set_led_onFloor(floor_2, FALSE);
		}
		break;
	case LIMIT_SWITCH_3:
		newFloor = floor_3;
		if (is_floorRequest(newFloor)) {
			set_led_onFloor(floor_3, FALSE);
		}
		break;
	case LIMIT_SWITCH_4:
		newFloor = floor_4;
		if (is_floorRequest(newFloor)) {
			set_led_onFloor(floor_4, FALSE);
		}
		break;
	}

	if (newFloor != 0xFF) {

		if (newFloor != floor) {
			floor = newFloor;
			display.controlDisplay(direction, floor);
			if (is_floorRequest(floor)) {
				set_led_onFloor(floor, FALSE);
			}
		}
	}
	return 0;
}

uint8_t Elevator::controlPosition() {
	if (!timer.isTimerExpired(timerChannel_checkPosition))
		return position;
	timer.setChannelTimer(timerChannel_checkPosition, CHECK_POSITION_TIMEOUT_MS);
	position = engine->getPosition();
	display.controlDisplay(direction, (uint8_t) round(position));
	return 0;
}

uint8_t Elevator::controlMove() {
	switch (direction) {
	case direction_down:
	case direction_up:
		if (is_floorRequest(floor) && floor != lastFloor) {
			stopMove();
		}
		break;
	case direction_pauseDown:
	case direction_pauseUp:
	case direction_none:
		startMove();
		break;
	default:
		break;

	}
	return 0;
}

uint8_t Elevator::stopMove() {
	timer.setChannelTimer(timerChannel_blockDoor, BLOCK_DOOR_TIME_MS);
	engine->stop();
	inMove = FALSE;
	clear_floorRequest(floor);
	switch (direction) {
	case direction_down:
		if (is_floorRequestDown()) {
			direction = direction_pauseDown;
		} else {
			direction = direction_none;
		}
		break;
	case direction_up:
		if (is_floorRequestUp()) {
			direction = direction_pauseUp;
		} else {
			direction = direction_none;
		}
		break;
	default:

		break;
	}
	display.controlDisplay(direction, floor);
	return 0;
}

uint8_t Elevator::startMove() {
	if (cabin.get_doorStatus()
			&& timer.getChannelTimer_ms(timerChannel_blockDoor)
					< (3 / 4.0) * BLOCK_DOOR_TIME_MS
			&& !timer.isTimerExpired(timerChannel_blockDoor)) {
		cabin.OpenDoor();
	}

	if (is_floorRequest(floor)) {
		set_led_onFloor(floor, FALSE);
	}

	if (!timer.isTimerExpired(timerChannel_blockDoor))
		return 0;

	if (!is_floorRequest()) {
		cabin.lockDoor();
		return 0;
	}

	if (!cabin.get_doorStatus())
		cabin.lockDoor();
	inMove = TRUE;

	switch (direction) {
	case direction_pauseUp:
		engine->goUp();
		direction = direction_up;
		break;
	case direction_pauseDown:
		engine->goDown();
		direction = direction_down;
		break;
	case direction_none:

		if (getNearestMove() > 0) {
			engine->goUp();
			direction = direction_up;
		} else {
			engine->goDown();
			direction = direction_down;
		}
		break;
	default:
		break;
	}
	lastFloor = floor;
	display.controlDisplay(direction, floor);
	return 0;
}

int Elevator::getNearestMove() {

	if (floor == floor_P)
		return 1;
	if (floor == FLOORS - 1)
		return -1;

	int distanceDown = 1;
	while (floor - distanceDown >= floor_P) {
		if (is_floorRequest(floor - distanceDown))
			break;
		distanceDown++;
		//no any request down retun positive number
		if (floor - distanceDown < 0)
			return 1;
	}

	int distanceUp = 1;
	while (floor + distanceUp < FLOORS) {
		if (is_floorRequest(floor + distanceUp))
			break;
		distanceUp++;
		// not ane rezuest on upper floors ( or I'm on highest floor)
		if (floor + distanceUp >= FLOORS)
			return -1;
	}

	return distanceDown - distanceUp;
}

uint8_t Elevator::is_floorRequestUp() {
	return floorRequestsFlags & (0xFF << floor); //if floor=2 mas is 1111 1000
}

uint8_t Elevator::is_floorRequestDown() {
	return floorRequestsFlags & ~(0xFF << floor); //if floor=2  mask is for 0&1 = ~(1111 1100) = 0000 0011
}

uint8_t Elevator::is_floorRequest() {
	return floorRequestsFlags;
}

uint8_t Elevator::is_floorRequest(uint8_t floor_p) {
	return (floorRequestsFlags & (1 << floor_p));
}

void Elevator::set_floorRequest(uint8_t floor_p) {
	floorRequestsFlags |= (1 << floor_p);
}

void Elevator::clear_floorRequest(uint8_t floor_p) {
	floorRequestsFlags &= ~(1 << floor_p);
}

void Elevator::set_led(uint8_t led_address, uint8_t led_status) {
	comunicator.sendCommand(led_address, led_status);
}

void Elevator::set_led_onFloor(uint8_t floor_p, uint8_t led_status) {
	switch (floor_p) {
	case floor_P:
		set_led(LED_CABIN_P, led_status);
		set_led(LED_HALL_P, led_status);
		break;
	case floor_1:
		set_led(LED_CABIN_1, led_status);
		set_led(LED_HALL_1, led_status);
		break;
	case floor_2:
		set_led(LED_CABIN_2, led_status);
		set_led(LED_HALL_2, led_status);
		break;
	case floor_3:
		set_led(LED_CABIN_3, led_status);
		set_led(LED_HALL_3, led_status);
		break;
	case floor_4:
		set_led(LED_CABIN_4, led_status);
		set_led(LED_HALL_4, led_status);
		break;
	}
}

Timer Elevator::getTimer(){
	return timer;
}

uint8_t Elevator::getSuccessReceive() {
	return successReceive;
}

/*
 * Cabin.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#include <elevator/Cabin.h>

Cabin::Cabin() {
	lockDoor();
}

Cabin::~Cabin() {
}

void Cabin::lockDoor() {
	comunicator.sendCommand(ELEVATOR_CABIN, cabin_lockDoor);
	isLock = TRUE;
}

void Cabin::OpenDoor() {
	comunicator.sendCommand(ELEVATOR_CABIN, cabin_unlockDoor);
	isLock = FALSE;
}

uint8_t Cabin::get_doorStatus() {
	return isLock;
}

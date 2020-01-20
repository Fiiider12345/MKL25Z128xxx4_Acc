/*
 * Cabin.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_CABIN_H_
#define ELEVATOR_CABIN_H_

#include <communication/Comunicator.h>
#include <stdio.h>

#define ELEVATOR_CABIN 0xF0 ///address of cabin

#define BLOCK_DOOR_TIME_MS 5000 ///defines open doors time

#define FALSE 0
#define TRUE 1

/**
 * commands for cabin
 */
enum cabin_command {
	cabin_unlockDoor, /// (0x00) unlock the door
	cabin_lockDoor, /// (0x01) lock the door
	cabin_statusDoor /// (else = 0x02) send cabin door status (0=unlock, 1=lock)
};

/**
 * class to represent cabin of elevator
 */
class Cabin {
private:
	Comunicator comunicator; /// use for communicate
	uint8_t isLock;  /// information if door is lock
public:

	/**
	 * Cabin constructor
	 * create and lock cabin
	 */
	Cabin();

	/**
	 * Cabin destructor
	 */
	virtual ~Cabin();

	/**
	 * function lock door on elevators cabin
	 */
	void lockDoor();

	/**
	 * function open door on elevators cabin
	 */
	void OpenDoor();

	/**
	 * @return 1
	 */
	uint8_t get_doorStatus();
};

#endif /* ELEVATOR_CABIN_H_ */

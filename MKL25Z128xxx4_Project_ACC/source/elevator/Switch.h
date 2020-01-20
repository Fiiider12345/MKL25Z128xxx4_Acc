/*
 * Switch.h
 *
 *  Created on: Jan 4, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_SWITCH_H_
#define ELEVATOR_SWITCH_H_

#include <stdio.h>
#include <elevator/Led.h>
#include <elevator/Elevator.h>

#define FALSE 0
#define TRUE 1

/**
 * defines addresses of LIMIT SWITCHES
 */
#define LIMIT_SWITCH_4 0xE4
#define LIMIT_SWITCH_3 0xE3
#define LIMIT_SWITCH_2 0xE2
#define LIMIT_SWITCH_1 0xE1
#define LIMIT_SWITCH_P 0xE0

/**
 * commands for switch
 */
enum switch_command {
	switch_low = 0x00, /// (0x00) Switch low
	switch_wide, /// (0x01) The cabin is in wide proximity
	switch_narrow /// (0x02) The cabin is in narrow proximity
};

/**
 * represent floors switches
 */
class Switch {
private:
	Led led; ///led object
	Elevator* elevator; ///object of elevator
public:
	/**
	 * Switch constructor
	 */
	Switch(Elevator* pElevator);

	/**
	 * Switch destructor
	 */
	virtual ~Switch();

	/**
	 * control if elevator come to floor, if yes do reaction
	 */
	uint8_t controlFloors();
};

#endif /* ELEVATOR_SWITCH_H_ */

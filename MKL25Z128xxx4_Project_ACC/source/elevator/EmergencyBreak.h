/*
 * EmergencyBreak.h
 *
 *  Created on: Jan 3, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_EMERGENCYBREAK_H_
#define ELEVATOR_EMERGENCYBREAK_H_

#include <stdio.h>
#include <timer/Timer.h>
#include <elevator/Elevator.h>

#define FALSE 0
#define TRUE 1

#define ELEVATOR_EMERGENCY_BREAK 0x0F ///defines address of EMERGENCY BREAK

#define EMERGENCY_BREAK_ACTIVE_TIME_MS 10000 ///defines how long activate emergency break when freefall detect

class EmergencyBreak {
private:
	uint8_t emergencyBreakActiv; /// information that emergency break is active
	Elevator* elevator; ///object of elevator
	Comunicator comunicator; /// use for communicate
public:
	/**
	 * EmergencyBreak constructor, init timer and elevator
	 * @param pElevator instance of object elevator
	 */
	EmergencyBreak(Elevator* pElevator);

	/**
	 * EmergencyBreak destructor
	 */
	virtual ~EmergencyBreak();

	/**
	 * function active emergency break
	 * @param active set to 1 for active, or to 0 for deactivate
	 */
	uint8_t set_emergencyBreak(uint8_t active);

	/**
	 * control emergency break of elevator
	 * @param freeFall set to 1 if freefall occurred clear
	 */
	uint8_t handBreak(uint8_t freeFall);

	/**
	 * emergencyBreakActiv getter
	 * @return emergencyBreakActiv
	 */
	uint8_t getEmergencyBreakActiv();
};

#endif /* ELEVATOR_EMERGENCYBREAK_H_ */

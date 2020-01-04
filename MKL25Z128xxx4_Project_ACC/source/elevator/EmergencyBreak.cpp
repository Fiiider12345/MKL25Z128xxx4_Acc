/*
 * EmergencyBreak.cpp
 *
 *  Created on: Jan 3, 2020
 *      Author: marek
 */

#include <elevator/EmergencyBreak.h>

EmergencyBreak::EmergencyBreak(Elevator* pElevator) {
	elevator = pElevator;
	emergencyBreakActiv = FALSE;
	set_emergencyBreak(FALSE);
}

EmergencyBreak::~EmergencyBreak() {
}

uint8_t EmergencyBreak::handBreak(uint8_t freeFall) {

	if (freeFall) {
		if (emergencyBreakActiv) {
			return TRUE;
		}
		set_emergencyBreak(TRUE);
		elevator->getTimer().setChannelTimer(timerChannel_emergencyBreak,
		EMERGENCY_BREAK_ACTIVE_TIME_MS);
		elevator->stopMove();
		return TRUE;
	} else {
		if (emergencyBreakActiv
				&& elevator->getTimer().isTimerExpired(timerChannel_emergencyBreak)) {
			set_emergencyBreak(FALSE);
			elevator->startMove();
			return FALSE;
		}
	}
	return TRUE;
}

uint8_t EmergencyBreak::set_emergencyBreak(uint8_t active) {
	emergencyBreakActiv = active;
	if (active) {
		comunicator.sendCommand(ELEVATOR_EMERGENCY_BREAK, emb_activateBreak);
	} else {
		comunicator.sendCommand(ELEVATOR_EMERGENCY_BREAK, emb_deactivateBreak);
	}
	return 0;
}

uint8_t EmergencyBreak::getEmergencyBreakActiv() {
	return emergencyBreakActiv;
}


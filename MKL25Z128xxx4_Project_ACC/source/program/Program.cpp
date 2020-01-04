/*
 * Program.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#include <program/Program.h>

MMA8451Q* accel;
uint8_t freeFall;

extern "C" void PORTA_IRQHandler(void) {

	GPIO_ClearPinsInterruptFlags(GPIOA, 0x00008000);
	accel->freeFallDetect();
	freeFall = TRUE;

}

Program::Program() {
	freeFall = 0;
	NVIC_EnableIRQ (PORTA_IRQn);
	accel = new MMA8451Q(0x1D);
	elevator = new Elevator();
	emergencyBreak = new EmergencyBreak(elevator);
}

Program::~Program() {
	delete accel;
}

uint8_t Program::start() {
	running = 1;
	while (running) {
		elevator->controlWatchdog();
		elevator->controlInput();
		controlReceiveComand();
		controlFreeFall();
	}
	//vratenie vytahu na prizemie
	return 0;
}

uint8_t Program::stop() {
	running = 0;
	return 0;
}

uint8_t Program::controlFreeFall() {
	emergencyBreak->handBreak(freeFall);
	if (freeFall)
		freeFall = FALSE;
	return freeFall;
}

uint8_t Program::controlReceiveComand() {
	if (!elevator->getSuccessReceive() || emergencyBreak->getEmergencyBreakActiv())
		return 0;
	elevator->controlButtons();
	elevator->controlFloors();
	elevator->controlMove();
	return 0;
}


/*
 * Program.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#ifndef PROGRAM_PROGRAM_H_
#define PROGRAM_PROGRAM_H_

#include <elevator/Elevator.h>
#include <accelerometer/MMA8451Q.h>
#include <fsl_gpio.h>
#include <elevator/EmergencyBreak.h>
#include <elevator/Button.h>
#include <elevator/Switch.h>

/**
 * represent program of elevator
 */
class Program {
private:
	Elevator* elevator;
	uint8_t running;
	EmergencyBreak *emergencyBreak;
	Button *button;
	Switch *mySwitch;

public:
	/**
	 * Program constructor
	 */
	Program();

	/**
	 * Program destructor
	 */
	virtual ~Program();

	/**
	 * function, where is main elevator program in loop
	 */
	uint8_t start();

	/**
	 * function can stop program
	 */
	uint8_t stop();

private:
	/**
	 * fuction, that provide control the elevator when freeFall occured
	 */
	uint8_t controlFreeFall();

	/**
	 * control main functions of elevator
	 * @return 0
	 */
	uint8_t controlReceiveComand();
};

#endif /* PROGRAM_PROGRAM_H_ */

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

/**
 * represent program of elevator
 */
class Program {
private:
	Elevator elevator;
	uint8_t running;

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
};

#endif /* PROGRAM_PROGRAM_H_ */

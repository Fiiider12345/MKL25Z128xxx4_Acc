/*
 * Elevator.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_ELEVATOR_H_
#define ELEVATOR_ELEVATOR_H_

#include <communication/Comunicator.h>
#include <elevator/Cabin.h>
#include <elevator/Display.h>
#include <timer/Timer.h>
#include <elevator/Engine.h>
#include <elevator/Watchdog.h>
#include <stdio.h>
#include <elevator/Led.h>

/**
 * represent object of elevator
 * provides control elevator
 */
class Elevator {
private:
	Comunicator comunicator; /// use for communicate
	Cabin cabin; /// object of cabin
	Display display; /// object of display
	Timer timer; /// object of timer
	Engine* engine; /// pointer to engine
	Watchdog* watchdog; /// pointer to watchdog
	Led led; /// object of led

	/**
	 * receive packet
	 */
	uint8_t successReceive; /// set if success receive packet else clear
	uint8_t inData[MESSAGE_BUFFER_SIZE]; /// useful data from packet
	uint8_t receivedSource; /// source address
	uint8_t receivedLenght; /// length of received data

	uint8_t floorRequestsFlags; /// flags register save information about floor request
	uint8_t floor; ///actually floor
	uint8_t lastFloor; /// last floor
	double position; /// position of elevator motors
	uint8_t direction; /// actually direction of elevator move
	uint8_t inMove; /// information that elevator is in move
public:

	/**
	 * create object of elevator initialiye them and create object of engine, timer and watchdog
	 */
	Elevator();

	/**
	 * delete object of engine, timer and watchdog
	 */
	virtual ~Elevator();

	/**
	 * control watchdog timer of elevator
	 */
	uint8_t controlWatchdog();

	/**
	 * control that some message was been received
	 */
	uint8_t controlInput();

	/**
	 * get position from engine and set this position to alevator
	 */
	uint8_t controlPosition();

	/**
	 * control move, if is need start move or stop move
	 */
	uint8_t controlMove();

	/**
	 * if is need stop the elevator
	 */
	uint8_t stopMove();

	/**
	 * if is need start move the elevator
	 */
	uint8_t startMove();

	/**
	 * @return 1 when is request on some upper floors, else return 0
	 */
	uint8_t is_floorRequestUp();

	/**
	 * @return 1 when is request on some lower floors, else return 0
	 */
	uint8_t is_floorRequestDown();

	/**
	 * @return 0 when is not any request
	 * @return 1 When is some request
	 */
	uint8_t is_floorRequest();

	/**
	 * @param number of checked floor
	 * @return 0 when is not request
	 * @return 1 When is request
	 */
	uint8_t is_floorRequest(uint8_t floor_p);

	/**
	 * function set floorRequest flag bit
	 */
	void set_floorRequest(uint8_t floor_p);

	/**
	 * function clear floorRequest flag bit
	 */
	void clear_floorRequest(uint8_t floor_p);

	/**
	 * @return >0 when nearest floor request is up
	 * @return <0 wnen nearest floor request is down
	 * @return 0 when distance are same
	 */
	int getNearestMove();

	/**
	 * Timer getter
	 * @return timer
	 */
	Timer getTimer();

	/**
	 * getter
	 * @return succesReceive
	 */
	uint8_t getSuccessReceive();

	/**
	 * getter
	 * @return receivedSource
	 */
	uint8_t getReceivedSource();

	/**
	 * getter
	 * @return floor
	 */
	uint8_t getFloor();

	/**
	 * getter
	 * @return floorRequestFlags
	 */
	uint8_t getFloorRequestsFlags() const {
		return floorRequestsFlags;
	}

	/**
	 * getter
	 * @returninMove
	 */
	uint8_t getInMove();

	/**
	 * getter
	 * @return cabin
	 */
	Cabin getCabin();

	/**
	 * setter
	 * @param floor set
	 */
	void setFloor(uint8_t floor);

	/**
	 * getter
	 * @return display
	 */
	Display getDisplay();

	/**
	 * getter
	 * @returndirection
	 */
	uint8_t getDirection();
};

#endif /* ELEVATOR_ELEVATOR_H_ */

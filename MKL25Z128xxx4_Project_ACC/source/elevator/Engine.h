/*
 * Engine.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_ENGINE_H_
#define ELEVATOR_ENGINE_H_

#include <communication/Comunicator.h>
#include <stdio.h>
#include <timer/Timer.h>

#define ELEVATOR_ENGINE 0xF1 ///defines address of ENGINE

#define CHECK_POSITION_TIMEOUT_MS 50 ///defines time how quickly must engine send response
#define FLOORS 5 ///defines numbers of floor

#define DEFAULT_ENGINE_SPEED 50 ///defines default engine speed
#define MESSAGE_BUFFER_SIZE 256 ///defines maximum size of usefull data

/**
 * commands for engine
 */
enum engine_command {
	engine_stop = 0x01, /// (0x01) stop engine
	engine_movement, /// (0x02) turn into motion engine
	engine_sendMotorEncoderCount /// (0x03) send engine encoder count
};

/**
 * enum for timer channels
 */
enum timerChannel_command {
	timerChannel_blockDoor,
	timerChannel_watchdog,
	timerChannel_checkPosition,
	timerChannel_checkPositionACK,
	timerChannel_emergencyBreak
};

/**
 * represent object of elevators engine
 */
class Engine {
private:
	Comunicator comunicator; /// use for communicate
	Timer* timer; /// pointer to elevator timer
	uint8_t speed; /// motor speed
public:
	/**
	 * Engine constructor
	 */
	Engine();

	/**
	 * constructor initialize engine and set pointer to timer
	 * @param timer pointer to timer
	 */
	Engine(Timer* timer);

	/**
	 * Engine destructor
	 */
	virtual ~Engine();

	/**
	 * stop the elevator
	 */
	void stop();

	/**
	 * start move down
	 */
	void goDown();

	/**
	 * start move up
	 */
	void goUp();

	/**
	 * set speed of elevator
	 * @param newSpeed new speed of elevator
	 */
	void set_speed(uint8_t newSpeed);

	/**
	 * get actually position of elevator
	 * @return actually position of elevator <0 to -1044.47>
	 */
	double getPosition();
};

#endif /* ELEVATOR_ENGINE_H_ */

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

#define FALSE 0
#define TRUE 1

#define MESSAGE_BUFFER_SIZE 256 ///defines maximum size of useful data
#define WATCHDOG_PERIODE_TIME_MS 250 ///defines period for send reset command for elevators  Watchdog
#define EMERGENCY_BREAK_ACTIVE_TIME_MS 10000 ///defines how long activate emergency break when freefall detect
#define BLOCK_DOOR_TIME_MS 5000 ///defines open doors time
#define CHECK_POSITION_TIMEOUT_MS 50 ///defines time how quickly must motor send response
#define FLOORS 5 ///defines numbers of floor
#define ELEVATOR_EMERGENCY_BREAK 0x0F ///defines address of EMERGENCY BREAK

/**
 * defines addresses of LEDS
 */
#define LED_HALL_4 0x14
#define LED_HALL_3 0x13
#define LED_HALL_2 0x12
#define LED_HALL_1 0x11
#define LED_HALL_P 0x10
#define LED_CABIN_4 0x24
#define LED_CABIN_3 0x23
#define LED_CABIN_2 0x22
#define LED_CABIN_1 0x21
#define LED_CABIN_P 0x20

/**
 * defines addresses of buttons
 */
#define BUTTON_HALL_4 0xC4
#define BUTTON_HALL_3 0xC3
#define BUTTON_HALL_2 0xC2
#define BUTTON_HALL_1 0xC1
#define BUTTON_HALL_P 0xC0
#define BUTTON_CABIN_4 0xB4
#define BUTTON_CABIN_3 0xB3
#define BUTTON_CABIN_2 0xB2
#define BUTTON_CABIN_1 0xB1
#define BUTTON_CABIN_P 0xB0

/**
 * defines addresses of LIMIT SWITCHES
 */
#define LIMIT_SWITCH_4 0xE4
#define LIMIT_SWITCH_3 0xE3
#define LIMIT_SWITCH_2 0xE2
#define LIMIT_SWITCH_1 0xE1
#define LIMIT_SWITCH_P 0xE0

/**
 * commands for emergency break
 */
enum emergency_break_command {
	emb_deactivateBreak, /// (0x00) deactivate emergency break
	emb_activateBreak /// (0x01) activate emergency break
};

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
	uint8_t emergencyBreakActiv; /// information that emergency break is active
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
	 * function active emergency break
	 * @param active set to 1 for active, or to 0 for deactivate
	 */
	uint8_t set_emergencyBreak(uint8_t active);

	/**
	 * control watchdog timer of elevator
	 */
	uint8_t controlWatchdog();

	/**
	 * control that some message was been received
	 */
	uint8_t controlInput();

	/**
	 * control main functions of elevator
	 */
	uint8_t controlReceiveComand();

	/**
	 * control emergency break of elevator
	 * @param freeFall set to 1 if freefall occurred clear
	 */
	uint8_t handBreak(uint8_t freeFall);

private:

	/**
	 * control if some button was been press, if yes do reaction
	 */
	uint8_t controlButtons();

	/**
	 * control if elevator come to floor, if yes do reaction
	 */
	uint8_t controlFloors();

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
	 * set led on or off
	 * @param led_address addres of LED
	 * @param led_status set to 1 for on, or to 0 for off
	 */
	void set_led(uint8_t led_address, uint8_t led_status);

	/**
	 * set led on floor also in hall and in cabin
	 * @param floor_p floor where led set
	 * @param led_status set to 1 for on, or to 0 for off
	 */
	void set_led_onFloor(uint8_t floor_p, uint8_t led_status);

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
};

#endif /* ELEVATOR_ELEVATOR_H_ */

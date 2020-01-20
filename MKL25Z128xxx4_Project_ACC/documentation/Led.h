/*
 * Led.h
 *
 *  Created on: Jan 4, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_LED_H_
#define ELEVATOR_LED_H_

#include <stdio.h>
#include <communication/Comunicator.h>
#include <elevator/Display.h>

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
 * represent floor and elevator leds
 */
class Led {
private:
	Comunicator comunicator;
public:
	/**
	 * Led constructor
	 */
	Led();

	/**
	 * Led destructor
	 */
	virtual ~Led();

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
};

#endif /* ELEVATOR_LED_H_ */

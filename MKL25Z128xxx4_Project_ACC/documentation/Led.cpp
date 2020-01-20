/*
 * Led.cpp
 *
 *  Created on: Jan 4, 2020
 *      Author: marek
 */

#include <elevator/Led.h>

Led::Led() {
	set_led_onFloor(floor_P, FALSE);
	set_led_onFloor(floor_1, FALSE);
	set_led_onFloor(floor_2, FALSE);
	set_led_onFloor(floor_3, FALSE);
	set_led_onFloor(floor_4, FALSE);
}

Led::~Led() {
}

void Led::set_led(uint8_t led_address, uint8_t led_status) {
	comunicator.sendCommand(led_address, led_status);
}

void Led::set_led_onFloor(uint8_t floor_p, uint8_t led_status) {
	switch (floor_p) {
	case floor_P:
		set_led(LED_CABIN_P, led_status);
		set_led(LED_HALL_P, led_status);
		break;
	case floor_1:
		set_led(LED_CABIN_1, led_status);
		set_led(LED_HALL_1, led_status);
		break;
	case floor_2:
		set_led(LED_CABIN_2, led_status);
		set_led(LED_HALL_2, led_status);
		break;
	case floor_3:
		set_led(LED_CABIN_3, led_status);
		set_led(LED_HALL_3, led_status);
		break;
	case floor_4:
		set_led(LED_CABIN_4, led_status);
		set_led(LED_HALL_4, led_status);
		break;
	}
}

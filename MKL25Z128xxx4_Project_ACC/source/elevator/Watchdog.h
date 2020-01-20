/*
 * Watchdog.h
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#ifndef ELEVATOR_WATCHDOG_H_
#define ELEVATOR_WATCHDOG_H_

#include <communication/Comunicator.h>
#include <timer/Timer.h>
#include <stdio.h>

#define ELEVATOR_WATCHDOG_TIMER 0xFE ///defines address of WATCHDOG TIMER

#define WATCHDOG_PERIODE_TIME_MS 500 ///defines period for send reset command for elevators  Watchdog

/**
 * commands for watchdog timer
 */
enum watchdogTimer_command {
	wdt_timeOutResetTimer = 0x01, /// (0x01) Reset watchdog timer after "timeing out"(restore watchdog timer to initial state))
	wdt_regularyResetTimer /// (else = 0x02) Regularly resets the watchdog timer to prevent it from elapsing.
};

/**
 * represent object of elevator Watchdog
 */
class Watchdog {
private:
	Comunicator comunicator; /// use for communicate
	Timer* timer; ///pointer to elevators timer
	int periodeTime_ms; ///send reset period
	uint8_t timerChannel; /// channel in timer
	uint8_t isBlock; ///information if Watchdog is block
public:
	/**
	 * Watchdog contructor
	 */
	Watchdog();

	/**
	 * initialize Watchdog
	 * @param timer pointer to elevator timer
	 * @param periodeTime_ms describe how often send reset command to WATCHDOG
	 * @param timerChannel number of channel for timer
	 */
	Watchdog(Timer* timer, int periodeTime_ms, uint8_t timerChannel);

	/**
	 * Watchdog destructor
	 */
	virtual ~Watchdog();

	/**
	 * control functionality of WATCHDOG
	 * @return 0 command for regular Reset was send
	 * @return 1 command for regular Reset was not send
	 */
	uint8_t controlWatchdog();

	/**
	 * send regular reset command
	 */
	void regularyReset();

	/**
	 * send reset command after timeout
	 */
	void timeOutReset();
};

#endif /* ELEVATOR_WATCHDOG_H_ */

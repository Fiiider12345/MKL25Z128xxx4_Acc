/*
 * Watchdog.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#include <elevator/Watchdog.h>

Watchdog::Watchdog() {
}

Watchdog::Watchdog(Timer* timer_p, int periodeTime_ms_p,
		uint8_t timerChannel_p) {
	timer = timer_p;
	periodeTime_ms = periodeTime_ms_p;
	timerChannel = timerChannel_p;
	timer->setChannelTimer(timerChannel, periodeTime_ms);
}

Watchdog::~Watchdog() {
}

uint8_t Watchdog::controlWatchdog() {
	if (timer->isTimerExpired(timerChannel)) {
		regularyReset();
		timer->setChannelTimer(timerChannel, periodeTime_ms);
		return 0;
	}
	return 1;
}

void Watchdog::regularyReset() {
	comunicator.sendCommand(ELEVATOR_WATCHDOG_TIMER, wdt_regularyResetTimer);
}

void Watchdog::timeOutReset() {
	comunicator.sendCommand(ELEVATOR_WATCHDOG_TIMER, wdt_timeOutResetTimer);
	isBlock = FALSE;
}


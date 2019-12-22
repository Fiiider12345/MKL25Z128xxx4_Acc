/*
 * Timer.h
 *
 *  Created on: Dec 22, 2019
 *      Author: marek
 */

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

#include "math.h"
#include <stdint.h>
#include "fsl_pit.h"

#define TIMER_PERIODE_US 75000
#define MAX_CHANNELS 5

class Timer {

public:
	/**
	*  Init PIT and enable interrupt from PIT
	*/
	Timer();
	virtual ~Timer();

	/**
	* this function start count the PIT
	*/
	void startTimer();

	/**
	* function set one of channel on time in parameter
	* @param channel number of timer channel
	* @param time_ms time in ms to expire channel
	*/
	void setChannelTimer(uint8_t channel, int time_ms);

	/**
	* get actual counter of channel
	* @param channel, channel from which counter will return
	* @return counter of channel
	*/
	int getChannelTimer_ms(uint8_t channel);

	/**
	* @param channel
	* @return  1 if timer counter of channel was expired jet else return 0
	*/
	uint8_t isTimerExpired(uint8_t channel);
};

#endif /* TIMER_TIMER_H_ */

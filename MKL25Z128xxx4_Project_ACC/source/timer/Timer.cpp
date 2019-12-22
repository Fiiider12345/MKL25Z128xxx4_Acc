/*
 * Timer.cpp
 *
 *  Created on: Dec 22, 2019
 *      Author: marek
 */

#include <timer/Timer.h>

int channelTimer[MAX_CHANNELS]; ///global timer channel counts

/**
 * pit ISR, when some channel is active, this function decrement count of this channel
 */
extern "C" void PIT_IRQHandler(void)
{
	/* Clear interrupt flag.*/
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

	for(int i=0; i<MAX_CHANNELS; i++) {
		if(channelTimer[i]) channelTimer[i]--;
	}
}

Timer::Timer() {
	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT, &pitConfig);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(TIMER_PERIODE_US, CLOCK_GetFreq(kCLOCK_BusClk)));
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_0);

	for(int i=0; i<MAX_CHANNELS; i++) {
		channelTimer[i] = 0;
	}
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::startTimer(){
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void Timer::setChannelTimer(uint8_t channel, int time_ms) {
	channelTimer[channel] = round( (time_ms*1000) / TIMER_PERIODE_US );
}

int Timer::getChannelTimer_ms(uint8_t channel) {
	return (channelTimer[channel] * TIMER_PERIODE_US) / 1000;
}

uint8_t Timer::isTimerExpired(uint8_t channel){
	if(channelTimer[channel]) return false;
	return true;
}

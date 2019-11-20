/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MKL25Z128xxx4_Project_ACC.cpp
 * @brief   Application entry point.
 */
#include <accelerometer/MMA8451Q.h>
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_pit.h"
#include "fsl_smc.h"

#define PERIODA 100000

volatile bool pitInt_flag = false;
MMA8451Q* accel;

extern "C" void PIT_IRQHandler(void) {
	/* Clear interrupt flag.*/
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	pitInt_flag = true;
}

extern "C" void PORTA_IRQHandler(void) {
	GPIO_ClearPinsInterruptFlags(GPIOA, 0x00008000);

	PRINTF("\n\n\rFREE_FALL_DETECTED!!!\n\n\r");

	accel->freeFallDetect();
}

void irqInit() {
	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT, &pitConfig);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(PERIODA, CLOCK_GetFreq(kCLOCK_BusClk)));
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_0);

	NVIC_EnableIRQ(PORTA_IRQn);
}

/**
 * dočasná funkcia na pauzu
 */
void print(int os){
	if(os>=0) {
		PRINTF("%d\n\r", os);
	} else {
		PRINTF("-%d\n\r",os);
	}
}

void print_naklon (float naklonX, float naklonY, float* low_pass_data) {
	if(low_pass_data[0] >= 0 && low_pass_data[2] >= 0){
		PRINTF("naklon v X= %d ", (int)naklonX);
	}else if(low_pass_data[0] >= 0 && low_pass_data[2] < 0){
		PRINTF("naklon v X= %d ", (180-(int)naklonX));
	}else if(low_pass_data[0] < 0 && low_pass_data[2]>=0){
		PRINTF("naklon v X= -%d ", (int)naklonX);
	}else {
		PRINTF("naklon v X= -%d ", (180+(int)naklonX));
	}

	if(low_pass_data[1] < 0 && low_pass_data[2] >=0){
		PRINTF("naklon v Y= %d\n\r", (int)naklonY);
	}else if(low_pass_data[1] < 0 && low_pass_data[2]<0){
		PRINTF("naklon v Y= %d\n\r", 180+(int)naklonY);
	}else if(low_pass_data[1] >= 0 && low_pass_data[2]>=0){
		PRINTF("naklon v Y= -%d\n\r", (int)naklonY);
	}else {
		PRINTF("naklon v Y= -%d\n\r", 180-(int)naklonY);
	}
}

/*
 * @brief   Application entry point.
 */
int main(void) {

		/* Init board hardware. */
	    BOARD_InitBootPins();
	    BOARD_InitBootClocks();
	    BOARD_InitBootPeripherals();
	  	/* Init FSL debug console. */
	    BOARD_InitDebugConsole();

	    irqInit();

	    MMA8451Q akcelerometer(0x1D);
	    accel = &akcelerometer;

	    float high_pass_data[3]={0,0,0};
	    float down_pass_data[3]={0,0,0};
		float current[3]={0,0,0};
		float previous[3];
		akcelerometer.getAccAllAxis(previous);
		LED_BLUE_INIT(kGPIO_DigitalOutput);

	    /* Enter an infinite loop, just incrementing a counter. */
	    while(1) {

	    	SMC_PreEnterStopModes();

	    	//Enable the wakeup interrupt here.
	    		if (pitInt_flag) {
	    		//LED_BLUE_TOGGLE();
	    		akcelerometer.getAccAllAxis(current);
	    		akcelerometer.high_pass_filter(high_pass_data, current, previous, PERIODA, 1);
	    		akcelerometer.low_pass_filter(down_pass_data, current, PERIODA, 0.5);

	    		PRINTF("\n\rHigh-pass filtering data \n\rX = ");
	    		print((int)(high_pass_data[0]*1000));
	    		PRINTF("Y = ");
	    		print((int)(high_pass_data[1]*1000));
	    		PRINTF("Z = ");
	    		print((int)(high_pass_data[2]*1000));

	    		PRINTF("\n\rLow-pass filtering data \n\rX = ");
	    		print((int)(down_pass_data[0]*1000));
	    		PRINTF("Y = ");
	    		print((int)(down_pass_data[1]*1000));
	    		PRINTF("Z = ");
	    		print((int)(down_pass_data[2]*1000));
	    		PRINTF("\n\r");

	    		print_naklon(akcelerometer.getNaklonX(down_pass_data), akcelerometer.getNaklonY(down_pass_data), down_pass_data);

	    		pitInt_flag= false;
	    	}

	    	PRINTF("\n\r");

	        SMC_SetPowerModeWait(SMC);
	    	SMC_PostExitStopModes();
	    }

	    return 0;
}

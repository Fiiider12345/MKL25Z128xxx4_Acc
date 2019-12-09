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
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_lpsci.h"
#include "communication/CircularBuffer.h"

uint8_t g_tipString[] = "LPSCI functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

volatile bool new_line_flag = false;

lpsci_config_t config;

buffer_t rxBuffer_handler;
buffer_t txBuffer_handler;
uint8_t rxbufferPt[100];
uint8_t txbufferPt[100];

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

	    bufferInit(&rxBuffer_handler, rxbufferPt, sizeof(rxbufferPt));
	    bufferInit(&txBuffer_handler, txbufferPt, sizeof(txbufferPt));

	    //CLOCK_SetLpsci0Clock(0x1U);

	    LPSCI_GetDefaultConfig(&config);
	    config.baudRate_Bps = 57600;
	    config.enableRx = true;
	    config.enableTx = true;

	    /* UART0 init */
	    //LPSCI_Init(UART0,&config,CLOCK_GetFreq(kCLOCK_CoreSysClk));
	    LPSCI_Init(UART0, &config, CLOCK_GetFreq(kCLOCK_PllFllSelClk));

	    LPSCI_DisableInterrupts(UART0, kLPSCI_AllInterruptsEnable);

	    /* Send g_tipString out. */
	    LPSCI_WriteBlocking(UART0, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]));

	    /* Enable RX interrupt. */
	    LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);

	    /* Enable interrupt in NVIC. */
	    EnableIRQ(UART0_IRQn);

	    /* Enter an infinite loop */
	    while(1) {

	    	/* Wait for new line */
	    	while(!new_line_flag){}

	    	/* Clear new line flag */
	    	new_line_flag = false;

	    	/* Copy data from RX to TX ring buffer */
	    	uint16_t count = bufferCapacity(&rxBuffer_handler) - bufferBytesFree(&rxBuffer_handler);
	    	uint8_t buffer[count];
	    	bufferRead(&rxBuffer_handler, buffer, count);
	    	bufferWrite(&txBuffer_handler, buffer, count);

	    	/* Enable TX interrupt. */
	    	LPSCI_EnableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);
	    }

	    return 0 ;
}

extern "C" void DEMO_LPSCI_IRQHandler(void) {

	uint8_t data;
	uint8_t receivedData;

	/* If new data arrived. */
	if ((kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(UART0)) {
		receivedData = LPSCI_ReadByte(UART0);
		bufferWrite(&rxBuffer_handler, &receivedData, 1);
		if (receivedData == '\r' || receivedData == '\n')
			new_line_flag = true;
	}

	/*If there are data to send*/
	if ((kLPSCI_TxDataRegEmptyFlag & LPSCI_GetStatusFlags(UART0))) {
		bufferRead(&txBuffer_handler, &data, 1);
		LPSCI_WriteByte(UART0, data);

	/* Disable TX interrupt If there are NO data to send */
	if (txBuffer_handler.full == BUFF_EMPTY)
		LPSCI_DisableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);

	}
}


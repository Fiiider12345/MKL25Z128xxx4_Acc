/*
 * CircularBufferUart.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

/*
 * this source is from https://github.com/hudikm/TPRIS_CIRCULAR_BUFFER.git
 */

#include <communication/CircularBufferUart.h>

lpsci_config_t user_config;

uint8_t shadowBuffer[_bufferUartSize];
uint8_t shadowOutputBuffer[_bufferUartSize];
buffer_t cBuffer;
buffer_t outBuffer;

uint8_t get_char(uint8_t* ch) {
	return bufferRead(&outBuffer, ch, 1);
}

/**
 * LPSCI IRQ handler
 */
extern "C" void UART0_IRQHandler(void) {
	UART0_Type *base = UART0;
	uint8_t pcBuffer;

	/* Send data register empty and the interrupt is enabled. */
	if ((base->S1 & kLPSCI_TxDataRegEmptyFlag)) {
		int c = bufferRead(&cBuffer, &pcBuffer, 1);
		if (c > 0) {
			UART0->D = pcBuffer;
		} else {
			/* Disable TX register empty interrupt. */
			LPSCI_DisableInterrupts(base, kLPSCI_TxDataRegEmptyInterruptEnable);
		}
		LPSCI_ClearStatusFlags(base, kLPSCI_TxDataRegEmptyFlag);
	}

	/* If RX overrun. */
	if (UART0_S1_OR_MASK & base->S1) {
		while (UART0_S1_RDRF_MASK & base->S1) {
			(void) base->D;
		}

		LPSCI_ClearStatusFlags(base, kLPSCI_RxOverrunFlag);
	}

	/* Receive data register full */
	if ((UART0_S1_RDRF_MASK & base->S1) && (UART0_C2_RIE_MASK & base->C2)) {
		uint8_t rxData;
		rxData = base->D;

		bufferWrite(&outBuffer, &rxData, 1);

	}

}

/// Redefine __sys_write to handle printf output
int __sys_write(int iFileHandle, char *pcBuffer, int iLength) {
	// Write String to BUFFER
	int size = bufferWrite(&cBuffer, (uint8_t *) pcBuffer, iLength);
	LPSCI_EnableInterrupts(UART0,
			kLPSCI_TxDataRegEmptyInterruptEnable
					| kLPSCI_RxOverrunInterruptEnable /* kLPSCI_TransmissionCompleteInterruptEnable | kLPSCI_RxOverrunInterruptEnable*/);
	return iLength - size;
}

void CBufferUart_Init() {
	bufferInit(&cBuffer, shadowBuffer, sizeof(shadowBuffer));
	bufferInit(&outBuffer, shadowOutputBuffer, sizeof(shadowOutputBuffer));

	LPSCI_GetDefaultConfig(&user_config);
	user_config.baudRate_Bps = BAUDRATE;
	user_config.enableTx = true;
	user_config.enableRx = true;

	LPSCI_Init(UART0, &user_config, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
	LPSCI_DisableInterrupts(UART0, kLPSCI_AllInterruptsEnable);

	/* Enable LPSCI RX IRQ if previously enabled. */
	LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);

	/* Enable interrupt in NVIC. */
	EnableIRQ (UART0_IRQn);
}

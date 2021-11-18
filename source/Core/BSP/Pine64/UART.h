/*
 * UART.h
 *
 *  Created on: 26 Jan. 2021
 *      Author: Ralim
 */

#ifndef CORE_BSP_PINE64_UART_H_
#define CORE_BSP_PINE64_UART_H_

#include "BSP.h"
#include <stdio.h>
#include <string.h>

const int uartTxBufferLength = 128;
extern char uartTxBuffer[uartTxBufferLength];
const int uartRxBufferLength = 256;
extern char uartRxBuffer[uartRxBufferLength];

void uartWrite(char* buffer, int32_t length);
int32_t uartReadUntil(char end, char* buffer, int32_t length);
int32_t uartIndexOf(char c);

extern "C" {
void USART1_IRQHandler(void);
}
#endif /* CORE_BSP_PINE64_UART_H_ */

/*
 * Debug.cpp
 *
 *  Created on: 26 Jan. 2021
 *      Author: Ralim
 */
#include "UART.h"
#include "Pins.h"
#include "cmsis_os.h"
extern "C" {
#include "gd32vf103_usart.h"
}
char                    uartTxBuffer[uartTxBufferLength];
char                    uartRxBuffer[uartRxBufferLength];
volatile int32_t        uartTxBufferHead = 0;            // Index of start of data
volatile int32_t        uartTxBufferTail = 0;            // Index of end of data
volatile int32_t        uartRxBufferHead = 0;            // Index of start of data
volatile int32_t        uartRxBufferTail = 0;            // Index of end of data

void uartWrite(char* buffer, int32_t length) {
    while(length > uartTxBufferLength-1){
        // If too much data, break it up in chunks of uartTxBufferLength
        length -= uartTxBufferLength-1;
        uartWrite(buffer, uartTxBufferLength-1);
        buffer = buffer + uartTxBufferLength-1;
    }
    if(length <= 0){
        // Quit if no data / no data left after splitting up in chunks
        return;
    }
    int32_t freeBufferSpace = 0;
    while(true){
        // Wait while there is not enough space in the Tx buffer
        freeBufferSpace = (uartTxBufferHead - uartTxBufferTail - 1);
        if(freeBufferSpace<0){
            freeBufferSpace += uartTxBufferLength;
        }
        if(freeBufferSpace >= length) break;
        // taskYIELD();
    }

    if(uartTxBufferLength - uartTxBufferTail < length){
        int32_t chunksize = uartTxBufferLength - uartTxBufferTail;
        memcpy(uartTxBuffer + uartTxBufferTail, buffer, chunksize);
        buffer += chunksize;
        length -= chunksize;
        uartTxBufferTail = 0;
    }
    memcpy(uartTxBuffer+uartTxBufferTail, buffer, length);
    uartTxBufferTail = (uartTxBufferTail + length) % uartTxBufferLength;
    usart_interrupt_enable(UART_PERIF, USART_INT_TBE);
}

int32_t uartRead(){
  if(uartRxBufferHead == uartRxBufferTail) return -1;
  char c = uartRxBuffer[uartRxBufferHead++];
  if(uartRxBufferHead >= uartRxBufferLength) uartRxBufferHead = 0;
  return c;
}

int32_t uartReadUntil(char end, char* buffer, int32_t length){
  if(length <= 0) return 0;

  int32_t i;
  for(i=0; i<length; i++){
    buffer[i] = uartRxBuffer[uartRxBufferHead++];
    if(uartRxBufferHead >= uartRxBufferLength) uartRxBufferHead = 0;
    // uartRxBufferHead %= uartRxBufferLength;
    if(uartRxBufferHead == uartRxBufferTail){
      break;
    }
    if(buffer[i] == end){
      break;
    }
  }
  return i + 1;
}

int32_t uartIndexOf(char c){
  int32_t bufferIdx = uartRxBufferHead;
  for(int i = 0;i<uartRxBufferLength;i++){
    if(bufferIdx == uartRxBufferTail){
      break;
    }

    if(uartRxBuffer[bufferIdx] == c){
      return i;
    }
    // bufferIdx = (bufferIdx + 1) % uartRxBufferLength;
    bufferIdx++;
    if(bufferIdx >= uartRxBufferLength) bufferIdx = 0;
  }
  return -1;
}

void USART1_IRQHandler(void) {
  if (RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_TBE)) {
    usart_data_transmit(UART_PERIF, uartTxBuffer[uartTxBufferHead++]);
    if(uartTxBufferHead >= uartTxBufferLength) uartTxBufferHead = 0;
    if (uartTxBufferHead == uartTxBufferTail) {
      usart_interrupt_disable(UART_PERIF, USART_INT_TBE);
    }
  }
  if (RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_RBNE)){
    /* Read byte from receive buffer */
    uartRxBuffer[uartRxBufferTail++] = usart_data_receive(UART_PERIF);
    if(uartRxBufferTail >= uartRxBufferLength) uartRxBufferTail = 0;
  }
}

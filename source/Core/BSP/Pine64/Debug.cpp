/*
 * Debug.cpp
 *
 *  Created on: 26 Jan. 2021
 *      Author: Ralim
 */
#include "Debug.h"
#include "Pins.h"
#include "UART.h"
extern "C" {
#include "gd32vf103_usart.h"
}

extern volatile uint8_t pendingPWM;
char        uartOutputBuffer[uartOutputBufferLength];

void log_system_state(int32_t PWMWattsx10) {
    // Want to print a CSV log out the uart
    // Tip_Temp_C,Handle_Temp_C,Output_Power_Wattx10,PWM,Tip_Raw\r\n
    // 3+1+3+1+3+1+3+1+5+2 = 23, so sizing at 32 for now
    // uint32_t       outputLength     = 0;
    // outputLength = snprintf(uartOutputBuffer, uartOutputBufferLength, "%lu,%u,%li,%u,%lu\r\n", //
    //                         TipThermoModel::getTipInC(false),                                  // Tip temp in C
    //                         getHandleTemperature(),                                            // Handle temp in C X10
    //                         PWMWattsx10,                                                       // Output Wattage
    //                         pendingPWM,                                                        // PWM
    //                         TipThermoModel::convertTipRawADCTouV(getTipRawTemp(0), true)       // Tip temp in uV
    // );
    // uartOutputBuffer[0] = 'a';
    // outputLength = 1;
    // uartWrite(uartOutputBuffer, outputLength);

    if(uartIndexOf('\n') >= 0){
        // for(int i=0; i<10; i++){
        //     uartOutputBuffer[i] = '0' + i;
        // }
        int32_t len = uartReadUntil('\n', uartOutputBuffer, uartOutputBufferLength);
        // uartOutputBuffer[0]++;
        uartWrite(uartOutputBuffer, len);
    }
}

// void USART1_IRQHandler(void) {
//   // usart_data_transmit(UART_PERIF, 'i');
//   if((RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_ERR_NERR)) || 
//      (RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_ERR_FERR)) ||
//      (RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_ERR_ORERR))){
//        usart_data_transmit(UART_PERIF, 'e');
//      }
//   if(RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_RBNE)){
//     char b = usart_data_receive(UART_PERIF);
//     // usart_flag_clear(UART_PERIF, USART_FLAG_RBNE);
//     // usart_interrupt_flag_clear(UART_PERIF, USART_INT_FLAG_RBNE);
//     usart_data_transmit(UART_PERIF, b);
//   }
//   // if (RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_TBE)) {
//   //   /* write one byte to the transmit data register */
//   //   usart_data_transmit(UART_PERIF, uartOutputBuffer[currentOutputPos++]);
//   //   if (currentOutputPos >= outputLength) {
//   //     currentOutputPos = 0xFF; // Mark done
//   //     usart_interrupt_disable(UART_PERIF, USART_INT_TBE);
//   //   }
//   // }
//   // if (RESET != usart_interrupt_flag_get(UART_PERIF, USART_INT_FLAG_RBNE)){
//   //   /* Read byte from receive buffer */
//   //   char b = usart_data_receive(UART_PERIF);
//   //   usart_data_transmit(UART_PERIF, b);
//   // }
// }

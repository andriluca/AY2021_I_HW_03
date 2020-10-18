/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#include "interrupt.h"
#include "UART.h"
#include "T_LED.h"


extern uint8_t isByteReceived;
extern uint8_t isTimeout;

/* ================================== ISR ================================== */

// USED IN ORDER TO SET THE FLAG WHEN DATA ARE RECEIVED
CY_ISR_PROTO(SERIAL_ISR){
    if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY) isByteReceived=1;
}

// USED TO SET FLAG WHEN TIMEOUT IS OCCURRING
CY_ISR_PROTO(Ti_LED_ISR){
    T_LED_ReadStatusRegister();
    isTimeout=1;
}
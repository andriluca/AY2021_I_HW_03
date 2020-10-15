/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "interrupt.h"

CY_ISR_PROTO(SERIAL_ISR){
    // settare flag
    if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
        isByteReceived=1;
    }
}

CY_ISR_PROTO(Ti_LED_ISR){
    T_LED_ReadStatusRegister();
    isTimeout=1;
}

//CY_ISR_PROTO(Ti_ISR){
//    Timer_ReadStatusRegister();
//    isTimeout=1;
//}

/* [] END OF FILE */

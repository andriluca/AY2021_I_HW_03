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
#ifndef __INTERRUPT_H_
    #define __INTERRUPT_H_
    #include "cytypes.h"
    #include "UART.h"
    #include "T_LED.h"
    uint8_t isByteReceived, countByte;
    uint8_t isTimeout;
    CY_ISR_PROTO(SERIAL_ISR);
    CY_ISR_PROTO(Ti_LED_ISR);
    //CY_ISR_PROTO(Ti_ISR); 
#endif


/* [] END OF FILE */

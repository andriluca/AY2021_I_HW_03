/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#ifndef __INTERRUPT_H
    #define __INTERRUPT_H
    #include "cytypes.h"
    
    CY_ISR_PROTO(SERIAL_ISR);   // HANDLING UART COM
    CY_ISR_PROTO(Ti_LED_ISR);   // HANDLING TIMING
#endif

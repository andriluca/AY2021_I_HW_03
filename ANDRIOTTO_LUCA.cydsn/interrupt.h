/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#ifndef __INTERRUPT_H
    #define __INTERRUPT_H
    #define MAXBYTES 5          // MAXIMAL LENGTH OF THE PACKET
    #include "cytypes.h"
    #include "UART.h"
    #include "T_LED.h"
    #include "EEPROM.h"
    
    CY_ISR_PROTO(SERIAL_ISR);   // ISR DECLARATIONS
    CY_ISR_PROTO(Ti_LED_ISR);
#endif

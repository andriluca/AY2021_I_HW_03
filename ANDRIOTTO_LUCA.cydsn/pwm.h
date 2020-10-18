/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#ifndef __PWM_H
    #define __PWM_H
    
    #include "cytypes.h"
    
 // SOME PROCEDURES TO HELP HANDLING THE PWMS   
    void RGB_PWM_Start();
    void RGB_PWM_WriteCMP(uint8_t r, uint8_t g, uint8_t b);
    
#endif

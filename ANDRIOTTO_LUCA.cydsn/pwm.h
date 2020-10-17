/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#ifndef __PWM_H
    #define __PWM_H
    
    #include "cytypes.h"
    // DATA STRUCTURE: PWM COMPARE VALUES
    typedef struct{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } Color;
    
 // SOME USEFUL PROCEDURES TO HELP HANDLING THE PWMS   
    void RGB_PWM_Start();
    void RGB_PWM_WriteCMP();
    
#endif

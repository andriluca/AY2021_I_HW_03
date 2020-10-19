/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#include "pwm.h"
#include "PWM_RG.h"
#include "PWM_B.h"

// INITIALIZATION OF PWMS
void RGB_PWM_Start(){
    PWM_RG_Start();
    PWM_B_Start();
}

// SETTING THE VALUE
void RGB_PWM_WriteCMP(uint8_t r, uint8_t g, uint8_t b){
    PWM_RG_WriteCompare1(r);
    PWM_RG_WriteCompare2(g);
    PWM_B_WriteCompare(b);
}

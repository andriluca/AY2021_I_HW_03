/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#include "pwm.h"
#include "PWM_RG.h"
#include "PWM_B.h"

Color color;

// INITIALIZATION OF THE PWMS
void RGB_PWM_Start(){
    PWM_RG_Start();
    PWM_B_Start();
}

// SETTING THE VALUE
void RGB_PWM_WriteCMP(){
    PWM_RG_WriteCompare1(color.red);
    PWM_RG_WriteCompare2(color.green);
    PWM_B_WriteCompare(color.blue);
}

/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#include "project.h"
#include "interrupt.h"
#include "states.h"

int main(void){
        
    doInit();                   // INITIALIZING PERIPHERALS AND VARIABLES
    for(;;) doStateMachine();   // EXECUTING THE STATE MACHINE
}

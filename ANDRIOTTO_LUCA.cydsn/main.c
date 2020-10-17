/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#include "project.h"
#include "interrupt.h"
#include "states.h"

int main(void){
    CyGlobalIntEnable; 
    // INITIALIZATION OF PERIPHERALS AND VARIABLES
    doInit();
    
    for(;;){
        // APP/MICROCONTROLLER HANDSHAKE
        if(onHandshake())
           doHandshake();
        
        // PRE-IDLE -> PREPARING VARIABLES FOR IDLE
        if(onPreIdle())
           doPreIdle();        
        
        // HANDLING INCOMING BYTES
        if(onReceivingFirst())
            doReceivingFirst(); 
        if(onReceivingComponents())
            doReceivingComponents(); 
        if(onReceivingFifth())
            doReceivingFifth(); 
    }
}

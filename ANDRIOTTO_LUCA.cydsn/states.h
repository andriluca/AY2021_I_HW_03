/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#ifndef __STATES_H
    #define __STATES_H
    #include "cytypes.h"

    void doInit();

    // STATES
    _Bool onHandshake();
    _Bool onPreIdle();
    _Bool onReceivingFirst();
    _Bool onReceivingComponents();
    _Bool onReceivingFifth();

    // PROCEDURES
    void doHandshake();
    void doPreIdle();
    void doReceivingFirst();
    void doReceivingComponents();
    void doReceivingFifth();
    
#endif

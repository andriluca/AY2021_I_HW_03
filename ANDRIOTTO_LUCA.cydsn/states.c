/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#include "interrupt.h"
#include "project.h"
#include "states.h"
#include "pwm.h"

uint8_t isByteReceived;                 // FLAG: TRUE WHEN DATA ARE RECEIVED
uint8_t countByte;                      // COUNTER: NUMBER OF RECEIVED BYTES
uint8_t isTimeout;                      // FLAG: TRUE IF TIMEOUT HAS OCCURRED
uint8_t isConnected;                    // FLAG: TRUE WHEN HANDSHAKE HAS OCCURRED
uint8_t isTransmissionSuccessful;       // FLAG: TRUE WHEN BYTE SEQUENCE HAS BEEN TRANSMITTED CORRECTLY
uint8_t isSequenceReceived;             // ISSEQUENCERECEIVED: 0 -> SEQUENCE HAS NOT BEGUN 
                                        //                     1 -> SEQUENCE HAS BEGUN 
                                        //                     2 -> SEQUENCE HAS ENDED

static char message[21]="RGB LED Program $$$\r\n";      // HANDSHAKE MESSAGE

extern uint8_t rgb[3];                  // COLLECTED STRUCTURED DATA (PWM COMPARES)

#define STARTCOUNTER 0xFF               // STARTING POINT OF THE TIMER

void doTimerReset();

/* ================================= INITIALIZATION ================================= */

void doInit(){
    //Timer
    T_LED_ISR_StartEx(Ti_LED_ISR);
    T_LED_Start();
    //T_LED_WritePeriod(STARTCOUNTER);

    // UART
    UART_ISR_StartEx(SERIAL_ISR);
    UART_Start();
    
    // PWMS and LED Status
    RGB_PWM_Start();
    RGB_PWM_WriteCMP(0,0,0);
    
    // Variables
    isConnected = 0;
    isTransmissionSuccessful=0;
    isByteReceived=0;
    countByte=0;
    isTimeout=0;
    isSequenceReceived = 0;
}
/* =============================== END INITIALIZATION =============================== */

/* ===================================== STATES ===================================== */

// _S0_: CHECKING IF COMPUTER PROGRAM IS RUNNING
_Bool onHandshake(){                                                            // 1. if
    return isByteReceived && !isConnected && UART_ReadRxData()=='v';            // 'v' as input
}

// _S1.1_: PREPARING FOR IDLE STATE
_Bool onPreIdle(){                                                              // 2. if
    return (isByteReceived && !isConnected && UART_ReadRxData() == 'v')         // 'v' as input
        || (isConnected && isTimeout && countByte < MAXBYTES)                   // or timeout reached
        || isTransmissionSuccessful;                                            // or packet transmitted
}

// _S1.2_: IDLE --> DOING NOTHING

// _S2_: RECEIVING SEQUENCE STARTING BYTE
_Bool onReceivingFirst(){                                                       // 3. if
    return isByteReceived                                                       // 0xA0 as input
        && isConnected
        && isSequenceReceived == 0
        && UART_ReadRxData() == 0xA0;
}

// _S3, S4, S5_: RECEIVING RGB VALUES
_Bool onReceivingComponents(){
    return isByteReceived                                                       // 4. if
        && isConnected                                                          // new byte in input
        && isSequenceReceived == 1
        && !isTimeout 
        && countByte < 3;
}

// _S6_: RECEIVING SEQUENCE ENDING BYTE
_Bool onReceivingFifth(){
    return isByteReceived                                                       // 5. if
        && isConnected                                                          // 0xC0 as input
        && countByte == 3 
        && isSequenceReceived == 2
        && UART_ReadRxData() == 0xC0;
}
/* =================================== END STATES =================================== */

/* =============================== STATES' PROCEDURES =============================== */

void doHandshake(){                             // 1 --> Sending the keyword
    UART_ClearRxBuffer();
    UART_PutString(message);
}

void doPreIdle(){                               // 2 --> Preparing for Idle
    countByte = MAXBYTES;
    isSequenceReceived=0;
    isConnected = 1;
    isTransmissionSuccessful=0;
}


void doReceivingFirst(){                        // 3 --> Receiving starting byte
    doTimerReset();
    isSequenceReceived=1;
    countByte=0;
}

void doReceivingComponents(){                   // 4 --> Receiving 3 bytes as a sequence
    rgb[countByte]=UART_ReadRxData();           // iteratively populating the rgb array
    if(countByte==2) isSequenceReceived=2;      // when string is complete isSequenceReceived is set to 2
    doTimerReset();
    countByte++;
}

void doReceivingFifth(){                        // 5 --> Receiving last byte
    doTimerReset();
    isTransmissionSuccessful=1;
    RGB_PWM_WriteCMP(rgb[0],rgb[1],rgb[2]);
}
/* ============================= END STATES' PROCEDURES ============================= */

/* ====================================== MISC ====================================== */

void doTimerReset(){                            // Resetting timer and clearing read buffer
    UART_ClearRxBuffer();
    isTimeout=0;
    isByteReceived=0;
    T_LED_WriteCounter(STARTCOUNTER);
}

/* ==================================== END MISC ==================================== */

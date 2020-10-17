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

extern Color color;                     // COLLECTED STRUCTURED DATA (PWM COMPARES)

#define STARTCOUNTER 0xFF               // STARTING POINT (AND PERIOD) OF THE TIMER

void doTimerReset();

/* ================================= INITIALIZATION ================================= */

void doInit(){
    //Timer
    T_LED_ISR_StartEx(Ti_LED_ISR);
    T_LED_Start();
    T_LED_WritePeriod(STARTCOUNTER);

    // UART
    UART_ISR_StartEx(SERIAL_ISR);
    UART_Start();
    
    // PWMS and LED Status
    RGB_PWM_Start();
    color.red = 0;
    color.blue = 0;
    color.green = 0;
    RGB_PWM_WriteCMP();
    
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
_Bool onHandshake(){
    return isByteReceived && !isConnected && UART_ReadRxData()=='v';
}

// PREPARING FOR IDLE STATE
_Bool onPreIdle(){
    return (isByteReceived && !isConnected && UART_ReadRxData() == 'v')
        || (isConnected && isTimeout && countByte < MAXBYTES) 
        || isTransmissionSuccessful;
}

// _S1_: IDLE --> DOING NOTHING

// _S2_: RECEIVING SEQUENCE STARTING BYTE
_Bool onReceivingFirst(){
    return isByteReceived
        && isConnected 
        && isSequenceReceived == 0
        && UART_ReadRxData() == 0xA0;
}

// _S3, S4, S6_: RECEIVING RGB VALUES
_Bool onReceivingComponents(){
    return isByteReceived
        && isConnected 
        && isSequenceReceived == 1
        && !isTimeout 
        && countByte < 3;
}

// _S7_: RECEIVING SEQUENCE ENDING BYTE
_Bool onReceivingFifth(){
    return isByteReceived
        && isConnected 
        && countByte == 3 
        && isSequenceReceived == 2
        && UART_ReadRxData() == 0xC0;
}
/* =================================== END STATES =================================== */

/* =============================== STATES' PROCEDURES =============================== */

void doHandshake(){
    UART_ClearRxBuffer();
    UART_PutString(message);
}

void doPreIdle(){
    doTimerReset();
    countByte= MAXBYTES;
    isSequenceReceived=0;
    isConnected = 1;
    isTransmissionSuccessful=0;
}


void doReceivingFirst(){
    doTimerReset();
    isSequenceReceived=1;
    countByte=0;
}

void doReceivingComponents(){
    switch(countByte){
        case 0: color.red=UART_ReadRxData();
                break;
        case 1: color.green=UART_ReadRxData();
                break;
        case 2: color.blue=UART_ReadRxData();
                isSequenceReceived=2;
                break;
    }
    doTimerReset();
    countByte++;
}

void doReceivingFifth(){
    doTimerReset();
    isTransmissionSuccessful=1;
    RGB_PWM_WriteCMP();
}
/* ============================= END STATES' PROCEDURES ============================= */

/* ====================================== MISC ====================================== */

void doTimerReset(){
    UART_ClearRxBuffer();
    isTimeout=0;
    isByteReceived=0;
    T_LED_WriteCounter(STARTCOUNTER);
}

/* ==================================== END MISC ==================================== */

/* [] END OF FILE */

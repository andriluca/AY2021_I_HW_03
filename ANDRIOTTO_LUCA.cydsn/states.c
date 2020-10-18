/* ========================================
 * Assignment #3
 * Luca Andriotto
 * ========================================
*/

#include "interrupt.h"
#include "project.h"
#include "states.h"
#include "pwm.h"

#define STARTCOUNTER 0xFF           // STARTING POINT OF THE TIMER
#define TRIGGERBYTES 5              // THIS VALUE CODES THE NON STARTED SEQUENCE CONDITION

uint8_t isByteReceived;             // FLAG: TRUE WHEN NEW DATA ARE RECEIVED
uint8_t isTimeout;                  // FLAG: TRUE IF TIMEOUT HAS OCCURRED
uint8_t isConnected;                // FLAG: TRUE WHEN HANDSHAKE HAS OCCURRED
uint8_t isTransmissionSuccessful;   // FLAG: TRUE WHEN CORRECT TRANSMISSION HAS OCCURRED

uint8_t countByte;                  // COUNTER: NUMBER OF RECEIVED BYTES (ITERATIONS).
                                    // TRIGGERBYTES --> TRANSMISSION YET TO START
                                    // 0..2 --> POPULATE RGB
                                    // 3    --> END BYTE TRANSMISSION

static char message[21]="RGB LED Program $$$\r\n";      // HANDSHAKE MESSAGE

uint8_t rgb[3];                     // COLLECTED STRUCTURED DATA (PWM COMPARES)

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

void doReceivingReset();

/* ================================= INITIALIZATION ================================= */
void doInit(){

    // ENABLING GLOBAL ISR
    CyGlobalIntEnable; 

    //Timer
    T_LED_ISR_StartEx(Ti_LED_ISR);
    T_LED_Start();

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
    isTimeout=0;
}
/* =============================== END INITIALIZATION =============================== */

/* ================================== STATE MACHINE ================================= */
void doStateMachine(){
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

/* =============================== END STATE MACHINE ================================ */

/* ===================================== STATES ===================================== */

// _S0_: CHECKING IF COMPUTER PROGRAM IS RUNNING
_Bool onHandshake(){                                                            // 1. if
    return isByteReceived && !isConnected && UART_ReadRxData()=='v';            // 'v' as input
}

// _S1.1_: PREPARING FOR IDLE STATE
_Bool onPreIdle(){                                                              // 2. if
    return (isByteReceived && !isConnected && UART_ReadRxData() == 'v')         // 'v' as input
        || (isConnected && isTimeout && countByte < 3)                          // or timeout reached
        || isTransmissionSuccessful;                                            // or packet transmitted
}

// _S1.2_: IDLE --> DOING NOTHING

// _S2_: RECEIVING SEQUENCE STARTING BYTE
_Bool onReceivingFirst(){
    return isByteReceived
        && isConnected
        && countByte == TRIGGERBYTES
        && UART_ReadRxData() == 0xA0;
}

// _S3, S4, S5_: RECEIVING RGB VALUES
_Bool onReceivingComponents(){
    return isByteReceived
        && isConnected
        && countByte < 3
        && !isTimeout;
}

// _S6_: RECEIVING SEQUENCE ENDING BYTE
_Bool onReceivingFifth(){
    return isByteReceived
        && isConnected
        && countByte == 3
        && UART_ReadRxData() == 0xC0;
}

/* =================================== END STATES =================================== */

/* =============================== STATES' PROCEDURES =============================== */

void doHandshake(){                 // 1 --> Sending the keyword
    UART_ClearRxBuffer();
    UART_PutString(message);
}

void doPreIdle(){                   // 2 --> Preparing for Idle
    countByte = TRIGGERBYTES;
    isConnected = 1;
    isTransmissionSuccessful=0;
}

void doReceivingFirst(){            // 3 --> Receiving starting byte
    doReceivingReset();
    countByte=0;
}

void doReceivingComponents(){       // 4 --> Receiving 3 bytes as a sequence
    rgb[countByte]=UART_ReadRxData();
    doReceivingReset();
    countByte++;
}

void doReceivingFifth(){            // 5 --> Receiving last byte
    doReceivingReset();
    isTransmissionSuccessful=1;
    RGB_PWM_WriteCMP(rgb[0],rgb[1],rgb[2]);
}

/* ============================= END STATES' PROCEDURES ============================= */

/* ====================================== MISC ====================================== */

void doReceivingReset(){            // Resetting timer and clearing read buffer
    UART_ClearRxBuffer();
    isTimeout=0;
    isByteReceived=0;
    T_LED_WriteCounter(STARTCOUNTER);
}

/* ==================================== END MISC ==================================== */
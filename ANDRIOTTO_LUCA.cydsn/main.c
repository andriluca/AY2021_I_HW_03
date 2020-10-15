/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

//#include "main.h"
#include "project.h"
#include "interrupt.h"
//#include "serial.h"

static char message[21]="RGB LED Program $$$\r\n";


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    //Timer Init: ATTENZIONE che ho messo periodo = 10s
    T_LED_ISR_StartEx(Ti_LED_ISR);
    
    
    
    // UART Init
    UART_ISR_StartEx(SERIAL_ISR);
    UART_Start();
    
    // EEPROM Init
    EEPROM_Start();
    EEPROM_UpdateTemperature();
    
    // Var Init
    isByteReceived=0;
    countByte=0;
    uint8 isConnected=0;
    uint8 isReady=0;
    uint8 isTimerInited=0;
    
    // STRUCT DEFINITION + Init
    typedef struct{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } Color;
    
    Color color;
    
    
    for(;;)
    {
        // PRE-IDLE -> handshake app/microcontrollore
        if((isByteReceived && !isConnected && !isReady && UART_ReadRxData()=='v')){
            UART_PutString(message);
            isTimeout=0;
            //T_LED_WriteCounter(250);
            isConnected=1;
            isReady=0;
            UART_ClearRxBuffer();   // puliamo la Rx
        }
        // STATO 1 -> ricezione header
        if(isByteReceived && isConnected && !isReady && UART_ReadRxData()==0xA0 && UART_ReadRxData()!=0xC0){
            // INIZIALIZZAZIONE TIMER
            if(!isTimerInited){
                T_LED_Start();
                T_LED_WritePeriod(250);
                isTimerInited=1;
            }
            T_LED_WriteCounter(250);
            //T_LED_WritePeriod(250);
            isTimeout=0;
            isReady=1;
            countByte=0;
            UART_ClearRxBuffer();
        }
        // STATO 2, 3, 4
        if(isByteReceived && isConnected && isReady && UART_ReadRxData() != 0xA0 && UART_ReadRxData() != 0xC0 && countByte < 3){
        //&& !isTimeout){
            // INIZIALIZZAZIONE TIMER
            T_LED_WriteCounter(250);
            //T_LED_WritePeriod(250);
            isTimeout=0;
            UART_ClearRxBuffer();
            switch (countByte){
                case 0: 
                        EEPROM_WriteByte(UART_ReadRxData(),0x00);
                        color.red = UART_ReadRxData();
                        break;
                case 1: 
                        EEPROM_WriteByte(UART_ReadRxData(),0x01);
                        color.green = UART_ReadRxData();
                        break;
                case 2: 
                        EEPROM_WriteByte(UART_ReadRxData(),0x02);
                        color.blue = UART_ReadRxData();
                        break;
            }
            UART_ClearRxBuffer();
            countByte++;
        }
        // STATO 5 -> ricezione tail
        if(isByteReceived && isConnected && isReady && UART_ReadRxData() == 0xC0 && UART_ReadRxData() != 0xA0){    
            isReady=0;
            isConnected=0;
            UART_PutString("Success\r\n");
        }
        if(isTimeout){
            isTimeout=0;
            //T_LED_WriteCounter(250);
            isConnected=1;
            isReady=0;
            UART_ClearRxBuffer();
            UART_PutString("IDLE\r\n");
        }
    }
}

/* [] END OF FILE */

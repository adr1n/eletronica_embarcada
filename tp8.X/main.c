/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1827
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#define BUFFER_MAX 3    ///< Máxima quantidade de bytes a serem recebidos.
#define MOTOR_TEMP 0
#define LUM_SENSOR 1

union{                  ///< União entre os nibles a serem transmitidos.
    uint16_t temp;         ///< Valor de 16 bits que contem os nibles n3, n2, n1 e n0.
    struct{
        uint8_t lsb : 8; ///< Byte n0 
        uint8_t msb : 8; ///< Byte n1  
    };
}TempTx; ///< Variável com os valores Tx.

union{                  ///< União entre os nibles a serem transmitidos.
    uint16_t lum;         ///< Valor de 16 bits que contem os nibles n3, n2, n1 e n0.
    struct{
        uint8_t lsb : 8; ///< Byte n0 
        uint8_t msb : 8; ///< Byte n1  
    };
}LumTx; ///< Variável com os valores Tx.

union{  
    uint16_t gainLum; ///< Valor de 16 bits que contem os nibles n3, n2, n1 e n0.
    struct{
        uint8_t lsb;
        uint8_t msb; ///< Byte n1  
    };
}gainLumRx;

union{    
    uint16_t gainTemp;///< Valor de 16 bits que contem os nibles n3, n2, n1 e n0.
    struct{
        uint8_t lsb;
        uint8_t msb;
    };
}gainTempRx;

union{
    uint16_t duty;
    struct{
        uint8_t lsb; ///< Byte n0 
        uint8_t msb; ///< Byte n1  
    };
}dutyRx;

uint8_t bufferRx[BUFFER_MAX];///< Buffer de Rx
uint8_t countRx = 0;    ///< Contador de bytes recebidos (ponteiro)
bool TxActivate = true;
/*
                         Main application
 */
void analisaRx(){
    if (bufferRx[0]==0x80){
        if (bufferRx[2]){
            TxActivate = true;
        }
        else {
            TxActivate = false;
        }
    }
    else if (bufferRx[0]==0x81){ //PWM Duty
        dutyRx.msb = bufferRx[1]; ///< Byte n0 
        dutyRx.lsb = bufferRx[2];
        EPWM1_LoadDutyValue(dutyRx.duty);
    }
    else if (bufferRx[0]==0x82){  //Ganho de Temperatura
        gainTempRx.msb = bufferRx[1];
        gainTempRx.lsb = bufferRx[2];
    }
    else if (bufferRx[0]==0x83){ //Ganho de Luminosidade
        gainLumRx.msb = bufferRx[1];
        gainLumRx.lsb = bufferRx[2];
    }                   
}

//void enviaTx(){
//    if(TMR1_HasOverflowOccured() && TxActivate){
//        EUSART_Write(0x80);               
//        EUSART_Write(TempTx.msb);            
//        EUSART_Write(TempTx.lsb); 
//        EUSART_Write(LumTx.msb);
//        EUSART_Write(LumTx.lsb); 
//    }
//}
void enviaTx(){
    if(TxActivate){
        EUSART_Write(0x80);               
        EUSART_Write(TempTx.msb);            
        EUSART_Write(TempTx.lsb); 
        EUSART_Write(LumTx.msb);
        EUSART_Write(LumTx.lsb); 
    }
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    
    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    gainTempRx.gainTemp = 2;
    gainLumRx.gainLum = 2;
    TMR1_SetInterruptHandler(enviaTx);
    while (1)
    {
        LumTx.lum = ADC_GetConversion(MOTOR_TEMP);
        LumTx.lum = LumTx.lum*gainLumRx.gainLum;
        TempTx.temp = ADC_GetConversion(LUM_SENSOR);
        TempTx.temp = TempTx.temp*gainTempRx.gainTemp;
//        if(EUSART_is_rx_ready()& countRx != 3){               // Se chega um byte 
//            uint8_t rxChar = EUSART_Read();     // Se guarda em rxChar
//            if(rxChar & 0x80){                 // Se for o inicio do quadro
//                countRx = 0;                    // zera contador
//                bufferRx[countRx] = rxChar; 
//            }
//            else if(countRx < BUFFERMAX-1){            // Se for o final
//                countRx++;
//                bufferRx[countRx] = rxChar;                   // analiza dados recebidos
//            }
//        }
        uint8_t rxChar;
        if(EUSART_is_rx_ready()){
            for(int i = 0; i<3;i++) {
                rxChar = EUSART_Read();
                bufferRx[i] = rxChar; 
            }
            analisaRx();
        }
    }   
}
/**
 End of File
*/
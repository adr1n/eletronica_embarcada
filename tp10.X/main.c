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
#include "main.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>



void matrixClear(){
    uint8_t data[4];                   // Buffer para tx spi
#if flip_matrix                        // No Lab Remoto a imagem aparece invertida na horizontal
    for(uint8_t i=8;i>0;i--){          // Endereça digitos 7..0
#else
    for(uint8_t i=1;i<9;i++){          // Endereça digitos 0..7
#endif
        data[0] = i;                   // Digito i da Matriz Esquerda
        data[1] = 0;    // Valor do digito i da Matriz Esquerda  
        data[2] = i;                   // Digito i da Matriz Direita       
        data[3] = 0;   // Valor do digito i da Matriz Direita
        txSpi(spiCS1, data, 4);        // Tx valores dos dígitos dig para as matrizes
    }
}

void initMatrix(){
    uint8_t data[4];            // Buffer para tx spi
    uint8_t k=0;                // Ponteiro do arranjo da configuração das matrizes
    matrixClear();        // Zera dígitos
    for(uint8_t i=0;i<6;i++){   // Envia os 8 valores de configuração
        for(uint8_t j=0;j<4;j=j+2){
            data[j]= matrix_conf[k];    // Define o endereço dos registradores de configuração
            data[j+1]= matrix_conf[k+1];// Define o valor dos registradores de configuração
        }
        k=k+2;                  // Inc ponteiro da configuração     
        txSpi(spiCS1, data, 4); // Tx configuração para as 2 matrizes
        if(i==4){               // Display-Test
            __delay_ms(800);
        }
    }
}
// 1 <= x <= 16 e 0 <= y <= 7
void positionCalc(){
    static float yi;
    yi = yi + a;
    y = round(yi);
    x = x + incx;
    if(x == 1){
        incx = 1;
    }
    if(x == 16){
        incx = -1;
    }
    if(y <= 0){
        y = 0;
        a = -a;
    }
    if(y >= 7){
        y = 7;
        a = -a;
    }
    if (address_write.v16 == 0x7FFF){
        address_write.v16 = 0;
    }
    Ext_Write(address_write.b0, address_write.b1, x, y);
    address_write.v16++;
}

void positionUpdate(uint8_t cordX, uint8_t cordY){
    uint8_t data[4];                   // Buffer para tx spi
    if (cordX <= 8){
        data[0] = 1;
        data[1] = 0;
        data[2] = cordX;
        data[3] = 0b10000000 >> cordY;
    }
    else if(cordX > 8){
        cordX = cordX - 8;
        data[0] = cordX;
        data[1] = 0b10000000 >> cordY;
        data[2] = 1;
        data[3] = 0;
    }   
    txSpi(spiCS1, data, 4);
}


void txSpi(spiCS_t cs, uint8_t *data, size_t dataSize){
    switch(cs){                      // Ativa CS do dispositivo
        case spiCS1:
            CS1_SetLow();            // Ativa CS1
            break;
        case spiCS2:
            CS2_SetLow();            // Ativa CS2
            break;
    }
    SPI1_ExchangeBlock(data,dataSize);// Tx
    switch(cs){
        case spiCS1:
            CS1_SetHigh();          // Desativa CS1
            break;
        case spiCS2:
            CS2_SetHigh();          // Desativa CS2
            break;
    }
    //__delay_us(1);
}

void rxSpi(spiCS_t cs, void *block, uint8_t blockSize){
    switch(cs){                      // Ativa CS do dispositivo
        case spiCS1:
            CS1_SetLow();            // Ativa CS1
            break;
        case spiCS2:
            CS2_SetLow();            // Ativa CS2
            break;
    }
    SPI1_ReadBlock(block, blockSize);// Tx
    switch(cs){
        case spiCS1:
            CS1_SetHigh();          // Desativa CS1
            break;
        case spiCS2:
            CS2_SetHigh();          // Desativa CS2
            break;
    }
    //__delay_us(1);
}

void analisaRx(){
    if(bufferRx == 0x55){
        random = TMR1;
        x = 1;
        a = -1.5 + 0.1*(random & 31);
        incx = 1;
        T = random & 15;
        y = random & 7;
        if (T == 0){
            T = 12;
        }
        address_read.v16 = 0;
        while(address_read.v16 != address_write.v16){
            Ext_Read(address_read.b0, address_read.b1);
            address_read.v16++;
        }
    }
}

void executaTudo(){
    if (y >= 7 | y <= 0 | x == 16 | x == 1){
        LED_SetHigh();
    }
    else {
        LED_SetLow();
    }
    matrixClear();
    positionUpdate(x,y);
    positionCalc();
}

void Ext_Write(uint8_t addressLow, uint8_t addressHigh, uint8_t x, uint8_t y){
    uint8_t data[5];
    data[0] = EE_WRITE;
    data[1] = addressLow;
    data[2] = addressHigh;
    data[3] = x;
    data[4] = y;
    txSpi(spiCS2, data, 5);
}

void Ext_Read(uint8_t addressLow, uint8_t addressHigh){
    uint8_t data[4];
    data[0] = EE_READ;
    data[1] = addressLow;
    data[2] = addressHigh;
    data[3] = 0;
    data[4] = 0;
    rxSpi(spiCS2, data, 5);
    EUSART_Write(data[3]);
    EUSART_Write(data[4]);
    EUSART_Write(0x0D);
}

void decrementaT(){
    static volatile unsigned int CountCallBack = 0;

    // clear the TMR2 interrupt flag
    PIR1bits.TMR2IF = 0;

    // callback function - called every 4th pass
    if (++CountCallBack >= T)
    {
        // ticker function call
        executaTudo();

        // reset ticker counter
        CountCallBack = 0;
    }
}

/*
                         Main application
 */
void main(void){
    random = TMR1;
    x = 1;
    incx = 1;
    a = -1.5 + 0.1*(random & 31);
    T = random & 15;
    y = random & 7;
    if (T == 0){
            T = 1;
        }
    char rxByte;
    // initialize the device
    
    SYSTEM_Initialize();
    TMR2_SetInterruptHandler(decrementaT);
    CS1_SetHigh();
    CS2_SetHigh();
    
    SPI1_Open(SPI1_DEFAULT);        // Configura MSSP1
    
    initMatrix();                   // Configura matrizes
    __delay_ms(1000);
    
    // Teste de matrizes e tabela look-up

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    while (1){
        if(EUSART_is_rx_ready()){                // Se chega um byte 
            uint8_t rxChar = EUSART_Read();     // Se guarda em rxChar
            bufferRx = rxChar;
            analisaRx();                    // analiza dados recebidos
        }
    }
}
/**
 End of File
*/
    // random de 0 a 31 -1.5 + 0.1*30 aproveitar so os 5 bits menos significativos
    // 
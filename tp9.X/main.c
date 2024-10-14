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

#include <string.h>
#include "mcc_generated_files/mcc.h"

#define FIM_MENSAGEM 0x0D
#define AGENDADO 1
#define ATENDIDO 2   
#define VAZIO 0

#define A 0x41
#define L 0x4C
#define P 0x50
#define R 0x52
#define SPACE 0x20
#define X 0x58

#define NAME_SIZE 21

typedef struct {
    uint8_t estado;
    char name[NAME_SIZE];
} ee_t;

__eeprom ee_t ee[10];
const char msg0[] = "L-Exibe lista, A-Agenda, P-Proximo, R-Apaga lista";
const char msg1[] = "Lista de agendamentos";
const char msg2[] = "Lista de agendamentos vazia";
const char msg3[] = "Proximo:";
const char msg4[] = "Digite o nome:";
const char msg5[] = "Nao foi possivel agendar (nome vazio)";
const char msg6[] = "Todos os agendamentos foram atendidos";
const char msg7[] = "Nao disponemos de mais agendamentos";
const char msg8[] = "Agendamento realizado";

uint8_t countName = 0;
uint8_t countRx;
uint8_t bufferRx;
bool nome_vazio = false;

void enviaMsg(const char *msg){
    uint8_t i = 0;
    while (msg[i] > 0) { // Envia caracteres da msg 
        EUSART_Write(msg[i]); // até encontrar zero (fim do string)
        i++;
    }
    EUSART_Write(FIM_MENSAGEM);
}
void enviaNomes(){
    for(int i = 0; i<=10; i++){
        if(ee[i].estado == 2){
            EUSART_Write(X);
            EUSART_Write(SPACE);
            for(int j = 0; j<20 && ee[i].name[j] != 0;j++){
                EUSART_Write(ee[i].name[j]);        
            }
            EUSART_Write(FIM_MENSAGEM);
        }
        else if(ee[i].estado == 1) {
            EUSART_Write(SPACE);
            EUSART_Write(SPACE);
            for(int j = 0; j<20 && ee[i].name[j] != 0;j++){
                EUSART_Write(ee[i].name[j]);
            }
            EUSART_Write(FIM_MENSAGEM);
        }
    }
}

void recebeGuardaNome(){
    static char nameRx[NAME_SIZE];
    uint8_t countByte = 0;
    countRx = 0;
    enviaMsg(msg4);
    while (countRx < (NAME_SIZE - 1)){
        if (EUSART_is_rx_ready()) { // Se chega um byte 
            uint8_t rxChar = EUSART_Read(); // Se guarda em rxChar
            if (rxChar == FIM_MENSAGEM){ // Se for o final
                break;
            }
            else { // e o buffer não está cheio
                nameRx[countRx] = rxChar; // Guarda valor
                countRx++;
            }
        }
    }
    nameRx[countRx] = 0; // Adiciona o terminador nulo no final do buffer
    if(nameRx[0] == 0){
        nome_vazio = true;
        enviaMsg(msg5);
        ee[countName].estado = VAZIO;
    }
    else {
        nome_vazio = false;
        enviaMsg(msg8);
        while(countByte < 21){
            ee[countName].name[countByte] = nameRx[countByte];
            countByte++;
        }
        ee[countName].estado = AGENDADO;
    }
}

void analisaRx(){
    if(bufferRx == L){
        if(ee[0].estado == VAZIO){
            enviaMsg(msg2);
        }
        else {
        enviaMsg(msg1);
        enviaNomes();
        }
    }
    else if(bufferRx == A){
        if (countName < 10){
            recebeGuardaNome();
            if(!nome_vazio){
                countName++;
            }
        }
        else {
            enviaMsg(msg7);
        }
    }
    if(bufferRx == P){
        int i = 0;
        while(i<=10){
            if(ee[i].estado == AGENDADO){
                enviaMsg(msg3);
                EUSART_Write(SPACE);
                EUSART_Write(SPACE);
                for(int j = 0; j<20 && ee[i].name[j] != 0;j++){
                    EUSART_Write(ee[i].name[j]);
                }
                EUSART_Write(FIM_MENSAGEM);
                ee[i].estado = ATENDIDO;
                break;
            }
            else if(ee[i].estado == VAZIO){
                enviaMsg(msg2);
                break;
            }
            else if(i==10){
                enviaMsg(msg6);
                i++;
            }
            else {
                i++;
            }
        }        
    }
    else if(bufferRx == R){
        for(int i = 0; i<=10; i++){
            ee[i].estado = VAZIO;
        }
        countName = 0;
        enviaMsg(msg2);
    }
    else {
        enviaMsg(msg0);
    }
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        if (EUSART_is_rx_ready()) {
            uint8_t rxChar = EUSART_Read();
            if (rxChar == L || rxChar == A || rxChar == P || rxChar == R) {
                bufferRx = rxChar;                
            } 
            else if (rxChar == FIM_MENSAGEM) {
                analisaRx();
            }
        }
    }
}

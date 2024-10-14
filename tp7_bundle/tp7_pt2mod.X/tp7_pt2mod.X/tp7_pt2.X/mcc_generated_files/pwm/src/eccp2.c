/**
 * PWM2 Generated Driver File.
 * 
 * @file eccp2.c
 * 
 * @ingroup pwm2
 * 
 * @brief This file contains the API implementations for the PWM2 driver.
 *
 * @version PWM2 Driver Version 1.0.0
*/
/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include <xc.h>
#include "../eccp2.h"

#define PWM2_INITIALIZE_DUTY_VALUE 0

void ECCP2_Initialize(void)
{
    // Set the PWM2 to the options selected in the User Interface
    
    // CCPM P2A: active high; P2B: active high; DCB 0; PM single; 
    CCP2CON = 0xC;
    
    // PSSBD low; PSSAC low; CCPAS disabled; CCPASE operating; 
    ECCP2AS = 0x0;
    
    // PDC 0; PRSEN manual_restart; 
    PWM2CON = 0x0;
    
    // STRA P2A_to_CCP2M; STRB P2B_to_port; STRSYNC start_at_begin; 
    PSTR2CON = 0x1;
    
    // CCPRH 0; 
    CCPR2H = 0x0;
    
    // CCPRL 0; 
    CCPR2L = 0x0;
    
    // Selecting Timer 2
    CCPTMRS0bits.C2TSEL = 0x0; 
}

void ECCP2_LoadDutyValue(uint16_t dutyValue)
{
	dutyValue &= 0x03FF;

    // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR2L = ((dutyValue & 0x03FC) >> 2);

    // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP2CON = (uint8_t)((CCP2CON & 0xCF) | ((dutyValue & 0x0003) << 4));
}

/**
 End of File
*/

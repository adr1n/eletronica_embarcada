/**
 * ECCP1 Generated Driver File.
 * 
 * @file eccp1.c
 * 
 * @ingroup capture1
 * 
 * @brief This file contains the API implementation for the ECCP1 driver.
 *
 * @version ECCP1 Driver Version 1.0.0
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
#include "../eccp1.h"

void ECCP1_Initialize(void)
{
    // Set the ECCP1 to the options selected in the User Interface
    
    // CCPM Every 4th rising edge; DCB 0; PM single; 
    CCP1CON = 0x6;
    
    // CCPRH 0; 
    CCPR1H = 0x0;
    
    // CCPRL 0; 
    CCPR1L = 0x0;
    
    // Selecting Timer 1
    CCPTMRS0bits.C1TSEL = 0x0; 

}

bool ECCP1_IsCapturedDataReady(void) 
{
    // Check if data is ready to read from capture module by reading "CCPIF" flag.
    bool status = PIR1bits.CCP1IF;
    if(status)
    {
        PIR1bits.CCP1IF = 0;
    }
    return (status);
}

uint16_t ECCP1_CaptureRead(void)
{
    CCPR1_PERIOD_REG_T module;

    // Copy captured value.
    module.ccpr1l = CCPR1L;
    module.ccpr1h = CCPR1H;
    
    // Return 16bit captured value
    return module.ccpr1_16Bit;
}


/**
 End of File
*/

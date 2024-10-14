/**
 * PWM2 Generated Driver API Header File.
 * 
 * @file eccp2.h
 * 
 * @defgroup pwm2 PWM2
 * 
 * @brief This file contains the API prototypes for the PWM2 module.
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

#ifndef PWM2_H
#define PWM2_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @ingroup pwm2
 * @brief Defines the Custom Name for the \ref ECCP2_Initialize API
 */
#define EPWM2_Initialize ECCP2_Initialize

/**
 * @ingroup pwm2
 * @brief Defines the Custom Name for the \ref ECCP2_LoadDutyValue API
 */
#define EPWM2_LoadDutyValue ECCP2_LoadDutyValue

 /**
 * @ingroup pwm2
 * @brief Initializes the ECCP2 module. This is called only once before calling other ECCP2 APIs.
 * @param None.
 * @return None.
 */
void ECCP2_Initialize(void);

/**
 * @ingroup pwm2
 * @brief Loads the 16-bit duty cycle value.
 * @pre ECCP2_Initialize() is already called.
 * @param dutyValue - 16-bit duty cycle value
 * @return None.
 */
void ECCP2_LoadDutyValue(uint16_t dutyValue);

#endif //PWM2_H
/**
 End of File
*/

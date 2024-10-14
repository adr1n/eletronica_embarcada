/**
 * ECCP1 Generated Driver API Header File.
 * 
 * @file eccp1.h
 * 
 * @defgroup capture1 CAPTURE1
 * 
 * @brief This file contains the API prototypes and other data types for the ECCP1 module.
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

#ifndef ECCP1_H
#define ECCP1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>


/**
 * @ingroup capture1
 * @union CCPR1_PERIOD_REG_T
 * @brief Custom data type to hold the low byte, high byte and 16-bit values of the Period register.
 */
typedef union CCPR1Reg_tag
{
   struct
   {
      uint8_t ccpr1l; /**< CCPR1L low byte.*/
      uint8_t ccpr1h; /**< CCPR1H high byte.*/
   };
   struct
   {
      uint16_t ccpr1_16Bit; /**< CCPR1 16-bit.*/
   };
} CCPR1_PERIOD_REG_T ;

/**
 * @ingroup capture1
 * @brief Initializes the ECCP1 module. This is called only once before calling other ECCP1 APIs.
 * @param None.
 * @return None.
 */
void ECCP1_Initialize(void);

/**
 * @ingroup capture1
 * @brief Determines if the data capture is complete or not.
 * @pre ECCP1_Initialize() is already called.
 * @param None.
 * @retval True - ECCP1 data capture is complete
 * @retval False - ECCP1 data capture is not yet complete
 */
bool ECCP1_IsCapturedDataReady(void);

/**
 * @ingroup capture1
 * @brief Reads the 16-bit capture value.
 * @pre ECCP1_Initialize() and ECCP1_IsCapturedDataReady() are already called.
 * @param None.
 * @return 16-bit captured value
 */
uint16_t ECCP1_CaptureRead(void);


#endif // ECCP1_H
/**
 End of File
*/
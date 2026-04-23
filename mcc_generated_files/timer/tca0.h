/**
 * TCA0 Generated Driver API Header File
 *
 * @file tca0.h
 *
 * @defgroup tca0_normal TCA0 in Normal Mode
 *
 * @brief This file contains the API prototypes for the TCA0 driver in Normal (16-bit) mode.
 *
 * @version TCA0 Driver Version 3.1.0
 *
 * @version Package Version 7.1.0
 */
/*
© [2026] Microchip Technology Inc. and its subsidiaries.

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

#ifndef TCA0_H_INCLUDED
#define TCA0_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include "../system/utils/compiler.h"
#include "./tca0_deprecated.h"

/**
 * @misradeviation{@advisory,2.5}
 * MPLABÂ® Code Configurator (MCC) Melody drivers provide macros that can be added to an application. 
 * It depends on the application whether a macro is used or not. 
 */

/**
 * @ingroup tca0_normal
 * @brief Defines the maximum count of the timer.
 */
#define TCA0_MAX_COUNT (65535U)
/**
 * @ingroup tca0_normal
 * @brief Defines the timer prescaled clock frequency in hertz.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define TCA0_CLOCK_FREQ (4000000UL)
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_MAX_COUNT.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FREQUENCYOUTPUTTIMER_MAX_COUNT TCA0_MAX_COUNT
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CLOCK_FREQ.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FREQUENCYOUTPUTTIMER_CLOCK_FREQ TCA0_CLOCK_FREQ
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Initialize.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Initialize TCA0_Initialize
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Deinitialize.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Deinitialize TCA0_Deinitialize
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Start.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Start TCA0_Start
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Stop.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Stop TCA0_Stop
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CounterSet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CounterSet TCA0_CounterSet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CounterGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CounterGet TCA0_CounterGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_FrequencySet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_FrequencySet TCA0_FrequencySet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_FrequencyGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_FrequencyGet TCA0_FrequencyGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_WO1OffsetSet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_WO1OffsetSet TCA0_WO1OffsetSet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_WO1OffsetGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_WO1OffsetGet TCA0_WO1OffsetGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_WO2OffsetSet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_WO2OffsetSet TCA0_WO2OffsetSet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_WO2OffsetGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_WO2OffsetGet TCA0_WO2OffsetGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_MaxCountGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_MaxCountGet TCA0_MaxCountGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_ModeSet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_ModeSet TCA0_ModeSet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_OverflowStatusClear.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_OverflowStatusClear TCA0_OverflowStatusClear
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_OverflowStatusGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_OverflowStatusGet TCA0_OverflowStatusGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CMP0MatchStatusClear.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CMP0MatchStatusClear TCA0_CMP0MatchStatusClear
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CMP0MatchStatusGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CMP0MatchStatusGet TCA0_CMP0MatchStatusGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CMP1MatchStatusClear.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CMP1MatchStatusClear TCA0_CMP1MatchStatusClear
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CMP1MatchStatusGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CMP1MatchStatusGet TCA0_CMP1MatchStatusGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CMP2MatchStatusClear.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CMP2MatchStatusClear TCA0_CMP2MatchStatusClear
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_CMP2MatchStatusGet.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_CMP2MatchStatusGet TCA0_CMP2MatchStatusGet
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Tasks.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Tasks TCA0_Tasks
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_OverflowCallbackRegister.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_OverflowCallbackRegister TCA0_OverflowCallbackRegister
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Compare0CallbackRegister.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Compare0CallbackRegister TCA0_Compare0CallbackRegister
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Compare1CallbackRegister.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Compare1CallbackRegister TCA0_Compare1CallbackRegister
/**
 * @ingroup tca0_normal
 * @brief Defines the Custom Name for the \ref TCA0_Compare2CallbackRegister.
 */
 /* cppcheck-suppress misra-c2012-2.5 */  
#define FrequencyOutputTimer_Compare2CallbackRegister TCA0_Compare2CallbackRegister

/**
 * @ingroup tca0_normal
 * @typedef TCA0_cb_t
 * @brief Function pointer to the callback function called by TCA while operating in Normal mode.
 *        The default value is set to NULL, which means that no callback function will be used.
 */
typedef void (*TCA0_cb_t)(void);

/**
 * @ingroup tca0_normal
 * @brief Initializes the TCA0 module.
 * @param None.
 * @return None.
 */ 
void TCA0_Initialize(void);

/**
 * @ingroup tca0_normal
 * @brief Deinitializes the TCA0 module.
 * @param None.
 * @return None.
 */
void TCA0_Deinitialize(void);

/**
 * @ingroup tca0_normal
 * @brief Starts the TCA0.
 * @param None.
 * @return None.
 */
void TCA0_Start(void);

/**
 * @ingroup tca0_normal
 * @brief Stops the TCA0.
 * @param None.
 * @return None.
 */
void TCA0_Stop(void);

/**
 * @ingroup tca0_normal
 * @brief Returns the current counter value.
 * @param None.
 * @return Current count value from the CNT register
 */
uint16_t TCA0_CounterGet(void);

/**
 * @ingroup tca0_normal
 * @brief Sets the counter value.
 * @param count - Counter value to be written to the CNT register
 * @return None.
 */
void TCA0_CounterSet(uint16_t count);

/**
 * @ingroup tca0_normal
 * @brief Sets the frequency count value.
 * @param count - Counter value to be written to the CMP0 register
 * @return None.
 */
void TCA0_FrequencySet(uint16_t count);

/**
 * @ingroup tca0_normal
 * @brief Returns the frequency count value.
 * @param None.
 * @return Frequency count value from the CMP0 register
 */
uint16_t TCA0_FrequencyGet(void);

/**
 * @ingroup tca0_normal
 * @brief Sets the WO1 offset count value.
 * @param count - Offset count value to be written to the CMP1 register
 * @return None.
 */
void TCA0_WO1OffsetSet(uint16_t count);

/**
 * @ingroup tca0_normal
 * @brief Returns the WO1 offset count value.
 * @param None.
 * @return Offset count value from the CMP1 register
 */
uint16_t TCA0_WO1OffsetGet(void);

/**
 * @ingroup tca0_normal
 * @brief Sets the WO2 offset count value.
 * @param count - Offset count value to be written to the CMP2 register
 * @return None.
 */
void TCA0_WO2OffsetSet(uint16_t count);

/**
 * @ingroup tca0_normal
 * @brief Returns the WO2 offset count value.
 * @param None.
 * @return Offset count value from the CMP2 register
 */
uint16_t TCA0_WO2OffsetGet(void);

/**
 * @ingroup tca0_normal
 * @brief Returns the maximum count value.
 * @param None.
 * @return Maximum count value
 */
uint16_t TCA0_MaxCountGet(void);

/**
 * @ingroup tca0_normal
 * @brief Sets the Waveform Generation mode.
 * @pre Initialize the TCA0 with TCA0__Initialize() before calling this API.
 * @param TCA_SINGLE_WGMODE_t mode:
 * - @c TCA_SINGLE_WGMODE_NORMAL_gc      @c  -     Normal mode
 * - @c TCA_SINGLE_WGMODE_FRQ_gc         @c  -     Frequency Generation mode
 * - @c TCA_SINGLE_WGMODE_SINGLESLOPE_gc @c  -     Single-Slope PWM
 * - @c TCA_SINGLE_WGMODE_DSTOP_gc       @c  -     Dual-Slope PWM, overflow on TOP
 * - @c TCA_SINGLE_WGMODE_DSBOTH_gc      @c  -     Dual-Slope PWM, overflow on TOP and BOTTOM
 * - @c TCA_SINGLE_WGMODE_DSBOTTOM_gc    @c  -     Dual-Slope PWM, overflow on BOTTOM
 * @return None.
 *
 */
void TCA0_ModeSet(TCA_SINGLE_WGMODE_t mode) __attribute__((deprecated("This function is deprecated.")));;

/**
 * @ingroup tca0_normal
 * @brief Clears the TCA0 Overflow interrupt flag.
 * @param None.
 * @return None.
 */
void TCA0_OverflowStatusClear(void);

/**
 * @ingroup tca0_normal
 * @brief Returns the status of the TCA0 Overflow interrupt flag.
 * @param None.
 * @retval True  - Overflow interrupt flag is set
 * @retval False - Overflow interrupt flag is not set
 */
bool TCA0_OverflowStatusGet(void);

/**
 * @ingroup tca0_normal
 * @brief Clears the TCA0 Compare 0 interrupt flag.
 * @param None.
 * @return None.
 */
void TCA0_CMP0MatchStatusClear(void);

/**
 * @ingroup tca0_normal
 * @brief Returns the status of the TCA0 Compare 0 interrupt flag.
 * @param None.
 * @retval True  - Compare 0 interrupt flag is set
 * @retval False - Compare 0 interrupt flag is not set
 */
bool TCA0_CMP0MatchStatusGet(void);

/**
 * @ingroup tca0_normal
 * @brief Clears the TCA0 Compare 1 interrupt flag.
 * @param None.
 * @return None.
 */
void TCA0_CMP1MatchStatusClear(void);

/**
 * @ingroup tca0_normal
 * @brief Returns the status of the TCA0 Compare 1 interrupt flag.
 * @param None.
 * @retval True  - Compare 1 interrupt flag is set
 * @retval False - Compare 1 interrupt flag is not set
 */
bool TCA0_CMP1MatchStatusGet(void);

/**
 * @ingroup tca0_normal
 * @brief Clears the TCA0 Compare 2 interrupt flag.
 * @param None.
 * @return None.
 */
void TCA0_CMP2MatchStatusClear(void);

/**
 * @ingroup tca0_normal
 * @brief Returns the status of the TCA0 Compare 2 interrupt flag.
 * @param None.
 * @retval True  - Compare 2 interrupt flag is set
 * @retval False - Compare 2 interrupt flag is not set
 */
bool TCA0_CMP2MatchStatusGet(void);

/**
 * @ingroup tca0_normal
 * @brief Registers a callback function for the TCA0 overflow or underflow event.
 * @param CallbackHandler - Address to the custom callback function
 * @return None.
 */ 

void TCA0_OverflowCallbackRegister(TCA0_cb_t CallbackHandler);

/**
 * @ingroup tca0_normal
 * @brief Registers a callback function for the TCA0 Compare 0 match event.
 * @param CallbackHandler - Address to the custom callback function
 * @return None.
 */ 
void TCA0_Compare0CallbackRegister(TCA0_cb_t CallbackHandler);

/**
 * @ingroup tca0_normal
 * @brief Registers a callback function for the TCA0 Compare 1 match event.
 * @param CallbackHandler - Address to the custom callback function
 * @return None.
 */ 
void TCA0_Compare1CallbackRegister(TCA0_cb_t CallbackHandler);

/**
 * @ingroup tca0_normal
 * @brief Registers a callback function for the TCA0 Compare 2 match event.
 * @param CallbackHandler - Address to the custom callback function
 * @return None.
 */ 
void TCA0_Compare2CallbackRegister(TCA0_cb_t CallbackHandler);

/**
 * @ingroup tca0_normal
 * @brief Performs tasks to be executed during the timer interrupt events.
 * @param None.
 * @return None.
 */
void TCA0_Tasks(void);

#endif /* TCA0_H_INCLUDED */
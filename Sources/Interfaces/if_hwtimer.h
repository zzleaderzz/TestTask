/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_hwtimer.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _IF_HWTIMER_H
#define _IF_HWTIMER_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef void(*If_HwTimer_Callback)(uint32_t period);

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_HwTimer_Init(uint32_t period_ms);
void If_HwTimer_DeInit(void);

void If_HwTimer_RegisterCallback(If_HwTimer_Callback callback);

void If_HwTimer_Enable(void);
void If_HwTimer_Disable(void);

#endif // _IF_HWTIMER0_H
/*-- EOF --------------------------------------------------------------------*/

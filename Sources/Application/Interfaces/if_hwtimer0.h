/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_hwtimer0.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _IF_HWTIMER0_H
#define _IF_HWTIMER0_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef void(*If_HwTimer0_Callback)(uint32_t period);

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_HwTimer0_Init(uint32_t period_ms);
void If_HwTimer0_DeInit(void);

void If_HwTimer0_RegisterCallback(If_HwTimer0_Callback callback);

void If_HwTimer0_Enable(void);
void If_HwTimer0_Disable(void);

#endif // _IF_HWTIMER0_H
/*-- EOF --------------------------------------------------------------------*/

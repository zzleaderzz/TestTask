/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_delay.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _IF_DELAY_H
#define _IF_DELAY_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_Delay_us(uint32_t us);
void If_Delay_ms(uint32_t ms);

#endif // _IF_DELAY_H
/*-- EOF --------------------------------------------------------------------*/

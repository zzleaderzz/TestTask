/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_i2s.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _IF_I2S_H
#define _IF_I2S_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef struct
{
	uint16_t ch_left;
	uint16_t ch_right;
} If_I2S_Buffer_item_t;

typedef void(*If_I2S_Callback)(If_I2S_Buffer_item_t *buffer, const uint32_t max_size);

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_I2S_Init(If_I2S_Callback callback);
void If_I2S_DeInit(void);

void If_I2S_Enable(void);
void If_I2S_Disable(void);

void If_I2S_Run(void);

#endif // _IF_I2S_H
/*-- EOF --------------------------------------------------------------------*/

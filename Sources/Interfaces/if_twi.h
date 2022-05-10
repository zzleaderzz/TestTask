/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_twi.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _IF_TWI_H
#define _IF_TWI_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_TWI_Init(void);
void If_TWI_DeInit(void);

void If_TWI_Enable(void);
void If_TWI_Disable(void);

bool If_TWI_Send(uint8_t address, uint8_t *data, uint8_t length);
bool If_TWI_Receive(uint8_t address, uint8_t *data, uint8_t length);

#endif // _IF_TWI_H
/*-- EOF --------------------------------------------------------------------*/

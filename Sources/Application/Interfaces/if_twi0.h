/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_twi0.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: NIK El.
 */

#ifndef _IF_TWI0_H
#define _IF_TWI0_H

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
void If_TWI0_Init(void);
void If_TWI0_DeInit(void);

void If_TWI0_Enable(void);
void If_TWI0_Disable(void);

bool If_TWI0_Send(uint8_t address, uint8_t *data, uint8_t length);
bool If_TWI0_Receive(uint8_t address, uint8_t *data, uint8_t length);

#endif // _IF_TWI0_H
/*-- EOF --------------------------------------------------------------------*/

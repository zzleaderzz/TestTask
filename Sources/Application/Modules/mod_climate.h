/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_climate.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_CLIMATE_H
#define _MOD_CLIMATE_H

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
void Mod_Climate_Init(void);

float Mod_Climate_GetTemperature(void);
float Mod_Climate_GetHumidity(void);

void Mod_Climate_Tick(uint32_t ms);

void Mod_Climate_Run(void);

#endif // _MOD_CLIMATE_H
/*-- EOF --------------------------------------------------------------------*/

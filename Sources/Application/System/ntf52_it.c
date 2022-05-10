/*-- File description -------------------------------------------------------*/
/**
 *   @file:    nrf52_it.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
#include "SysTick.h"
#include "mod_climate.h"
#include "mod_accelerometer.h"

/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void SysTick_Handler(void)
{
	SysTick_IncTick();
	//Mod_Climate_Tick(SYSTICK_FREQ_1KHZ);
	Mod_Accelerometer_Tick(SYSTICK_FREQ_1KHZ);
}

/*-- EOF --------------------------------------------------------------------*/

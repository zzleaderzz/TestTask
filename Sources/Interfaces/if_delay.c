/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_delay.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "if_delay.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "nrf_delay.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_Delay_us(uint32_t us)
{
	nrf_delay_us(us);
}

void If_Delay_ms(uint32_t ms)
{
	nrf_delay_us(ms);
}

/*-- EOF --------------------------------------------------------------------*/

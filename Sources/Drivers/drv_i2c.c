/*-- File description -------------------------------------------------------*/
/**
 *   @file:    aht10.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: NIK El.
 */

#include "drv_i2c.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void DrvI2C_Init(DrvI2c_Config_t *config)
{
	if((config) && (config->Entity))
	{
		if(config->Instance == DrvI2c_0)
		{
			config->Entity->TwiModule_p = NRF_DRV_TWI_INSTANCE_0;
		}
		else if ()
		{
			config->Entity->TwiModule_p = NRF_DRV_TWI_INSTANCE_1;
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_climate.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: NIK El.
 */

#include "mod_climate.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "if_twi0.h"
#include "aht10.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static AHT10_Data_t data;
static AHT10_Entity_t ath10_entity;
static const AHT10_Config_t ath10_cfg = 
{
    .I2C_Functions =
    {
        .Send = If_TWI0_Send,
        .Receive = If_TWI0_Receive
    },
    .Entity = &ath10_entity
};
/*-- Local functions --------------------------------------------------------*/

/*-- Exported functions -----------------------------------------------------*/
void Mod_Climate_Init(void)
{
	If_TWI0_Init();
	If_TWI0_Enable();

	AHT10_Set_WorkMode(&ath10_cfg, AHT10_WorkMode__AUTOMATIC);
	AHT10_Set_Period(&ath10_cfg, 500);
}

float Mod_Climate_GetTemperature(void)
{
	return data.Temperature;
}

float Mod_Climate_GetHumidity(void)
{
	return data.Humidity;
}

void Mod_Climate_Tick(uint32_t ms)
{
	AHT10_Tick(&ath10_cfg, ms);
}

void Mod_Climate_Run(void)
{
	AHT10_Run(&ath10_cfg, &data);
}

/*-- EOF --------------------------------------------------------------------*/

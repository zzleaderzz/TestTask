/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_climate.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_climate.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "if_twi.h"
#include "aht10.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
typedef struct
{
	float Temperature;
	float Humidity;
} ClimateData_t;

/*-- Local function prototypes ----------------------------------------------*/
static void AHT10_DataReceivedCallback(float temperature, float humidity);

/*-- Local variables --------------------------------------------------------*/
static ClimateData_t cl_data = { 0.0f, 0.0f };

static AHT10_Entity_t ath10_entity;
static const AHT10_Config_t ath10_cfg = 
{
    .I2C_Functions =
    {
        .Send = If_TWI_Send,
        .Receive = If_TWI_Receive
    },
	.DataReadyCallback = AHT10_DataReceivedCallback,
    .Entity = &ath10_entity
};
/*-- Local functions --------------------------------------------------------*/
static void AHT10_DataReceivedCallback(float temperature, float humidity)
{
	cl_data.Temperature = temperature;
	cl_data.Humidity = humidity;
}

/*-- Exported functions -----------------------------------------------------*/
void Mod_Climate_Init(void)
{
	If_TWI_Init();
	If_TWI_Enable();

	AHT10_Set_WorkMode(&ath10_cfg, AHT10_WorkMode_AUTOMATIC);
	AHT10_Set_Period(&ath10_cfg, 500);
}

float Mod_Climate_GetTemperature(void)
{
	return cl_data.Temperature;
}

float Mod_Climate_GetHumidity(void)
{
	return cl_data.Humidity;
}

void Mod_Climate_Tick(uint32_t ms)
{
	AHT10_Tick(&ath10_cfg, ms);
}

void Mod_Climate_Run(void)
{
	AHT10_Run(&ath10_cfg);
}

/*-- EOF --------------------------------------------------------------------*/

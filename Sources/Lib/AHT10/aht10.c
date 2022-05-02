/*-- File description -------------------------------------------------------*/
/**
 *   @file:    aht10.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "aht10.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define AHT10_WakeUpTime					25 //ms
#define AHT10_InitTime						25 //ms
#define AHT10_MeasurementTime				80 //ms

/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint8_t cmd_Init[3] = { 0xE1, 0x08, 0x00 };
static uint8_t cmd_Measure[3] = { 0xAC, 0x33, 0x00 };
static uint8_t cmd_Reset[1] = { 0xBA };

/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void AHT10_Tick(const AHT10_Config_t *config, uint32_t ms)
{
	if((config) && (config->Entity) && (ms > 0))
	{
		config->Entity->TimeCounter += ms;
	}
}

void AHT10_Set_WorkMode(const AHT10_Config_t *config, AHT10_WorkMode_t mode)
{
	if((config) && (config->Entity))
	{
		config->Entity->WorkMode = mode;
	}
}

void AHT10_Set_Period(const AHT10_Config_t *config, uint32_t period_ms)
{
	if(period_ms > AHT10_MeasurementTime)
	{
		config->Entity->Period = period_ms;
	}
	else
	{
		config->Entity->Period = AHT10_MeasurementTime;
	}
}

void AHT10_Request(const AHT10_Config_t *config)
{
	if((config) && (config->Entity))
	{
		if(config->Entity->WorkMode == AHT10_WorkMode_MANUAL)
		{
			if(!config->Entity->SendedFlag)
			{
				//Send
				config->I2C_Functions.Send(AHT10_I2C_ADDRESS, (uint8_t *)cmd_Measure, 3);

				config->Entity->TimeCounter = 0;
				config->Entity->SendedFlag = true;
			}
		}
	}
}


void AHT10_Reset(const AHT10_Config_t *config)
{
	if(config && config->Entity)
	{
		config->Entity->SendedFlag = false;
		config->Entity->TimeCounter = 0;
		config->Entity->WorkStep = AHT10_Step_Reset;
	}
}

void AHT10_Run(const AHT10_Config_t *config)
{
	if(config && config->Entity)
	{
		if(config->I2C_Functions.Send && config->I2C_Functions.Receive)
		{
			switch(config->Entity->WorkStep)
			{
				case AHT10_Step_WakeUp:
				{
					if(config->Entity->SendedFlag)
					{
						if(config->Entity->TimeCounter >= AHT10_WakeUpTime)
						{
							config->Entity->SendedFlag = false;

							//Step
							config->Entity->WorkStep = AHT10_Step_Reset;
						}
					}
					else
					{
						config->Entity->TimeCounter = 0;
						config->Entity->SendedFlag = true;
					}
				}
				break;

				case AHT10_Step_Reset:
				{
					if(config->Entity->SendedFlag)
					{
						if(config->Entity->TimeCounter >= AHT10_WakeUpTime)
						{
							config->Entity->SendedFlag = false;

							//Step
							config->Entity->WorkStep = AHT10_Step_Init;
						}
					}
					else
					{
						//Send
						config->I2C_Functions.Send(AHT10_I2C_ADDRESS, (uint8_t *)cmd_Reset, 1);

						config->Entity->TimeCounter = 0;
						config->Entity->SendedFlag = true;
					}
				}
				break;

				case AHT10_Step_Init:
				{
					if(config->Entity->SendedFlag)
					{
						if(config->Entity->TimeCounter >= AHT10_InitTime)
						{
							config->Entity->SendedFlag = false;
							
							//Send firs request if we are in automatic mode
							if(config->Entity->WorkMode == AHT10_WorkMode_AUTOMATIC)
							{
								//Send
								config->I2C_Functions.Send(AHT10_I2C_ADDRESS, (uint8_t *)cmd_Measure, 3);

								config->Entity->TimeCounter = 0;
								config->Entity->SendedFlag = true;
							}

							//Step
							config->Entity->WorkStep = AHT10_Step_Working;
						}
					}
					else
					{
						//Send
						config->I2C_Functions.Send(AHT10_I2C_ADDRESS, (uint8_t *)cmd_Init, 3);

						config->Entity->TimeCounter = 0;
						config->Entity->SendedFlag = true;
					}
				}
				break;

				case AHT10_Step_Working:
				{
					if(config->Entity->SendedFlag)
					{
						if(config->Entity->TimeCounter >= AHT10_MeasurementTime)
						{
							//Receive
							uint8_t raw_data[6] = { 0 };
							config->I2C_Functions.Receive(AHT10_I2C_ADDRESS, raw_data, 6);

							if(~raw_data[0] & 0x80)
							{
								uint32_t extracted_data;
								float temperature = 0.0f;
								float humidity = 0.0f;

								// Convert to Temperature in °C
								extracted_data = (((uint32_t)raw_data[3] & 15) << 16) | ((uint32_t)raw_data[4] << 8) | raw_data[5];
								temperature = (float)(extracted_data * 200.0f / 1048576.0f) - 50.0f;

								// Convert to Relative Humidity in %
								extracted_data = ((uint32_t)raw_data[1] << 12) | ((uint32_t)raw_data[2] << 4) | (raw_data[3] >> 4);
								humidity = (float)(extracted_data * 100.0f / 1048576.0f);

								if(config->DataReadyCallback)
								{
									config->DataReadyCallback(temperature, humidity);
								}
							}

							config->Entity->SendedFlag = false;
						}
					}
					else
					{
						if(config->Entity->WorkMode == AHT10_WorkMode_AUTOMATIC)
						{
							if(config->Entity->TimeCounter >= config->Entity->Period)
							{
								//Send
								config->I2C_Functions.Send(AHT10_I2C_ADDRESS, (uint8_t *)cmd_Measure, 3);

								config->Entity->TimeCounter = 0;
								config->Entity->SendedFlag = true;
							}
						}
					}
				}
				break;
			}
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

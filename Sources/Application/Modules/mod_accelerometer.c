/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_accelerometer.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_accelerometer.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "bma280.h"
#include "if_twi0.h"
#include "if_exti.h"
#include "if_delay.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
static void Accelerometer_INT_Callback(void);
static void Accelerometer_DataReady_Callback(BMA280_Data_t *data);

/*-- Local variables --------------------------------------------------------*/
static BMA280_Entity_t bma280_entity;
static const BMA280_Config_t bma280_cfg = 
{
	.I2cAddress = BMA280_I2C_DEFAULT_ADDRESS,
	.DataRange = BMA280_DataRange_4g,
	.FilterBandwidth = BMA280_FilterBandwidth_125Hz,
	.LowPowerMode = BMA280_PowerMode_Normal,
	.SleepDuraction = BMA280_SleepDuraction_100ms,
    .Functions =
    {
        .Send = If_TWI0_Send,
        .Receive = If_TWI0_Receive,
		.Delay_us = If_Delay_us,
		.DataReadyCallback = Accelerometer_DataReady_Callback
    },
	.AutoModeConfig =
	{
		.Acceleration_X = true,
		.Acceleration_Y = true,
		.Acceleration_Z = true,
		.Temperature = true,
		.Orientation = true,
		.StatusSlope = true,
		.StatusTap = true
	},
    .Entity = &bma280_entity
};

static bool interrupt_triggered = false;
AccelerometerData_t AccelerometerData;

/*-- Local functions --------------------------------------------------------*/
static void Accelerometer_INT_Callback(void)
{
	interrupt_triggered = true;
}

static void Accelerometer_DataReady_Callback(BMA280_Data_t *data)
{
	AccelerometerData.X = (int16_t)data->Acceleration.X;
	AccelerometerData.Y = (int16_t)data->Acceleration.Y;
	AccelerometerData.Z = (int16_t)data->Acceleration.Z;
}

/*-- Exported functions -----------------------------------------------------*/
void Mod_Accelerometer_Init(void)
{
	If_TWI0_Init();

	If_Exti_Init();
	If_Exti_RegisterCallback(Accelerometer_INT_Callback);

	If_TWI0_Enable();
	If_Exti_Enable();

	//BMA280_Set_Period(&bma280_cfg, 250);
	//BMA280_Set_WorkMode(&bma280_cfg, BMA280_WorkMode_AUTOMATIC);

	BMA280_Set_WorkMode(&bma280_cfg, BMA280_WorkMode_MANUAL);
}

void Mod_Accelerometer_Tick(uint32_t ms)
{
	BMA280_Tick(&bma280_cfg, ms);
}

void Mod_Accelerometer_Run(void)
{
	if(interrupt_triggered)
	{
		if(BMA280_Get_Ready(&bma280_cfg))
		{
			BMA280_Data_t bma280_data;

			//All operations takes ~2ms
			bma280_data.Acceleration.X = BMA280_Read_Acceleration(&bma280_cfg, BMA280_Axis_X);
			bma280_data.Acceleration.Y = BMA280_Read_Acceleration(&bma280_cfg, BMA280_Axis_Y);
			bma280_data.Acceleration.Z = BMA280_Read_Acceleration(&bma280_cfg, BMA280_Axis_Z);
			bma280_data.Temperature = BMA280_Read_Temparature(&bma280_cfg);
			bma280_data.Orient = BMA280_Read_Orient(&bma280_cfg);
			bma280_data.Slope = BMA280_Read_Status_Slope(&bma280_cfg);
			bma280_data.Tap = BMA280_Read_Status_Tap(&bma280_cfg);

			AccelerometerData.X = (int16_t)bma280_data.Acceleration.X;
			AccelerometerData.Y = (int16_t)bma280_data.Acceleration.Y;
			AccelerometerData.Z = (int16_t)bma280_data.Acceleration.Z;
		}

		interrupt_triggered = false;
	}

	BMA280_Run(&bma280_cfg);
}

/*-- EOF --------------------------------------------------------------------*/

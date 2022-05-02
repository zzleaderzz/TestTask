/*-- File description -------------------------------------------------------*/
/**
 *   @file:    bma280.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _BMA280_H
#define _BMA280_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
#define BMA280_I2C_DEFAULT_ADDRESS			0x18

/*-- Typedefs ---------------------------------------------------------------*/
typedef bool(*BMA280_I2C_Transfer)(uint8_t address, uint8_t *data, uint8_t length);
typedef void(*BMA280_Delay_us)(uint32_t delay_us);

typedef enum
{
	BMA280_Axis_X,
	BMA280_Axis_Y,
	BMA280_Axis_Z,
} BMA280_Axis_e;

typedef enum
{
	BMA280_Orient_XY_UpsideDown,
	BMA280_Orient_XY_LandscapeLeft,
	BMA280_Orient_XY_LandscapeRigth,
	BMA280_Orient_XY_UpRight,
} BMA280_Orient_xy_e;

typedef enum
{
	BMA280_Orient_Z_Upward,
	BMA280_Orient_Z_Downward,
} BMA280_Orient_z_e;

typedef struct
{
	BMA280_Orient_xy_e XY;
	BMA280_Orient_z_e Z;
} BMA280_Orient_t;

typedef enum
{
	BMA280_Slope_None,
	BMA280_Slope_X_Positive,
	BMA280_Slope_X_Negative,
	BMA280_Slope_Y_Positive,
	BMA280_Slope_Y_Negative,
	BMA280_Slope_Z_Positive,
	BMA280_Slope_Z_Negative,
} BMA280_Slope_e;

typedef enum
{
	BMA280_Tap_None,
	BMA280_Tap_X_Positive,
	BMA280_Tap_X_Negative,
	BMA280_Tap_Y_Positive,
	BMA280_Tap_Y_Negative,
	BMA280_Tap_Z_Positive,
	BMA280_Tap_Z_Negative,
} BMA280_Tap_e;

typedef struct
{
	struct
	{
		float X;
		float Y;
		float Z;
	} Acceleration;
	float Temperature;
	BMA280_Orient_t Orient;
	BMA280_Slope_e Slope;
	BMA280_Tap_e Tap;
} BMA280_Data_t;

typedef void(*BMA280_DataReceivedCallback)(BMA280_Data_t *data);

typedef enum
{
	BMA280_DataRange_2g,
	BMA280_DataRange_4g,
	BMA280_DataRange_8g,
	BMA280_DataRange_16g
} BMA280_DataRange_e; 

typedef enum
{
	BMA280_FilterBandwidth_7_81Hz,
	BMA280_FilterBandwidth_15_63Hz,
	BMA280_FilterBandwidth_31_25Hz,
	BMA280_FilterBandwidth_62_5Hz,
	BMA280_FilterBandwidth_125Hz,
	BMA280_FilterBandwidth_250Hz,
	BMA280_FilterBandwidth_500Hz,
	BMA280_FilterBandwidth_Max,
} BMA280_FilterBandwidth_e; 

typedef enum
{
	BMA280_PowerMode_Normal,
	BMA280_PowerMode_DeepSleep,
	BMA280_PowerMode_LowPower,
	BMA280_PowerMode_Suspend
} BMA280_LowPowerMode_e; 

typedef enum
{
	BMA280_SleepDuraction_500us,
	BMA280_SleepDuraction_1ms,
	BMA280_SleepDuraction_2ms,
	BMA280_SleepDuraction_4ms,
	BMA280_SleepDuraction_6ms,
	BMA280_SleepDuraction_10ms,
	BMA280_SleepDuraction_25ms,
	BMA280_SleepDuraction_50ms,
	BMA280_SleepDuraction_100ms,
	BMA280_SleepDuraction_500ms,
	BMA280_SleepDuraction_1s
} BMA280_SleepDuraction_e; 

typedef enum
{
	BMA280_WorkMode_MANUAL,
	BMA280_WorkMode_AUTOMATIC,
} BMA280_WorkMode_t;

typedef enum
{
	BMA280_Step_WakeUp,
	BMA280_Step_Reset,
	BMA280_Step_Init_1,
	BMA280_Step_Init_2,
	BMA280_Step_Init_3,
	BMA280_Step_Init_4,
	BMA280_Step_Working,
} BMA280_WorkSteps_e;

typedef struct
{
	BMA280_I2C_Transfer Send;
	BMA280_I2C_Transfer Receive;
	BMA280_Delay_us Delay_us;
	BMA280_DataReceivedCallback DataReadyCallback;
} BMA280_Functions_t;

typedef struct
{
	BMA280_WorkSteps_e WorkStep;
	BMA280_WorkMode_t WorkMode;
	uint32_t Period;
	bool SendedFlag;
	uint32_t TimeCounter;
} BMA280_Entity_t;

typedef struct
{
	bool Acceleration_X : 1;
	bool Acceleration_Y : 1;
	bool Acceleration_Z : 1;
	bool Temperature : 1;
	bool Orientation : 1;
	bool StatusSlope : 1;
	bool StatusTap : 1;
} BMA280_AutomaticModeConfig_t;

typedef struct
{
	uint8_t I2cAddress;
	BMA280_DataRange_e DataRange;
	BMA280_FilterBandwidth_e FilterBandwidth;
	BMA280_LowPowerMode_e LowPowerMode;
	BMA280_SleepDuraction_e SleepDuraction;
	BMA280_Functions_t Functions;
	BMA280_AutomaticModeConfig_t AutoModeConfig;
	BMA280_Entity_t *Entity;
} BMA280_Config_t;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void BMA280_Tick(const BMA280_Config_t *config, uint32_t ms);
bool BMA280_Get_Ready(const BMA280_Config_t *config);
void BMA280_Set_WorkMode(const BMA280_Config_t *config, BMA280_WorkMode_t mode);
void BMA280_Set_Period(const BMA280_Config_t *config, uint32_t period_ms);

float BMA280_Read_Temparature(const BMA280_Config_t *config);
BMA280_Orient_t BMA280_Read_Orient(const BMA280_Config_t *config);
float BMA280_Read_Acceleration(const BMA280_Config_t *config, BMA280_Axis_e axis);
BMA280_Slope_e BMA280_Read_Status_Slope(const BMA280_Config_t *config);
BMA280_Tap_e BMA280_Read_Status_Tap(const BMA280_Config_t *config);
void BMA280_Reset(const BMA280_Config_t *config);

void BMA280_Run(const BMA280_Config_t *config);

#endif // _BMA280_H
/*-- EOF --------------------------------------------------------------------*/

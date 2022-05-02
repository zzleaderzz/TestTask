/*-- File description -------------------------------------------------------*/
/**
 *   @file:    bma280.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "bma280.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define BMA280_WakeUpTime					            5 //ms   //1.8 in datasheet
#define BMA280_InitTime						            25 //ms


//BMA 280 Registers
#define BMA280_REG_CHIP_ID				                0x00
#define BMA280_REG_AXIS_X_LSB                           0x02
#define BMA280_REG_AXIS_X_MSB                           0x03
#define BMA280_REG_AXIS_Y_LSB                           0x04
#define BMA280_REG_AXIS_Y_MSB                           0x05
#define BMA280_REG_AXIS_Z_LSB                           0x06
#define BMA280_REG_AXIS_Z_MSB                           0x07
#define BMA280_REG_TEMPERATURE                          0x08
#define BMA280_REG_INT_STATUS_0                         0x09
#define BMA280_REG_INT_STATUS_1                         0x0A
#define BMA280_REG_INT_STATUS_2                         0x0B
#define BMA280_REG_INT_STATUS_3                         0x0C
#define BMA280_REG_FIFO_STATUS                          0x0E
#define BMA280_REG_PMU_RANGE                            0x0F
#define BMA280_REG_PMU_BW                               0x10
#define BMA280_REG_PMU_LPW                              0x11
#define BMA280_REG_PMU_LOW_POWER                        0x12
#define BMA280_REG_ACCD_HBW                             0x13
#define BMA280_REG_BGW_SOFTRESET                        0x14
#define BMA280_REG_INT_EN_0                             0x16
#define BMA280_REG_INT_EN_1                             0x17
#define BMA280_REG_INT_EN_2                             0x18
#define BMA280_REG_INT_MAP_0                            0x19
#define BMA280_REG_INT_MAP_1                            0x1A
#define BMA280_REG_INT_MAP_2                            0x1B
#define BMA280_REG_INT_SRC                              0x1E
#define BMA280_REG_INT_OUT_CTRL                         0x20
#define BMA280_REG_INT_RST_LATCH                        0x21
#define BMA280_REG_INT1                                 0x23
#define BMA280_REG_INT2                                 0x24
#define BMA280_REG_INT3                                 0x25
#define BMA280_REG_INT4                                 0x26
#define BMA280_REG_INT5                                 0x27
#define BMA280_REG_INT6                                 0x28
#define BMA280_REG_INT7                                 0x29
#define BMA280_REG_INT8                                 0x2A
#define BMA280_REG_INT9                                 0x2B
#define BMA280_REG_INT_A                                0x2C
#define BMA280_REG_INT_B                                0x2D
#define BMA280_REG_INT_C                                0x2E
#define BMA280_REG_INT_D                                0x2F
#define BMA280_REG_FIFO_CONFIG_0                        0x30
#define BMA280_REG_PMU_SELF_TEST                        0x32
#define BMA280_REG_TRIM_NVM_CTRL                        0x33
#define BMA280_REG_BGW_SPI3_WDT                         0x34
#define BMA280_REG_OFC_CTRL                             0x36
#define BMA280_REG_OFC_SETTINGS                         0x37
#define BMA280_REG_OFC_OFFSET_X                         0x38
#define BMA280_REG_OFC_OFFSET_Y                         0x39
#define BMA280_REG_OFC_OFFSET_Z                         0x3A
#define BMA280_REG_TRIM_GP_0                            0x3B
#define BMA280_REG_TRIM_GP_1                            0x3C
#define BMA280_REG_FIFO_CONFIG_1                        0x3E
#define BMA280_REG_FIFO_DATA                            0x3F


//BMA 280 Commands
#define BMA280_CND_SOFT_RESET                           0xB6


//BMA280 Interrupt status register 0
#define BMA280_STATUS_0_FLAT_INT                        0x80
#define BMA280_STATUS_0_ORIENT_INT                      0x40
#define BMA280_STATUS_0_SINGLE_TAP_INT                  0x20
#define BMA280_STATUS_0_DOUBLE_TAP_INT                  0x10
#define BMA280_STATUS_0_MOTION_INT                      0x08
#define BMA280_STATUS_0_SLOPE_INT                       0x04
#define BMA280_STATUS_0_HIGH_INT                        0x02
#define BMA280_STATUS_0_LOW_INT                         0x01

//BMA280 Interrupt status register 1
#define BMA280_STATUS_1_DATA_INT			            0x80
#define BMA280_STATUS_1_FIFO_WM_INT                     0x40
#define BMA280_STATUS_1_FIFO_FULL_INT                   0x20

//BMA280 Interrupt status register 2
#define BMA280_STATUS_2_TAP_SING                        0x80
#define BMA280_STATUS_2_TAP_FIRST_Z                     0x40
#define BMA280_STATUS_2_TAP_FIRST_Y                     0x20
#define BMA280_STATUS_2_TAP_FIRST_X                     0x10
#define BMA280_STATUS_2_SLOPE_SING                      0x08
#define BMA280_STATUS_2_SLOPE_FIRST_Z                   0x04
#define BMA280_STATUS_2_SLOPE_FIRST_Y                   0x02
#define BMA280_STATUS_2_SLOPE_FIRST_X                   0x01

//BMA280 Interrupt status register 3
#define BMA280_STATUS_3_FLAT_POSITION                   0x80
#define BMA280_STATUS_3_ORIENT_Z_UPWARD                 0x00
#define BMA280_STATUS_3_ORIENT_Z_DOWNWARD               0x40
#define BMA280_STATUS_3_ORIENT_XY_UPRIGHT               0x00
#define BMA280_STATUS_3_ORIENT_XY_UPSIDE                0x10
#define BMA280_STATUS_3_ORIENT_XY_LANDSCAPE_LEFT        0x20
#define BMA280_STATUS_3_ORIENT_XY_LANDSCAPE_RIGHT       0x30
#define BMA280_STATUS_3_HIGH_SING_POSITIVE              0x00
#define BMA280_STATUS_3_HIGH_SING_NEGATIVE              0x08
#define BMA280_STATUS_3_HIGH_FIRST_Z                    0x04
#define BMA280_STATUS_3_HIGH_FIRST_Y                    0x02
#define BMA280_STATUS_3_HIGH_FIRST_X                    0x01

//BMA280 Interrupt enable group 0
#define BMA280_INT_EN_0_FLAT_ENABLE                     0x80
#define BMA280_INT_EN_0_ORIENT_ENABLE                   0x40
#define BMA280_INT_EN_0_SINGLE_TAP_ENABLE               0x20
#define BMA280_INT_EN_0_DOUBLE_TAP_ENABLE               0x10
#define BMA280_INT_EN_0_SLOPE_Z_ENABLE                  0x04
#define BMA280_INT_EN_0_SLOPE_Y_ENABLE                  0x02
#define BMA280_INT_EN_0_SLOPE_X_ENABLE                  0x01

//BMA280 Interrupt enable group 1
#define BMA280_INT_EN_1_FIFO_WM_ENABLE                  0x40
#define BMA280_INT_EN_1_FIFO_FULL_ENABLE                0x20
#define BMA280_INT_EN_1_DATA_ENABLE                     0x10
#define BMA280_INT_EN_1_LOW_ENABLE                      0x08
#define BMA280_INT_EN_1_HIGH_Z_ENABLE                   0x04
#define BMA280_INT_EN_1_HIGH_Y_ENABLE                   0x02
#define BMA280_INT_EN_1_HIGH_X_ENABLE                   0x01

//BMA280 Interrupt enable group 2
#define BMA280_INT_EN_2_MOTION_ENABLE                   0x08
#define BMA280_INT_EN_2_MOTION_Z_ENABLE                 0x04
#define BMA280_INT_EN_2_MOTION_Y_ENABLE                 0x02
#define BMA280_INT_EN_2_MOTION_X_ENABLE                 0x01

//BMA280 Interrupt map group 0
#define BMA280_INT1_MAP_FLAT_ENABLE                     0x80
#define BMA280_INT1_MAP_ORIENT_ENABLE                   0x40
#define BMA280_INT1_MAP_SINGLE_TAP_ENABLE               0x20
#define BMA280_INT1_MAP_DOUBLE_TAP_ENABLE               0x10
#define BMA280_INT1_MAP_MOTION_ENABLE                   0x08
#define BMA280_INT1_MAP_SLOPE_ENABLE                    0x04
#define BMA280_INT1_MAP_HIGH_ENABLE                     0x02
#define BMA280_INT1_MAP_LOW_ENABLE                      0x01

//BMA280 Interrupt map group 1
#define BMA280_INT2_MAP_DATA_ENABLE                     0x80
#define BMA280_INT2_MAP_FIFO_WM_ENABLE                  0x40
#define BMA280_INT2_MAP_FIFO_FULL_ENABLE                0x20
#define BMA280_INT1_MAP_DATA_ENABLE                     0x01
#define BMA280_INT1_MAP_FIFO_WM_ENABLE                  0x02
#define BMA280_INT1_MAP_FIFO_FULL_ENABLE                0x04

//BMA280 Interrupt map group 2
#define BMA280_INT2_MAP_FLAT_ENABLE                     0x80
#define BMA280_INT2_MAP_ORIENT_ENABLE                   0x40
#define BMA280_INT2_MAP_SINGLE_TAP_ENABLE               0x20
#define BMA280_INT2_MAP_DOUBLE_TAP_ENABLE               0x10
#define BMA280_INT2_MAP_MOTION_ENABLE                   0x08
#define BMA280_INT2_MAP_SLOPE_ENABLE                    0x04
#define BMA280_INT2_MAP_HIGH_ENABLE                     0x02
#define BMA280_INT2_MAP_LOW_ENABLE                      0x01

//BMA280 Interrupt with selectable data source
#define BMA280_INT_SRC_DATA                             0x20
#define BMA280_INT_SRC_TAP                              0x10
#define BMA280_INT_SRC_MOTION                           0x08
#define BMA280_INT_SRC_SLOPE                            0x04
#define BMA280_INT_SRC_HIGH                             0x02
#define BMA280_INT_SRC_LOW                              0x01

//BMA280 Configuration of the interrupt pin 
#define BMA280_INT2_CTRL_OPEN_DRAIN                     0x08
#define BMA280_INT1_CTRL_OPEN_DRAIN                     0x02
#define BMA280_INT2_CTRL_ACTIVE_HIGH_LEVEL              0x04
#define BMA280_INT1_CTRL_ACTIVE_HIGH_LEVEL              0x01

//BMA280 Interrupt reset and interrupt selection mode
#define BMA280_LATCH_RESET_INT                          0x80
#define BMA280_LATCH_INT_250ms                          0x01
#define BMA280_LATCH_INT_500ms                          0x02
#define BMA280_LATCH_INT_1sec                           0x03
#define BMA280_LATCH_INT_2sec                           0x04
#define BMA280_LATCH_INT_4sec                           0x05
#define BMA280_LATCH_INT_8sec                           0x06
#define BMA280_LATCH_INT_250us                          0x09
#define BMA280_LATCH_INT_500us                          0x0A
#define BMA280_LATCH_INT_1ms                            0x0B
#define BMA280_LATCH_INT_12_5ms                         0x0C
#define BMA280_LATCH_INT_25ms                           0x0D
#define BMA280_LATCH_INT_50ms                           0x0E

//BMA280 Single tap and double tap interrupts
#define BMA280_TAP_QUIET_30ms                           0x00
#define BMA280_TAP_QUIET_20ms                           0x80
#define BMA280_TAP_SHOCK_50ms                           0x00
#define BMA280_TAP_SHOCK_75ms                           0x40
#define BMA280_TAP_DUR_50ms                             0x00
#define BMA280_TAP_DUR_100ms                            0x01
#define BMA280_TAP_DUR_150ms                            0x02
#define BMA280_TAP_DUR_200ms                            0x03
#define BMA280_TAP_DUR_250ms                            0x04
#define BMA280_TAP_DUR_375ms                            0x05
#define BMA280_TAP_DUR_500ms                            0x06
#define BMA280_TAP_DUR_700ms                            0x07

//BMA280 Tap samples
#define BMA280_TAP_2_SAMPLES                            0x00
#define BMA280_TAP_4_SAMPLES                            0x40
#define BMA280_TAP_8_SAMPLES                            0x80
#define BMA280_TAP_16_SAMPLES                           0xC0

//BMA280 Self test
#define BMA280_SELF_TEST_SING_POSITIVE                  0x40
#define BMA280_SELF_TEST_SING_NEGATIVE                  0x00
#define BMA280_SELF_TEST_DISABLE                        0x00
#define BMA280_SELF_TEST_X_AXIS                         0x01
#define BMA280_SELF_TEST_Y_AXIS                         0x02
#define BMA280_SELF_TEST_Z_AXIS                         0x03


//BMA280 Settings for the digital interfaces
#define BMA280_I2C_ENABLE                               0x04
#define BMA280_I2C_DISABLE                              0x00
#define BMA280_I2C_WTD_1ms                              0x00
#define BMA280_I2C_WTD_50ms                             0x02
#define BMA280_SPI_4_WIRE                               0x00
#define BMA280_SPI_3_WIRE                               0x01


//BMA280 Offset control register
#define BMA280_OFFSET_RESET                             0x80
#define BMA280_OFFSET_TRIGGER_X_AXIS                    0x20
#define BMA280_OFFSET_TRIGGER_Y_AXIS                    0x40
#define BMA280_OFFSET_TRIGGER_Z_AXIS                    0x60
#define BMA280_OFFSET_NOT_TRIGGER                       0x00
#define BMA280_OFC_HP_X_ENABLE                          0x04
#define BMA280_OFC_HP_Y_ENABLE                          0x02
#define BMA280_OFC_HP_Z_ENABLE                          0x01

//BMA280 Offset settings
#define BMA280_OFFSET_TARGET_Z_0g                       0x00
#define BMA280_OFFSET_TARGET_Z_PLUS_1g                  0x20
#define BMA280_OFFSET_TARGET_Z_MINUS_1g                 0x40
#define BMA280_OFFSET_TARGET_Y_PLUS_1g                  0x08
#define BMA280_OFFSET_TARGET_Y_MINUS_1g                 0x10
#define BMA280_OFFSET_TARGET_X_PLUS_1g                  0x02
#define BMA280_OFFSET_TARGET_X_MINUS_1g                 0x04
#define BMA280_OFFSET_CUT_OFF_1Hz                       0x00
#define BMA280_OFFSET_CUT_OFF_10Hz                      0x01

//BMA280 FIFO mode and FIFO data selsect
#define BMA280_FIFO_MODE_BYPASS                         0x00
#define BMA280_FIFO_MODE_FIFO                           0x40
#define BMA280_FIFO_MODE_STREAM                         0x80
#define BMA280_FIFO_SELECT_XYZ_AXIS                     0x00
#define BMA280_FIFO_SELECT_X_AXIS                       0x01
#define BMA280_FIFO_SELECT_Y_AXIS                       0x02
#define BMA280_FIFO_SELECT_Z_AXIS                       0x03

/*-- Local Typedefs ---------------------------------------------------------*/
typedef struct
{
	uint8_t value_register;
	float value_float;
} RegisterAndFloatValues_t;

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
const RegisterAndFloatValues_t data_range_values[] =
{
	{ 0x03, 2.0f },
	{ 0x05, 4.0f },
	{ 0x08, 8.0f },
	{ 0x0C, 16.0f }
};

const RegisterAndFloatValues_t filter_bandwidth_values[] =
{
	{ 0x08, 7.81f },
	{ 0x09, 15.63f },
	{ 0x0A, 31.25f },
	{ 0x0B, 62.5f },
	{ 0x0C, 125.0f },
	{ 0x0D, 250.0f },
	{ 0x0E, 500.0f },
	{ 0x0F, 2000.0f },
};

const uint8_t power_mode_values[] =
{
	0x00,
	0x20,
	0x40,
	0x80
};

const uint8_t sleep_duraction_values[] =
{
	0x05,
	0x06,
	0x07,
	0x08,
	0x09,
	0x0A,
	0x0B,
	0x0C,
	0x0D,
	0x0E,
	0x0F,
};

/*-- Local functions --------------------------------------------------------*/
static void BMA280_I2C_Write(const BMA280_Config_t *config, uint8_t reg, uint8_t data)
{
	if(config->Functions.Send)
	{
		uint8_t buufer[2] = { reg, data };
		config->Functions.Send(config->I2cAddress, buufer, 2);
	}

	//Need to wait up to 500 us after writing register to apply changes
	if(config->Functions.Delay_us)
	{
		config->Functions.Delay_us(500);
	}
}

static void BMA280_I2C_Read(const BMA280_Config_t *config, uint8_t reg, uint8_t *data_buf, uint8_t len)
{
	//Set register to read from
	if(config->Functions.Send)
	{
		config->Functions.Send(config->I2cAddress, &reg, 1);
	}

	//Need to wait up to 450 us after writing register to apply changes
	if(config->Functions.Delay_us)
	{
		config->Functions.Delay_us(50);
	}

	//Read
	if(config->Functions.Receive)
	{
		config->Functions.Receive(config->I2cAddress, data_buf, len);
	}

	//Need to wait up to 100 us after reading register
	if(config->Functions.Delay_us)
	{
		config->Functions.Delay_us(50);
	}
}

/*-- Exported functions -----------------------------------------------------*/
void BMA280_Tick(const BMA280_Config_t *config, uint32_t ms)
{
	if((config) && (config->Entity) && (ms > 0))
	{
		config->Entity->TimeCounter += ms;
	}
}

bool BMA280_Get_Ready(const BMA280_Config_t *config)
{
	if(config && config->Entity)
	{
		if(config->Entity->WorkStep == BMA280_Step_Working)
		{
			return true;
		}
	}

	return false;
}

void BMA280_Set_WorkMode(const BMA280_Config_t *config, BMA280_WorkMode_t mode)
{
	if((config) && (config->Entity))
	{
		config->Entity->WorkMode = mode;
	}
}

void BMA280_Set_Period(const BMA280_Config_t *config, uint32_t period_ms)
{
	if(config && config->Entity)
	{
		config->Entity->Period = period_ms;
	}
}

float BMA280_Read_Temparature(const BMA280_Config_t *config)
{
	if(config && config->Entity)
	{
		if(config->Entity->WorkMode == BMA280_WorkMode_MANUAL)
		{
			int8_t read_reg = 0;

			BMA280_I2C_Read(config, BMA280_REG_TEMPERATURE, &read_reg, 1);

			return (0.5f * ((float)read_reg) + 23.0f);
		}
	}

	return 0;
}

BMA280_Orient_t BMA280_Read_Orient(const BMA280_Config_t *config)
{
	BMA280_Orient_t orient;

	if(config && config->Entity)
	{
		if(config->Entity->WorkMode == BMA280_WorkMode_MANUAL)
		{
			uint8_t orient_data = 0;
			
			BMA280_I2C_Read(config, BMA280_REG_INT_STATUS_3, &orient_data, 1);

			//XY
			if((orient_data & 0x30) == 0x30)
			{
				orient.XY = BMA280_Orient_XY_LandscapeRigth;
			}
			else if((orient_data & 0x20) != 0)
			{
				orient.XY = BMA280_Orient_XY_LandscapeLeft;
			}
			else if ((orient_data & 0x10) != 0)
			{
				orient.XY = BMA280_Orient_XY_UpsideDown;
			}
			else
			{
				orient.XY = BMA280_Orient_XY_UpRight;
			}

			//Z
			if((orient_data & 0x40) != 0)
			{
				orient.Z = BMA280_Orient_Z_Upward;
			}
			else
			{
				orient.Z = BMA280_Orient_Z_Downward;
			}
		}
	}

	return orient;
}

float BMA280_Read_Acceleration(const BMA280_Config_t *config, BMA280_Axis_e axis)
{
	float f_axis = 0.0f;

	if(config && config->Entity)
	{
		if(config->Entity->WorkMode == BMA280_WorkMode_MANUAL)
		{
			if(config && config->Entity)
			{
				int16_t axis_data = 0;
				uint8_t reg = 0;

				switch(axis)
				{
					case BMA280_Axis_X:
					{
						reg = BMA280_REG_AXIS_X_LSB;
					}
					break;

					case BMA280_Axis_Y:
					{
						reg = BMA280_REG_AXIS_Y_LSB;
					}
					break;

					case BMA280_Axis_Z:
					{
						reg = BMA280_REG_AXIS_Z_LSB;
					}
					break;
				}

				BMA280_I2C_Read(config, reg, (uint8_t *)&axis_data, 2);

				axis_data = axis_data >> 2;
				f_axis = axis_data * ( data_range_values[config->DataRange].value_float / 8192.0f );// / 4.0f;
				f_axis = f_axis * 1000;
			}
		}
	}

	return f_axis;
}

BMA280_Slope_e BMA280_Read_Status_Slope(const BMA280_Config_t *config)
{
	if(config && config->Entity)
	{
		if(config->Entity->WorkMode == BMA280_WorkMode_MANUAL)
		{
			uint8_t slope_data = 0;
			BMA280_I2C_Read(config, BMA280_REG_INT_STATUS_2, &slope_data, 1);

			if((slope_data & 0x08) != 0)
			{
				if ((slope_data & 0x04) != 0)
				{
					return BMA280_Slope_X_Negative;
				}
				if ((slope_data & 0x02) != 0)
				{
					return BMA280_Slope_Y_Negative;
				}
				if ((slope_data & 0x01) != 0)
				{
					return BMA280_Slope_Z_Negative;
				}
			}
			else
			{
				if ((slope_data & 0x04) != 0)
				{
					return BMA280_Slope_X_Positive;
				}
				if ((slope_data & 0x02) != 0)
				{
					return BMA280_Slope_Y_Positive;
				}
				if ((slope_data & 0x01) != 0)
				{
					return BMA280_Slope_Z_Positive;
				}
			}
		}
	}

	return BMA280_Slope_None;
}

BMA280_Tap_e BMA280_Read_Status_Tap(const BMA280_Config_t *config)
{
	if(config && config->Entity)
	{
		if(config->Entity->WorkMode == BMA280_WorkMode_MANUAL)
		{
			uint8_t tap_data = 0;
			BMA280_I2C_Read(config, BMA280_REG_INT_STATUS_2, &tap_data, 1);

			if((tap_data & 0x80) != 0 )
			{
				if ((tap_data & 0x40) != 0)
				{
					return BMA280_Tap_X_Negative;
				}
				if ((tap_data & 0x20) != 0)
				{
					return BMA280_Tap_Y_Negative;
				}
				if ((tap_data & 0x10) != 0)
				{
					return BMA280_Tap_Z_Negative;
				}
			}
			else 
			{
				if ((tap_data & 0x40) != 0)
				{
					return BMA280_Tap_X_Positive;
				}
				if ((tap_data & 0x20) != 0)
				{
					return BMA280_Tap_Y_Positive;
				}
				if ((tap_data & 0x10) != 0)
				{
					return BMA280_Tap_Z_Positive;
				}
			}
		}
	}

	return BMA280_Tap_None;
}

void BMA280_Reset(const BMA280_Config_t *config)
{
	if(config && config->Entity)
	{
		config->Entity->SendedFlag = false;
		config->Entity->TimeCounter = 0;
		config->Entity->WorkStep = BMA280_Step_Reset;
	}
}

void BMA280_Run(const BMA280_Config_t *config)
{
	if(config && config->Entity)
	{
		switch(config->Entity->WorkStep)
		{
			case BMA280_Step_WakeUp:
			{
				if(config->Entity->SendedFlag)
				{
					if(config->Entity->TimeCounter >= BMA280_WakeUpTime)
					{
						config->Entity->SendedFlag = false;

						//Step
						config->Entity->WorkStep = BMA280_Step_Reset;
					}
				}
				else
				{
					config->Entity->TimeCounter = 0;
					config->Entity->SendedFlag = true;
				}
			}
			break;

			case BMA280_Step_Reset:
			{
				if(config->Entity->SendedFlag)
				{
					if(config->Entity->TimeCounter >= BMA280_WakeUpTime)
					{
						config->Entity->SendedFlag = false;

						//Step
						config->Entity->WorkStep = BMA280_Step_Init_1;
					}
				}
				else
				{
					//Send
					BMA280_I2C_Write(config, BMA280_REG_BGW_SOFTRESET, BMA280_CND_SOFT_RESET);

					config->Entity->TimeCounter = 0;
					config->Entity->SendedFlag = true;
				}
			}
			break;

			case BMA280_Step_Init_1:
			{
				if(config->Entity->SendedFlag)
				{
					if(config->Entity->TimeCounter >= 100)
					{
						config->Entity->SendedFlag = false;

						//Step
						config->Entity->WorkStep = BMA280_Step_Init_2;
					}
				}
				else
				{
					//Set full-scale range
					BMA280_I2C_Write(config, BMA280_REG_PMU_RANGE, data_range_values[config->DataRange].value_register);
					
					//Set bandwidth (and thereby sample rate) 
					BMA280_I2C_Write(config, BMA280_REG_PMU_BW, filter_bandwidth_values[config->FilterBandwidth].value_register);
					
					//Set power mode and sleep duration
					BMA280_I2C_Write(config, BMA280_REG_PMU_LPW, (power_mode_values[config->LowPowerMode] | (sleep_duraction_values[config->SleepDuraction] << 1)));

					config->Entity->TimeCounter = 0;
					config->Entity->SendedFlag = true;
				}
			}
			break;

			case BMA280_Step_Init_2:
			{
				if(config->Entity->SendedFlag)
				{
					if(config->Entity->TimeCounter >= 100)
					{
						config->Entity->SendedFlag = false;

						//Step
						config->Entity->WorkStep = BMA280_Step_Init_3;
					}
				}
				else
				{
					//Set I2C watchdog
					BMA280_I2C_Write(config, BMA280_REG_BGW_SPI3_WDT, (BMA280_I2C_ENABLE | BMA280_I2C_WTD_50ms));

					//Set data ready interrupt (bit 4)
					BMA280_I2C_Write(config, BMA280_REG_INT_EN_1, BMA280_INT_EN_1_DATA_ENABLE);

					//Map data ready interrupt to INT1 (bit 0)
					BMA280_I2C_Write(config, BMA280_REG_INT_MAP_1, BMA280_INT1_MAP_DATA_ENABLE);

					config->Entity->TimeCounter = 0;
					config->Entity->SendedFlag = true;
				}
			}
			break;

			case BMA280_Step_Init_3:
			{
				if(config->Entity->SendedFlag)
				{
					if(config->Entity->TimeCounter >= 100)
					{
						config->Entity->SendedFlag = false;

						//Step
						config->Entity->WorkStep = BMA280_Step_Init_4;
					}
				}
				else
				{
					//Send
					BMA280_I2C_Write(config, BMA280_REG_INT_EN_0, (BMA280_INT_EN_0_SINGLE_TAP_ENABLE |
																   BMA280_INT_EN_0_DOUBLE_TAP_ENABLE |
																   BMA280_INT_EN_0_ORIENT_ENABLE |
																   BMA280_INT_EN_0_SLOPE_X_ENABLE |
																   BMA280_INT_EN_0_SLOPE_Y_ENABLE |
																   BMA280_INT_EN_0_SLOPE_Z_ENABLE |
																   BMA280_INT_EN_0_FLAT_ENABLE));

					BMA280_I2C_Write(config, BMA280_REG_INT_MAP_0, (BMA280_INT1_MAP_ORIENT_ENABLE |
																   BMA280_INT1_MAP_SINGLE_TAP_ENABLE | 
																   BMA280_INT1_MAP_DOUBLE_TAP_ENABLE |
																   BMA280_INT1_MAP_SLOPE_ENABLE));

					config->Entity->TimeCounter = 0;
					config->Entity->SendedFlag = true;
				}
			}
			break;

			case BMA280_Step_Init_4:
			{
				if(config->Entity->SendedFlag)
				{
					if(config->Entity->TimeCounter >= 100) //Wait after write last config register
					{
						config->Entity->SendedFlag = false;

						//Step
						config->Entity->WorkStep = BMA280_Step_Working;
					}
				}
				else
				{
					//Set tap threshold to 10 x 3.125% of full range
					BMA280_I2C_Write(config, BMA280_REG_INT9, 10);

					//Interrupts push-pull, active HIGH (bits 0:3) 
					BMA280_I2C_Write(config, BMA280_REG_INT_OUT_CTRL, (BMA280_INT2_CTRL_ACTIVE_HIGH_LEVEL | BMA280_INT1_CTRL_ACTIVE_HIGH_LEVEL));

					config->Entity->TimeCounter = 0;
					config->Entity->SendedFlag = true;
				}
			}
			break;

			case BMA280_Step_Working:
			{
				if(config->Entity->WorkMode == BMA280_WorkMode_AUTOMATIC)
				{
					if(config->Entity->TimeCounter >= config->Entity->Period)
					{
						config->Entity->TimeCounter = 0;

						if(config->Functions.DataReadyCallback)
						{
							BMA280_Data_t data;

							if(config->AutoModeConfig.Acceleration_X)
							{
								data.Acceleration.X = BMA280_Read_Acceleration(config, BMA280_Axis_X);
							}

							if(config->AutoModeConfig.Acceleration_Y)
							{
								data.Acceleration.Y = BMA280_Read_Acceleration(config, BMA280_Axis_Y);
							}

							if(config->AutoModeConfig.Acceleration_Z)
							{
								data.Acceleration.Z = BMA280_Read_Acceleration(config, BMA280_Axis_Z);
							}

							if(config->AutoModeConfig.Temperature)
							{
								data.Temperature = BMA280_Read_Temparature(config);
							}

							if(config->AutoModeConfig.Orientation)
							{
								data.Orient = BMA280_Read_Orient(config);
							}

							if(config->AutoModeConfig.StatusSlope)
							{
								data.Slope = BMA280_Read_Status_Slope(config);
							}

							if(config->AutoModeConfig.StatusTap)
							{
								data.Tap = BMA280_Read_Status_Tap(config);
							}

							config->Functions.DataReadyCallback(&data);
						}
					}
				}
			}
			break;
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
 *   @file:    aht10.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: NIK El.
 */

#ifndef _AHT10_H
#define _AHT10_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
#define AHT10_I2C_ADDRESS					0x38

/*-- Typedefs ---------------------------------------------------------------*/
typedef bool(*AHT10_I2C_Transfer)(uint8_t address, uint8_t *data, uint8_t length);

typedef enum
{
	AHT10_WorkMode__MANUAL,
	AHT10_WorkMode__AUTOMATIC,
} AHT10_WorkMode_t;

typedef enum
{
	AHT10_Step_Startup,
	AHT10_Step_Reset,
	AHT10_Step_Init,
	AHT10_Step_Working,
} AHT10_WorkSteps_e;

typedef struct
{
	AHT10_I2C_Transfer Send;
	AHT10_I2C_Transfer Receive;
} AHT10_I2C_Functions_t;

typedef struct
{
	AHT10_WorkSteps_e WorkStep;
	AHT10_WorkMode_t WorkMode;
	uint32_t Period;
	bool SendedFlag;
	uint32_t TimeCounter;
} AHT10_Entity_t;

typedef struct
{
	AHT10_I2C_Functions_t I2C_Functions;
	AHT10_Entity_t *Entity;
} AHT10_Config_t;

typedef struct
{
	float Temperature;
	float Humidity;
} AHT10_Data_t;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void AHT10_Tick(const AHT10_Config_t *config, uint32_t ms);
void AHT10_Set_WorkMode(const AHT10_Config_t *config, AHT10_WorkMode_t mode);
void AHT10_Set_Period(const AHT10_Config_t *config, uint32_t period_ms);

void AHT10_Reset(const AHT10_Config_t *config);
void AHT10_Request(const AHT10_Config_t *config);
void AHT10_Run(const AHT10_Config_t *config, AHT10_Data_t *data);

#endif // _AHT10_H
/*-- EOF --------------------------------------------------------------------*/

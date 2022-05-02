/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_accelerometer.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_ACCELEROMETER_H
#define _MOD_ACCELEROMETER_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
} AccelerometerData_t;

/*-- Exported variables -----------------------------------------------------*/
extern AccelerometerData_t AccelerometerData;

/*-- Exported functions -----------------------------------------------------*/
void Mod_Accelerometer_Init(void);

void Mod_Accelerometer_Tick(uint32_t ms);

void Mod_Accelerometer_Run(void);

#endif // _MOD_ACCELEROMETER_H
/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_indication.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_INDICATION_H
#define _MOD_INDICATION_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef enum
{
	IndiStatus_Application_Idle,
	IndiStatus_Application_Sleep,
	IndiStatus_Application_Run,
} Mod_Indication_Application_status_t;

typedef enum
{
	IndiStatus_Bluetooth_Idle,
	IndiStatus_Bluetooth_Advertising,
	IndiStatus_Bluetooth_Connected,
} Mod_Indication_Bluetooth_status_t;

typedef enum
{
	IndiStatus_Accelerometer_Idle,
	IndiStatus_Accelerometer_DataUpdated,
} Mod_Indication_Accelerometer_status_t;

typedef enum
{
	IndiStatus_Audio_Idle,
	IndiStatus_Audio_Stop,
	IndiStatus_Audio_Play,
	IndiStatus_Audio_Paused,
	IndiStatus_Audio_Trigger
} Mod_Indication_Audio_status_t;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void Mod_Indication_Init(void);

void Mod_Indication_SetStatus_Application(Mod_Indication_Application_status_t status);
void Mod_Indication_SetStatus_Bluetooth(Mod_Indication_Bluetooth_status_t status);
void Mod_Indication_SetStatus_Accelerometer(Mod_Indication_Accelerometer_status_t status);
void Mod_Indication_SetStatus_Audio(Mod_Indication_Audio_status_t status);

void Mod_Indication_Run(void);

#endif // _MOD_INDICATION_H
/*-- EOF --------------------------------------------------------------------*/

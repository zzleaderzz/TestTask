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
	IndiStatus_Application_Run,
} Mod_Indication_Application_status_e;

typedef enum
{
	IndiStatus_Bluetooth_Idle,
	IndiStatus_Bluetooth_Advertising,
	IndiStatus_Bluetooth_Connected,
} Mod_Indication_Bluetooth_status_e;

typedef enum
{
	IndiStatus_Accelerometer_Idle,
	IndiStatus_Accelerometer_DataUpdated,
} Mod_Indication_Accelerometer_status_e;

typedef enum
{
	IndiStatus_Audio_Idle,
	IndiStatus_Audio_Play,
	IndiStatus_Audio_Paused,
	IndiStatus_Audio_Trigger
} Mod_Indication_Audio_status_e;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void Mod_Indication_Init(void);

bool Mod_Indication_IsBusy(void);
void Mod_Indication_EnterSleepMode(void);

void Mod_Indication_SetState_Application(Mod_Indication_Application_status_e status);
void Mod_Indication_SetState_Bluetooth(Mod_Indication_Bluetooth_status_e status);
void Mod_Indication_SetState_Accelerometer(Mod_Indication_Accelerometer_status_e status);
void Mod_Indication_SetState_Audio(Mod_Indication_Audio_status_e status);

void Mod_Indication_Run(void);

#endif // _MOD_INDICATION_H
/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_bluetooth.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_BLUETOOTH_H
#define _MOD_BLUETOOTH_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void Mod_Bluetooth_Init(void);

bool Mod_Bluetooth_IsBusy(void);
void Mod_Bluetooth_EnterSleepMode(void);

void Mod_Bluetooth_Accelerometer_NewData(uint16_t X, uint16_t Y, uint16_t Z);

void Mod_Bluetooth_Tick(uint32_t ms);

void Mod_Bluetooth_Run(void);

#endif // _MOD_BLUETOOTH_H
/*-- EOF --------------------------------------------------------------------*/

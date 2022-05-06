/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_ble.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_BLE_H
#define _MOD_BLE_H

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
void Mod_Ble_Init(void);

void Mod_Ble_Accelerometer_NewData(uint16_t X, uint16_t Y, uint16_t Z);

void Mod_Ble_Tick(uint32_t ms);

void Mod_Ble_Run(void);

#endif // _MOD_BLE_H
/*-- EOF --------------------------------------------------------------------*/

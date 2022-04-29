/*-- File description -------------------------------------------------------*/
/**
 *   @file:    drv_i2c.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: NIK El.
 */

#ifndef _DRV_I2C_H
#define _DRV_I2C_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "nrf_drv_twi.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
//const nrf_drv_twi_config_t twi_lm75b_config = {
//   .scl                = ARDUINO_SCL_PIN,		  //P0.27
//   .sda                = ARDUINO_SDA_PIN,         //P0.26
//   .frequency          = NRF_DRV_TWI_FREQ_400K,
//   .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
//   .clear_bus_init     = false
//};
typedef enum
{
	DrvI2c_0,
	DrvI2c_1,
} DrvI2c_Instance_e;

typedef enum
{
    DrvI2c_Freq_100K = NRF_DRV_TWI_FREQ_100K,   // 100 kbps.
    DrvI2c_Freq_250K = NRF_DRV_TWI_FREQ_250K,   // 250 kbps.
    DrvI2c_Freq_400K = NRF_DRV_TWI_FREQ_400K	// 400 kbps.
} DrvI2c_Frequency_e;

typedef struct
{
	nrf_drv_twi_t TwiModule_p;
} DrvI2c_Entity_t;

typedef struct
{
    uint32_t PinScl; // SCL pin number.
    uint32_t PinSda; // SDA pin number.
    DrvI2c_Frequency_e Frequency; // I2C frequency.
    uint8_t InterruptPriority; // Interrupt priority.
    bool ClearBusInit; // Clear bus during init.
    bool HoldBusUninit; // Hold pull up state on gpio pins after uninit.
} DrvI2c_BusConfig_t;

typedef struct
{
	DrvI2c_Instance_e Instance;
	DrvI2c_Entity_t *Entity;
	DrvI2c_BusConfig_t *Bus;
} DrvI2c_Config_t;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void DrvI2C_Init(DrvI2c_Config_t *config);

#endif // _DRV_I2C_H
/*-- EOF --------------------------------------------------------------------*/

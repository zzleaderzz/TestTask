/*-- File description -------------------------------------------------------*/
/**
 *   @file:    ble_aas.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _BLE_AAS_H
#define _BLE_AAS_H

#ifdef __cplusplus
extern "C" {
#endif

/*-- Standard C/C++ Libraries ---------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries ------------------------------------------------------------------------------------------------*/
/*-- Hardware specific libraries ------------------------------------------------------------------------------------*/
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

/*-- Project specific includes --------------------------------------------------------------------------------------*/
/*-- Exported macro -------------------------------------------------------------------------------------------------*/
#define BLE_AAS_DEF(_name)                                                                  \
static ble_aas_t _name;                                                                     \
NRF_SDH_BLE_OBSERVER(_name ## _obs, BLE_AAS_BLE_OBSERVER_PRIO, ble_ass_on_ble_evt, &_name)

#define AAS_UUID_BASE                           { 0xBC, 0x9a, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, \
                                                0xDE, 0xEF, 0xCD, 0xAB, 0xCC, 0xCC, 0xCC, 0xCC }

#define AAS_UUID_SERVICE                        0x1200
#define AAS_UUID_CHAR_ACCELEROMETER             0x1201
#define AAS_UUID_CHAR_AUDIO                     0x1202


#define AAS_ACCELEROMETER_SIZE                  6
#define AAS_AUDIO_SIZE                          1

/*-- Typedefs -------------------------------------------------------------------------------------------------------*/
//Forward declaration of the ble_aas_t type.
typedef struct ble_aas_s                    ble_aas_t;

typedef void (*ble_aas_audio_write_handler_t) (uint16_t conn_handle, ble_aas_t *p_aas, uint8_t value);

typedef struct
{
    ble_gatts_char_handles_t handles;				//Handles related to the Accelerometer Characteristic.
	uint8_t value[AAS_ACCELEROMETER_SIZE];
	uint16_t value_len;
} ble_aas_accel_char_t;

typedef struct
{
     ble_gatts_char_handles_t handles;              //Handles related to the Audio Characteristic.
	 uint8_t value[AAS_AUDIO_SIZE];
	 uint16_t value_len;
	 ble_aas_audio_write_handler_t write_handler;   //Event handler to be called when the Audio Characteristic is written.
} ble_aas_audio_char_t;

typedef struct
{
    ble_aas_accel_char_t accelerometer;
	ble_aas_audio_char_t audio;
} ble_aas_characteristics_t;

typedef struct
{
    uint16_t handle;                                //Handle of Accelerometer Audio Service (as provided by the BLE stack).
	uint8_t uuid_type;                              //UUID type for the Accelerometer Audio Service.
} ble_aas_service_t;

//Accelerometer Audio Service
//This structure contains various status information for the service.
struct ble_aas_s
{
    ble_aas_service_t service;
	ble_aas_characteristics_t characteristics;
};

/*-- Exported variables ---------------------------------------------------------------------------------------------*/
/*-- Exported functions ---------------------------------------------------------------------------------------------*/
/**@brief Function for initializing the Accelerometer Audio Service.
 *
 * @param[out] p_aas            Accelerometer Audio Service structure. This structure must be supplied by
 *                              the application. It is initialized by this function and will later
 *                              be used to identify this particular service instance.
 * @param[in] p_aas_init_params Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was initialized successfully. Otherwise, an error code is returned.
 */
uint32_t ble_aas_init(ble_aas_t *p_aas);


/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the Accelerometer Audio Service.
 *
 * @param[in] p_ble_evt	Event received from the BLE stack.
 * @param[in] p_context Accelerometer Audio Service structure.
 */
void ble_ass_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context);


/**@brief Function for sending a button state notification.
 *
 ' @param[in] conn_handle	Handle of the peripheral connection to which the button state notification will be sent.
 * @param[in] p_aas         Accelerometer Audio Service structure.
 * @param[in] button_state  New button state.
 *
 * @retval NRF_SUCCESS If the notification was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_aas_on_accelerometer_new_data(uint16_t conn_handle, ble_aas_t *p_aas, uint16_t x, uint16_t y, uint16_t z);


#ifdef __cplusplus
}
#endif

#endif // _BLE_AAS_H
/*-- EOF ------------------------------------------------------------------------------------------------------------*/

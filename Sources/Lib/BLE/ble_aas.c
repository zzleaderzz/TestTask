/*-- File description -------------------------------------------------------*/
/**
 *   @file:    ble_aas.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "ble_aas.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "sdk_common.h"
#include "ble_srv_common.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
static void on_audio_write(ble_aas_t *p_aas, ble_evt_t const *p_ble_evt)
{
    ble_gatts_evt_write_t const *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if ((p_evt_write->handle == p_aas->characteristics.audio.handles.value_handle) && (p_evt_write->len == AAS_AUDIO_SIZE))
    {
		if(p_aas->characteristics.audio.write_handler)
	    {
	        p_aas->characteristics.audio.write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_aas, p_evt_write->data[0]);
		}
    }
}

static uint32_t ble_add_characteristic_Accelerometer(ble_aas_t *p_aas)
{
	ble_add_char_params_t characteristic_params = { 0 };

	//Set Parameters
    characteristic_params.uuid  = AAS_UUID_CHAR_ACCELEROMETER;
    characteristic_params.uuid_type = p_aas->service.uuid_type;
    characteristic_params.max_len = p_aas->characteristics.accelerometer.value_len;

	//Save value storage out of stack
	characteristic_params.is_value_user = true;

	//Actios
    characteristic_params.char_props.read = 1;
    characteristic_params.char_props.notify = 1;

	//Access
    characteristic_params.read_access = SEC_OPEN;
    characteristic_params.cccd_write_access = SEC_OPEN;

	//Initial value
	characteristic_params.init_len = p_aas->characteristics.accelerometer.value_len;
	characteristic_params.p_init_value = p_aas->characteristics.accelerometer.value;

	//Add
    return characteristic_add(p_aas->service.handle, &characteristic_params, &p_aas->characteristics.accelerometer.handles);
}

static uint32_t ble_add_characteristic_Audio(ble_aas_t *p_aas)
{
	ble_add_char_params_t characteristic_params = { 0 };

	//Set Parameters
    characteristic_params.uuid  = AAS_UUID_CHAR_AUDIO;
    characteristic_params.uuid_type = p_aas->service.uuid_type;
    characteristic_params.max_len = p_aas->characteristics.audio.value_len;

	//Save value storage out of stack
	characteristic_params.is_value_user = true;

	//Actios
    characteristic_params.char_props.read = 1;
    characteristic_params.char_props.write = 1;

	//Access
    characteristic_params.read_access = SEC_OPEN;
    characteristic_params.write_access = SEC_OPEN;

	//Initial value
	characteristic_params.init_len = p_aas->characteristics.audio.value_len;
	characteristic_params.p_init_value = p_aas->characteristics.audio.value;

	//Add
    return characteristic_add(p_aas->service.handle, &characteristic_params, &p_aas->characteristics.audio.handles);
}

/*-- Exported functions -----------------------------------------------------*/
uint32_t ble_aas_init(ble_aas_t *p_aas)
{
    uint32_t err_code = NRF_SUCCESS;
    ble_uuid_t ble_uuid;

    //Add service
    ble_uuid128_t base_uuid = { AAS_UUID_BASE };
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_aas->service.uuid_type);
    VERIFY_SUCCESS(err_code);

	//Set BLE UUID
    ble_uuid.type = p_aas->service.uuid_type;
    ble_uuid.uuid = AAS_UUID_SERVICE;
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_aas->service.handle);
    VERIFY_SUCCESS(err_code);

    //Add Accelerometer characteristic.
    err_code = ble_add_characteristic_Accelerometer(p_aas);
	VERIFY_SUCCESS(err_code);

    //Add Audio characteristic.
    err_code = ble_add_characteristic_Audio(p_aas);
	VERIFY_SUCCESS(err_code);

	return err_code;
}

void ble_ass_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context)
{
    ble_aas_t *p_aas = (ble_aas_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTS_EVT_WRITE:
		{
			on_audio_write(p_aas, p_ble_evt);
		}
		break;

        default:
		// No implementation needed.
		break;
    }
}

uint32_t ble_aas_on_accelerometer_new_data(uint16_t conn_handle, ble_aas_t *p_aas, uint16_t x, uint16_t y, uint16_t z)
{
	//Save new params
	*(uint16_t *)&p_aas->characteristics.accelerometer.value[0] = x;
	*(uint16_t *)&p_aas->characteristics.accelerometer.value[2] = y;
	*(uint16_t *)&p_aas->characteristics.accelerometer.value[4] = z;

	//Notify it
    ble_gatts_hvx_params_t params;

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_aas->characteristics.accelerometer.handles.value_handle;
    params.p_data = p_aas->characteristics.accelerometer.value;
    params.p_len  = &p_aas->characteristics.accelerometer.value_len;

    return sd_ble_gatts_hvx(conn_handle, &params);
}

/*-- EOF --------------------------------------------------------------------*/

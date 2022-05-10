/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_bluetooth.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_bluetooth.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "boards.h"

#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"

#include "mod_indication.h"

#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"

#include "app_timer.h"
#include "ble_aas.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"


/*-- Project specific includes ----------------------------------------------*/
#include "mod_audio_player.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define DEVICE_NAME                     "Nordic TestTask"                       //Name of device. Will be included in the advertising data.

#define APP_BLE_OBSERVER_PRIO           3                                       //Application's BLE observer priority.
#define APP_BLE_CONN_CFG_TAG            1                                       //A tag identifying the SoftDevice BLE configuration.

#define APP_ADV_INTERVAL                MSEC_TO_UNITS(40, UNIT_0_625_MS)        //The advertising interval (in units of 0.625 ms; this value corresponds to 40 ms).
#define APP_ADV_DURATION                MSEC_TO_UNITS(1000, UNIT_10_MS)         //The advertising time-out (in units of seconds). When set to 0, we will never time out.
#define APP_ADV_ATTEMPT_PERIOD          APP_TIMER_TICKS(4000)                   //Time between each advertisement attempt.

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(500, UNIT_1_25_MS)        //Minimum acceptable connection interval (0.5 seconds).
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(1000, UNIT_1_25_MS)       //Maximum acceptable connection interval (1 second).
#define SLAVE_LATENCY                   0                                       //Slave latency.
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)         //Connection supervisory time-out (4 seconds).

#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(20000)                  //Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (15 seconds).
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000)                   //Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds).
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                       //Number of attempts before giving up the connection parameter negotiation.

/*-- Local Typedefs ---------------------------------------------------------*/
typedef enum
{
	BleAudioCmd_PlayTrack1 = 0x01,
	BleAudioCmd_PlayTrack2 = 0x02,
	BleAudioCmd_Stop = 0x03,
} ble_audio_comands_e;

/*-- Local function prototypes ----------------------------------------------*/
static void ble_advertising_start(void);

/*-- Local variables --------------------------------------------------------*/
static bool module_busy = false;
APP_TIMER_DEF(m_advertisement_attempt_timer_id);                                //Advertisement attempt timer id.

BLE_AAS_DEF(m_aas);                                                             //Accelerometer Audio Service instance.
NRF_BLE_GATT_DEF(m_gatt);                                                       //GATT module instance.
NRF_BLE_QWR_DEF(m_qwr);                                                         //Context for the Queued Write module.

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                        //Handle of the current connection.

static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;                   //Advertising handle used to identify an advertising set.
static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                    //Buffer for storing an encoded advertising set.
static uint8_t m_enc_scan_response_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX];         //Buffer for storing an encoded scan data.

//Struct that contains pointers to the encoded advertising data
static ble_gap_adv_data_t m_adv_data =
{
    .adv_data =
    {
        .p_data = m_enc_advdata,
        .len = BLE_GAP_ADV_SET_DATA_SIZE_MAX
    },
    .scan_rsp_data =
    {
        .p_data = m_enc_scan_response_data,
        .len = BLE_GAP_ADV_SET_DATA_SIZE_MAX

    }
};

/*-- Local functions (Callbacks) --------------------------------------------*/
/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void ble_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for handling write events to the LED characteristic.
 *
 * @param[in] p_aas     Instance of LED Button Service to which the write applies.
 * @param[in] led_state Written/desired state of the LED.
 */
static void ble_audio_write_handler(uint16_t conn_handle, ble_aas_t *p_aas, uint8_t audio_state)
{
	switch(audio_state)
	{
		case BleAudioCmd_PlayTrack1:
			Mod_AudioPlayer_Play(AudioPlayer_Track_1);
			break;

		case BleAudioCmd_PlayTrack2:
			Mod_AudioPlayer_Play(AudioPlayer_Track_2);
			break;

		case BleAudioCmd_Stop:
			Mod_AudioPlayer_Stop();
			break;

		default:
			break;
	}
}


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_event_handler(ble_evt_t const *p_ble_evt, void *p_context)
{
    ret_code_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
			Mod_Indication_SetState_Bluetooth(IndiStatus_Bluetooth_Connected);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            Mod_Indication_SetState_Bluetooth(IndiStatus_Bluetooth_Advertising);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            ble_advertising_start();
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

		case BLE_GAP_EVT_ADV_SET_TERMINATED:
			Mod_Indication_SetState_Bluetooth(IndiStatus_Bluetooth_Idle);
			//Advertisement duraction is elapsed
			if(m_conn_handle == BLE_CONN_HANDLE_INVALID)
			{
				//Start timer to new advertisement attempt
				app_timer_start(m_advertisement_attempt_timer_id, APP_ADV_ATTEMPT_PERIOD, NULL);

				//Set status
				module_busy = false;
			}
			break;

        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module that
 *          are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply
 *       setting the disconnect_on_fail config parameter, but instead we use the event
 *       handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void ble_on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void ble_conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


static void ble_advertisement_attempt_timer_timeout_handler(void * p_context)
{
	ble_advertising_start();
}

/*-- Local functions --------------------------------------------------------*/
/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
static void ble_timers_init(void)
{
    // Initialize timer module, making it use the scheduler
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

	// Create advertisements attempt timer
	app_timer_create(&m_advertisement_attempt_timer_id, APP_TIMER_MODE_SINGLE_SHOT, ble_advertisement_attempt_timer_timeout_handler);
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_event_handler, NULL);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void ble_gap_params_init(void)
{
    ret_code_t err_code;
    ble_gap_conn_params_t gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)DEVICE_NAME, strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));
    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the GATT module.
 */
static void ble_gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing services that will be used by the application.
 */
static void ble_services_init(void)
{
    ret_code_t err_code;
    
    // Initialize Queued Write Module.
	nrf_ble_qwr_init_t qwr_init = { 0 };
    qwr_init.error_handler = ble_qwr_error_handler;
    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

	//Set Accelerometer default value
	memset(&m_aas.characteristics.accelerometer.value, 0x00, AAS_ACCELEROMETER_SIZE);
	m_aas.characteristics.accelerometer.value_len = AAS_ACCELEROMETER_SIZE;

	//Set Audio default value
	m_aas.characteristics.audio.value[0] = BleAudioCmd_Stop;
	m_aas.characteristics.audio.value_len = AAS_AUDIO_SIZE;
	m_aas.characteristics.audio.write_handler = ble_audio_write_handler;


    // Initialize AAS.
    err_code = ble_aas_init(&m_aas);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
static void ble_advertising_init(void)
{
    ret_code_t err_code;
    ble_advdata_t advdata;
    ble_advdata_t srdata;

    ble_uuid_t adv_uuids[] = { { AAS_UUID_SERVICE, m_aas.service.uuid_type }};

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;


    memset(&srdata, 0, sizeof(srdata));
    srdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    srdata.uuids_complete.p_uuids  = adv_uuids;

    err_code = ble_advdata_encode(&advdata, m_adv_data.adv_data.p_data, &m_adv_data.adv_data.len);
    APP_ERROR_CHECK(err_code);

    err_code = ble_advdata_encode(&srdata, m_adv_data.scan_rsp_data.p_data, &m_adv_data.scan_rsp_data.len);
    APP_ERROR_CHECK(err_code);

    ble_gap_adv_params_t adv_params;

    // Set advertising parameters.
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.primary_phy = BLE_GAP_PHY_1MBPS;
    adv_params.duration = APP_ADV_DURATION;
    adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
    adv_params.p_peer_addr = NULL;
    adv_params.filter_policy = BLE_GAP_ADV_FP_ANY;
    adv_params.interval = APP_ADV_INTERVAL;

    err_code = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &adv_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void ble_connection_params_init(void)
{
    ret_code_t err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));
    cp_init.p_conn_params = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail = false;
    cp_init.evt_handler = ble_on_conn_params_evt;
    cp_init.error_handler = ble_conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting advertising.
 */
static void ble_advertising_start(void)
{
    ret_code_t err_code;

	//Set status
	module_busy = true;

    err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
    APP_ERROR_CHECK(err_code);

    Mod_Indication_SetState_Bluetooth(IndiStatus_Bluetooth_Advertising);
}

/*-- Exported functions -----------------------------------------------------*/
void Mod_Bluetooth_Init(void)
{
	ble_timers_init();
	ble_stack_init();
	ble_gap_params_init();
	ble_gatt_init();
	ble_services_init();
	ble_advertising_init();
	ble_connection_params_init();

	// Start execution.
    ble_advertising_start();
}

bool Mod_Bluetooth_IsBusy(void)
{
	return module_busy;
}

void Mod_Bluetooth_EnterSleepMode(void)
{

}

void Mod_Bluetooth_Accelerometer_NewData(uint16_t X, uint16_t Y, uint16_t Z)
{
	uint32_t err_code = ble_aas_on_accelerometer_new_data(m_conn_handle, &m_aas, X, Y, Z);

	if (err_code != NRF_SUCCESS &&
		err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
		err_code != NRF_ERROR_INVALID_STATE &&
		err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING &&
		err_code != NRF_ERROR_RESOURCES)  //Looks like bluetooth queue is full...
	{
		APP_ERROR_CHECK(err_code);
	}
}

void Mod_Bluetooth_Run(void)
{
	switch (Mod_AudioPlayer_GetStatus())
	{
		case AudioPlayer_Status_Stopped:
		{
			m_aas.characteristics.audio.value[0] = BleAudioCmd_Stop;
		}
		break;

		case AudioPlayer_Status_Play:
		{
			switch(Mod_AudioPlayer_GetTrack())
			{
				case AudioPlayer_Track_1:
				{
					m_aas.characteristics.audio.value[0] = BleAudioCmd_PlayTrack1;
				}
				break;

				case AudioPlayer_Track_2:
				{
					m_aas.characteristics.audio.value[0] = BleAudioCmd_PlayTrack2;
				}
				break;

				default:
				{
					m_aas.characteristics.audio.value[0] = 0xff;
				}
				break;
			}
		}
		break;

		case AudioPlayer_Status_Paused:
		{
			m_aas.characteristics.audio.value[0] = BleAudioCmd_Stop;
		}
		break;
	}
}

/*-- EOF --------------------------------------------------------------------*/

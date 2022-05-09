/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_power_management.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_power_management.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "nrf_pwr_mgmt.h"
#include "app_error.h"
#include "if_swtimer.h"

/*-- Project specific includes ----------------------------------------------*/
#include "mod_accelerometer.h"
#include "mod_indication.h"
#include "mod_audio_player.h"
#include "mod_ble.h"

/*-- Imported functions -----------------------------------------------------*/
#include "SysTick.h"

/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void Mod_PowerManagement_Init(void)
{
	ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

void Mod_PowerManagement_Run(void)
{
	if(
		(!Mod_Accelerometer_IsBusy()) &&
		(!Mod_AudioPlayer_IsBusy()) &&
		(!Mod_Ble_IsBusy()) &&
		(!Mod_Indication_IsBusy())
	)
	{
		//Prepare modules
		Mod_Accelerometer_EnterSleepMode();
		Mod_AudioPlayer_EnterSleepMode();
		Mod_Ble_EnterSleepMode();
		Mod_Indication_EnterSleepMode();

		//Set indication
		Mod_Indication_SetState_Application(IndiStatus_Application_Idle);

		//Stop systick
		SysTick_SuspendTick();

		//Stop timer
		If_SwTimer_Stop();



		//Enter sleep mode
		nrf_pwr_mgmt_run();



		//Start systick timer
		Systick_InitTick(1);

		//Resume timer
		If_SwTimer_Start();

		//Set indication
		Mod_Indication_SetState_Application(IndiStatus_Application_Run);
	}
}

/*-- EOF --------------------------------------------------------------------*/

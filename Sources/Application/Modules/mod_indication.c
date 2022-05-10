/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_indication.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_indication.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
#include "SoftwareTimer.h"

/*-- Hardware specific libraries --------------------------------------------*/
#include "SysTick.h"
#include "boards.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define LED_APP_WORKING                 BSP_BOARD_LED_0         //Is on when device in sleep mode. Blink if app is in work state.
#define LED_BT_STATE                    BSP_BOARD_LED_1         //Is on when device has connected.
#define LED_ACCEL_STATE                 BSP_BOARD_LED_2         //LED to be toggled with when accelerometer data readed.
#define LED_AUDIO_STATE                 BSP_BOARD_LED_3         //LED indicating audio player state.

/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static bool module_busy = false;
static uint32_t led_app_timer_id = 0;
static uint32_t led_bt_timer_id = 0;
static uint32_t led_accel_timer_id = 0;

/*-- Local functions --------------------------------------------------------*/
void Led_APP_Toggle_TimerCallback(void *param)
{
	bsp_board_led_invert(LED_APP_WORKING);
}

void Led_BT_Toggle_TimerCallback(void *param)
{
	bsp_board_led_invert(LED_BT_STATE);
}

void Led_Accel_Off_TimerCallback(void *param)
{
	bsp_board_led_off(LED_ACCEL_STATE);
	led_accel_timer_id = 0;

	//Set status
	module_busy = false;
}

/*-- Exported functions -----------------------------------------------------*/
void Mod_Indication_Init(void)
{
	bsp_board_init(BSP_INIT_LEDS);

	//Set status
	module_busy = false;
}

bool Mod_Indication_IsBusy(void)
{
	return module_busy;
}

void Mod_Indication_EnterSleepMode(void)
{

}

void Mod_Indication_SetState_Application(Mod_Indication_Application_status_e status)
{
	switch(status)
	{
		case IndiStatus_Application_Idle:
			if(led_app_timer_id != 0)
			{
				SwTimer_Stop(led_app_timer_id);
				led_app_timer_id = 0;
			}
			bsp_board_led_on(LED_APP_WORKING);
		break;

		case IndiStatus_Application_Run:
			if(led_app_timer_id == 0)
			{
				bsp_board_led_off(LED_APP_WORKING);
				led_app_timer_id = SwTimer_Start(SWTT_CONTINUOUS, 200, SWTP_LEVEL_LOWEST, Led_APP_Toggle_TimerCallback, NULL);
			}
		break;
	}
}

void Mod_Indication_SetState_Bluetooth(Mod_Indication_Bluetooth_status_e status)
{
	switch(status)
	{
		case IndiStatus_Bluetooth_Idle:
			if(led_bt_timer_id != 0)
			{
				SwTimer_Stop(led_bt_timer_id);
				led_bt_timer_id = 0;
			}
			bsp_board_led_off(LED_BT_STATE);
		break;

		case IndiStatus_Bluetooth_Advertising:
			if(led_bt_timer_id == 0)
			{
				bsp_board_led_off(LED_BT_STATE);
				led_bt_timer_id = SwTimer_Start(SWTT_CONTINUOUS, 50, SWTP_LEVEL_LOWEST, Led_BT_Toggle_TimerCallback, NULL);
			}
		break;

		case IndiStatus_Bluetooth_Connected:
			if(led_bt_timer_id != 0)
			{
				SwTimer_Stop(led_bt_timer_id);
				led_bt_timer_id = 0;
			}
			bsp_board_led_on(LED_BT_STATE);
		break;
	}
}

void Mod_Indication_SetState_Accelerometer(Mod_Indication_Accelerometer_status_e status)
{
	switch(status)
	{
		case IndiStatus_Accelerometer_Idle:
			if(led_accel_timer_id != 0)
			{
				SwTimer_Stop(led_accel_timer_id);
				led_accel_timer_id = 0;
			}
			bsp_board_led_off(LED_ACCEL_STATE);

			//Set status
			module_busy = false;
		break;

		case IndiStatus_Accelerometer_DataUpdated:
			if(led_accel_timer_id != 0)
			{
				SwTimer_Stop(led_accel_timer_id);
				led_accel_timer_id = 0;
			}
			bsp_board_led_on(LED_ACCEL_STATE);
			led_accel_timer_id = SwTimer_Start(SWTT_SINGLE, 50, SWTP_LEVEL_LOWEST, Led_Accel_Off_TimerCallback, NULL);

			//Set status
			module_busy = true;
		break;
	}
}

void Mod_Indication_SetState_Audio(Mod_Indication_Audio_status_e status)
{
	switch(status)
	{
		case IndiStatus_Audio_Idle:
			bsp_board_led_off(LED_AUDIO_STATE);
		break;

		case IndiStatus_Audio_Play:
			bsp_board_led_on(LED_AUDIO_STATE);
		break;

		case IndiStatus_Audio_Paused:
			bsp_board_led_on(LED_AUDIO_STATE);
		break;

		case IndiStatus_Audio_Trigger:
			bsp_board_led_invert(LED_AUDIO_STATE);
		break;
	}
}

void Mod_Indication_Run(void)
{

}

/*-- EOF --------------------------------------------------------------------*/

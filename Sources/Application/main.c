/*-- File description -------------------------------------------------------*/
/**
 *   @file:    main.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
#include "SoftwareTimer.h"

/*-- Hardware specific libraries --------------------------------------------*/
#include "if_exti.h"

/*-- Project specific includes ----------------------------------------------*/
#include "mod_indication.h"
#include "mod_climate.h"
#include "mod_swtimer.h"
#include "mod_accelerometer.h"
#include "mod_audio_player.h"
#include "mod_ble.h"

/*-- Imported functions -----------------------------------------------------*/
#include "SysTick.h"

/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
void main(void)
{
	//Start systick timer
	Systick_InitTick(1);

	//Start sw timer module
	Mod_SwTimer_Init();
	Mod_SwTimer_Start();

	//Init external interrupts from buttons
	If_Exti_Init();
	If_Exti_Enable();
	
	//Start modules
	Mod_Indication_Init();

	//Mod_Climate_Init();
	Mod_Accelerometer_Init();
	Mod_AudioPlayer_Init();
	Mod_Ble_Init();

	//Set indication
	Mod_Indication_SetStatus_Application(IndiStatus_Application_Run);

	//Main loop
    while (true)
    {
		//Mod_Climate_Run();
		Mod_Accelerometer_Run();
		Mod_AudioPlayer_Run();
		Mod_Ble_Run();

		Mod_Indication_Run();

		Mod_SwTimer_Run();
    }
}

/*-- EOF --------------------------------------------------------------------*/

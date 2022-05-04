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
#include "boards.h"
#include "if_exti.h"

/*-- Project specific includes ----------------------------------------------*/
#include "mod_climate.h"
#include "mod_swtimer.h"
#include "mod_accelerometer.h"
#include "mod_audio_player.h"

/*-- Imported functions -----------------------------------------------------*/
#include "SysTick.h"

/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
void LedToggle_TimerCallback(void *param)
{
	static uint8_t i = 0;

	bsp_board_led_invert(i);

	i++;
	if(i == (LEDS_NUMBER - 1))
	{
		i = 0;
	}
}

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

    // Configure board
    bsp_board_init(BSP_INIT_LEDS);
	
	//Start climate module
	//Mod_Climate_Init();
	Mod_Accelerometer_Init();
	Mod_AudioPlayer_Init();

	//Start Led Triggering
	SwTimer_Start(SWTT_CONTINUOUS, 125, SWTP_LEVEL_LOWEST, LedToggle_TimerCallback, NULL);


	//Main loop
    while (true)
    {
		//Mod_Climate_Run();
		Mod_Accelerometer_Run();
		Mod_AudioPlayer_Run();

		Mod_SwTimer_Run();
    }
}

/*-- EOF --------------------------------------------------------------------*/

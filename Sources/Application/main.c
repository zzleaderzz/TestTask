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

/*-- Project specific includes ----------------------------------------------*/
#include "mod_climate.h"
#include "mod_swtimer.h"

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
	if(i == LEDS_NUMBER)
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


    // Configure board
    bsp_board_init(BSP_INIT_LEDS);
	
	//Start climate module
	Mod_Climate_Init();
	

	//Start Led Triggering
	SwTimer_Start(SWTT_CONTINUOUS, 125, SWTP_LEVEL_LOWEST, LedToggle_TimerCallback, NULL);


    while (true)
    {
        //do
        //{
        //    __WFE();
        //}while (m_xfer_done == false);

		Mod_Climate_Run();

		Mod_SwTimer_Run();
    }
}

/*-- EOF --------------------------------------------------------------------*/

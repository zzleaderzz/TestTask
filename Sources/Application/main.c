/*-- File description -------------------------------------------------------*/
/**
 *   @file:    main.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: NIK El.
 */

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "boards.h"

/*-- Project specific includes ----------------------------------------------*/
#include "mod_climate.h"

/*-- Imported functions -----------------------------------------------------*/
#include "SysTick.h"

/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
void main(void)
{
	SysTick_WaitEntity_t wait_ent;

	Systick_InitTick(1);


    // Configure board
    bsp_board_init(BSP_INIT_LEDS);

	
	Mod_Climate_Init();
	

    // Toggle LEDs
    while (true)
    {
		if(SysTick_Wait(&wait_ent, 125))
		{
			static uint8_t i = 0;
			
			bsp_board_led_invert(i);

			i++;
			if(i == LEDS_NUMBER)
			{
				i = 0;
			}
		}

        //do
        //{
        //    __WFE();
        //}while (m_xfer_done == false);

		Mod_Climate_Run();
    }
}

/*-- EOF --------------------------------------------------------------------*/

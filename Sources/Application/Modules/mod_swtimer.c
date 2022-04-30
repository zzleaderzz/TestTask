/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_swtimer.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_swtimer.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "if_hwtimer0.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
#include "SoftwareTimer.h"

/*-- Local Macro Definitions ------------------------------------------------*/
#define HW_TIMER0_PERIOD                    1

/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
static void HwTimer0_Callback(uint32_t period)
{
	SwTimer_Tick(period);
}

/*-- Exported functions -----------------------------------------------------*/
void Mod_SwTimer_Init(void)
{
	If_HwTimer0_Init(HW_TIMER0_PERIOD);
	If_HwTimer0_RegisterCallback(HwTimer0_Callback);
}

void Mod_SwTimer_Start(void)
{
	If_HwTimer0_Enable();
}

void Mod_SwTimer_Stop(void)
{
	If_HwTimer0_Disable();
}

void Mod_SwTimer_Run(void)
{
	SwTimer_Run(SWTP_LEVEL_LOWEST);
}

/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
*   @file:    SoftwareTimer_api.c
*
*   @author:  valeriy.grimalskiy.
*   @company: Lab.
*/

#include "SoftwareTimer.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
//#include "TIM4.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void SwTimer_TickPause(void)
{
	//TIM4_Stop();
}

void SwTimer_TickResume(void)
{
	//TIM4_Start();
}

#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
void SwTimer_TicksConfigure(uint16_t nextTickAfter_ms)
{
	/*
	uint32_t updatePeriod_ms = TIM4_GetUpdatePeriod_ms();
	uint16_t reloadPeriod = TIM4_GetAutoreloadPeriod();
	uint16_t ticksPerMs = reloadPeriod / updatePeriod_ms;
	
	uint16_t periodNext = nextTickAfter_ms * ticksPerMs;
	
	TIM4_SetCounter(periodNext);
	*/
	
	TIM4_SetCounter(nextTickAfter_ms * (TIM4_GetAutoreloadPeriod() / TIM4_GetUpdatePeriod()));
}

uint16_t SwTimer_MilisecondsElapsedGet(void)
{
	/*
	uint32_t updatePeriod_ms = TIM4_GetUpdatePeriod_ms();
	uint16_t reloadPeriod = TIM4_GetAutoreloadPeriod();
	uint16_t ticksPerMs = reloadPeriod / updatePeriod_ms;
	
	uint16_t elapsed_ms = TIM4_GetCounter() / ticksPerMs;
	
	return elapsed_ms;
	*/
	
	return (TIM4_GetCounter() / (TIM4_GetAutoreloadPeriod() / TIM4_GetUpdatePeriod()));
}
#endif

/*-- EOF --------------------------------------------------------------------*/

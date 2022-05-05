/*-- File description -------------------------------------------------------*/
/**
*   @file:    SysTick.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "SysTick.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "nrfx.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define SYSTICK_MAX_DELAY                   0xFFFFFFFFU

/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
volatile uint32_t sysTick = 0;

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
void SysTick_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{ 
	uint32_t priorityGroup = 0x00U;

	// Check the parameters
	//assert_param(IS_NVIC_SUB_PRIORITY(SubPriority));
	//assert_param(IS_NVIC_PREEMPTION_PRIORITY(PreemptPriority));

	priorityGroup = NVIC_GetPriorityGrouping();

	NVIC_SetPriority(IRQn, NVIC_EncodePriority(priorityGroup, PreemptPriority, SubPriority));
}


uint32_t SysTick_SYSTICK_Config(uint32_t TicksNumb)
{
	return SysTick_Config(TicksNumb);
}

/*-- Exported functions -----------------------------------------------------*/
bool Systick_InitTick(uint32_t TickPriority)
{
	// Configure the SysTick to have interrupt in 1ms time basis
	if (SysTick_SYSTICK_Config(SystemCoreClock / (1000U / SYSTICK_FREQ)) > 0U)
	{
		return false;
	}

	// Configure the SysTick IRQ priority
	if (TickPriority < (1UL << __NVIC_PRIO_BITS))
	{
		SysTick_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
	}
	else
	{
		return false;
	}

	// Return function status
	return true;
}

void SysTick_IncTick(void)
{
	sysTick += (uint32_t)(SYSTICK_FREQ);
}

uint32_t SysTick_GetTick(void)
{
	return sysTick;
}

void SysTick_SuspendTick(void)
{
	// Disable SysTick Interrupt
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void SysTick_ResumeTick(void)
{
	// Enable SysTick Interrupt
	SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}

void SysTick_Delay(uint32_t delay)
{
	uint32_t tickstart = sysTick;

	// Add a freq to guarantee minimum wait
	if (delay < SYSTICK_MAX_DELAY)
	{
		delay += (uint32_t)(SYSTICK_FREQ);
	}

	while((sysTick - tickstart) < delay)
	{
		//__asm("nop");
	}
}

bool SysTick_WaitBefore(SysTick_WaitEntity_t *entity, uint32_t ms, bool repeat)
{
	if(!entity->Started)
	{
		entity->StartTime = SysTick_GetTick();
		entity->SetupMs = ms;
		entity->Started = true;

		return false;
	}
	else
	{
		if((SysTick_GetTick() - entity->StartTime) > entity->SetupMs)
		{
			if(repeat)
			{
				entity->StartTime = SysTick_GetTick();
				entity->SetupMs = ms;
				entity->Started = true;
			}
			else
			{
				entity->Started = false;
			}

			return true;
		}
		else
		{
			return false;
		}
	}
}

bool SysTick_WaitAfter(SysTick_WaitEntity_t *entity, uint32_t ms, bool repeat)
{
	if(!entity->Started)
	{
		//Start
		entity->StartTime = SysTick_GetTick();
		entity->SetupMs = ms;
		entity->Started = true;

		return true;
	}
	else
	{
		if((SysTick_GetTick() - entity->StartTime) > entity->SetupMs)
		{
			if(repeat)
			{
				//Restart
				entity->StartTime = SysTick_GetTick();
				entity->SetupMs = ms;
				entity->Started = true;
			}
			else
			{
				entity->Started = false;
			}

			return true;
		}
		else
		{
			return false;
		}
	}
}

//TODO not tested
bool SysTick_WaitTimeout(SysTick_WaitEntity_t *entity, uint32_t ms, bool exitFlag)
{
	if(!entity->Started)
	{
		entity->StartTime = SysTick_GetTick();
		entity->SetupMs = ms;
		entity->Started = true;
	}
	else
	{
		if((SysTick_GetTick() - entity->StartTime) > entity->SetupMs)
		{
			entity->Started = false;
		}
		else if (exitFlag)
		{
			//TODO Complete this
		}
	}
	
	return !entity->Started;
}

/*-- EOF --------------------------------------------------------------------*/

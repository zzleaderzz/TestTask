/*-- File description -------------------------------------------------------*/
/**
*   @file:    SysTick.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif 

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
#define SYSTICK_FREQ_10HZ                   100u
#define SYSTICK_FREQ_100HZ                  10u
#define SYSTICK_FREQ_1KHZ                   1u

#define SYSTICK_FREQ                        SYSTICK_FREQ_1KHZ

/*-- Typedefs ---------------------------------------------------------------*/
typedef struct
{
	bool Started;
	uint32_t StartTime;
	uint32_t SetupMs;
} SysTick_WaitEntity_t;

/*-- Exported variables -----------------------------------------------------*/
extern volatile uint32_t sysTick;

/*-- Exported functions -----------------------------------------------------*/
bool Systick_InitTick(uint32_t TickPriority);
void SysTick_IncTick(void);

uint32_t SysTick_GetTick(void);

void SysTick_SuspendTick(void);
void SysTick_ResumeTick(void);

void SysTick_Delay(uint32_t Delay);
bool SysTick_WaitBefore(SysTick_WaitEntity_t *entity, uint32_t ms, bool repeat);
bool SysTick_WaitAfter(SysTick_WaitEntity_t *entity, uint32_t ms, bool repeat);

bool SysTick_IsWaitElapsed(SysTick_WaitEntity_t *entity);

#ifdef __cplusplus
}
#endif

#endif // __SYSTICK_H
/*-- EOF --------------------------------------------------------------------*/

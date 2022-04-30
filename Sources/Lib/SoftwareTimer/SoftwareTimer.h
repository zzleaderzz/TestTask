/*-- File description -------------------------------------------------------*/
/**
*   @file:    SoftwareTimer.h
*
*   @author:  valeriy.grimalskiy.
*   @company: Lab.
*/

#ifndef _SW_TIMER_H
#define _SW_TIMER_H

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
#define SW_TIMER__EXTERNAL_TIMERS_SUPPORT                       0
#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
#define SW_TIMER__EXTERNAL_TIMERS_COUNT                         5
#endif

#define SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT               0
#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
#define SW_TIMER__ADVANCED_TICK_MAXIMUM_PERIOD_MS            5000
#endif

#define SW_TIMER__TIMERS_COUNT                                 25

//Up to 255 priority levels
//Can be useful for RTOS

//User must call next functions to timer work:
//SwTimer_TickCounter(n) in hwTimer interrupt handler //To tick timer with highest priority
//SwTimer_Run(n) in otner places to work with other priority levels software timers
//SwTimer_Run(SWTP_LEVEL_LOWEST) in supercycle to work with lowest priority software timers

//To avoid collision in interrupt handler, must specify next functions:
//void SwTimer_TickPause(void)
//void SwTimer_TickResume(void)

//To use advanced tick calculating, must specify next functions
//void SwTimer_TicksConfigure(uint16_t nextTickAfter_ms)
//uint16_t SwTimer_MilisecondsElapsedGet(void)


/*-- Typedefs ---------------------------------------------------------------*/
typedef void (*SwTimerCallback_t)(void *);

typedef enum
{
	SWTT_CONTINUOUS = 0,
	SWTT_SINGLE = 1,
}SwTimerType_t;

typedef enum
{
	SWTP_LEVEL_HIGHEST = 0,
	SWTP_LEVEL_LOWEST = 0xFF,
}SwTimerPriority_t;

typedef enum
{
	SWTS_CLEAR = 0,
	SWTS_CONFIGURING,
	SWTS_RUNNING,
	SWTS_PAUSED,
	SWTS_WAIT_CALLBACK_CALL,
	SWTS_RUNNING_DELAY,
	SWTS_CLEAR_DELAY,
}SwTimerState_t;

typedef struct
{
	SwTimerState_t      State;
	uint32_t            Id;
	SwTimerPriority_t   Priority;
	SwTimerType_t       WorkCountSetup;
	uint32_t            WorkCount;
	uint32_t            StartDelay;
	uint32_t            Period;
	uint32_t            Ticks;
	SwTimerCallback_t   CallbackFunction;
	void *              CallbackParameter;
}SwTimer_t;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void SwTimer_Init(void);
bool SwTimer_IsPresent(uint32_t id);
uint32_t SwTimer_Start(uint32_t work_count, uint32_t period_ms, uint8_t priority, SwTimerCallback_t callback, void *parameter);
uint32_t SwTimer_StartDelayed(uint32_t work_count, uint32_t start_delay_ms, uint32_t period_ms, uint8_t priority, SwTimerCallback_t callback, void *parameter);
void SwTimer_Pause(uint32_t id);
void SwTimer_PauseFor(uint32_t id, uint32_t delay_ms);
void SwTimer_Resume(uint32_t id);
void SwTimer_Stop(uint32_t id);
void SwTimer_Refresh(uint32_t id);
void SwTimer_Reload(uint32_t id, uint32_t period_ms);
void SwTimer_Delay(uint32_t period_ms);

#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
bool SwTimer_External_IsPresent(uint32_t *variable);
bool SwTimer_External_Start(uint32_t *variable);
void SwTimer_External_Stop(uint32_t *variable);
#endif

void SwTimer_Tick(uint32_t ms_per_tick);
void SwTimer_Run(uint8_t priority);

#ifdef __cplusplus
}
#endif

#endif // _SW_TIMER_H
/*-- EOF --------------------------------------------------------------------*/

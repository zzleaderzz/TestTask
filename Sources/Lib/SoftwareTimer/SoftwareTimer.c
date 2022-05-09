/*-- File description -------------------------------------------------------*/
/**
*   @file:    SoftwareTimer.c
*
*   @author:  valeriy.grimalskiy.
*   @company: Lab.
*/

#include "SoftwareTimer.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
#include "defs.h"

/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint32_t _idCounter = 0;
static SwTimer_t _swTimer[SW_TIMER__TIMERS_COUNT];

#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
static uint32_t *_swTimer_ext[SW_TIMER__EXTERNAL_TIMERS_COUNT] = { 0 };
#endif

/*-- Local function prototypes ----------------------------------------------*/
static inline void SwTimer_FreeTimer(uint8_t index);
static inline void SwTimer_RunTimerCalback(uint8_t index);

//User to specify
__WEAK void SwTimer_TickPause(void);
__WEAK void SwTimer_TickResume(void);
#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
static inline void SwTimer_TicksCalculate(void);
__WEAK void SwTimer_TicksConfigure(uint16_t nextTickAfter_ms);
__WEAK uint16_t SwTimer_MilisecondsElapsedGet(void);
#endif

/*-- Local functions --------------------------------------------------------*/
static inline void SwTimer_FreeTimer(uint8_t index)
{
	_swTimer[index].Parameter = 0;
	_swTimer[index].CallbackFunction = 0;
	_swTimer[index].Ticks = 0;
	_swTimer[index].Period = 0;
	_swTimer[index].StartDelay = 0;
	_swTimer[index].WorkCount = 0;
	_swTimer[index].WorkCountSetup = (SwTimerType_t)0;
	_swTimer[index].Priority = (SwTimerPriority_t)0;
	_swTimer[index].Id = 0;
	_swTimer[index].State = SWTS_CLEAR; //Must be last peration
}

static inline void SwTimer_RunTimerCalback(uint8_t index)
{
	if(_swTimer[index].CallbackFunction)
	{
		_swTimer[index].CallbackFunction(_swTimer[index].Parameter);
	}
	
	//Restart if timer is continuous
	if(_swTimer[index].WorkCountSetup == SWTT_CONTINUOUS)
	{
		//Reload timer
		_swTimer[index].Ticks = _swTimer[index].Period;
		_swTimer[index].State = SWTS_RUNNING;
		
	#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
		//Calculate next tick
		SwTimer_TicksCalculate();
	#endif
	}
	else if(_swTimer[index].WorkCount > 1)
	{
		//Reload timer
		_swTimer[index].Ticks = _swTimer[index].Period;
		_swTimer[index].State = SWTS_RUNNING;
		
		//Decrease work count
		_swTimer[index].WorkCount--;
		
	#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
		//Calculate next tick
		SwTimer_TicksCalculate();
	#endif
	}
	else
	{
		//Free Timer
		SwTimer_FreeTimer(index);
	}
}

#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
static void SwTimer_TicksCalculate(void)
{
	uint8_t i = 0;
	uint16_t nextTickAfter_ms = SW_TIMER__ADVANCED_TICK_MAXIMUM_PERIOD_MS;
	uint16_t elapsedTicks_ms = SwTimer_MilisecondsElapsedGet();
	
#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
	bool foundExternal = false;
	
	for(i = 0; i < SW_TIMER__EXTERNAL_TIMERS_COUNT; i++)
	{
		if(_swTimer_ext[i])
		{
			foundExternal = true;
			break;
		}
	}

	if(foundExternal)
	{
		nextTickAfter_ms = 1;
	}
	else
	{
#endif
		//Find smallest value of ticks
		for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
		{
			if(_swTimer[i].State == SWTS_RUNNING)
			{
				if(_swTimer[i].StartDelay > 0)
				{
					if(_swTimer[i].StartDelay < nextTickAfter_ms)
					{
						nextTickAfter_ms = _swTimer[i].StartDelay;
					}
				}
				else
				{
					if(_swTimer[i].Ticks < nextTickAfter_ms)
					{
						nextTickAfter_ms = _swTimer[i].Ticks;
					}
				}
			}
		}
#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
	}
#endif
	SwTimer_TicksConfigure(nextTickAfter_ms);
}
#endif

static inline uint32_t SwTimer_IncreaseId(void)
{
	_idCounter++;	
	
	//To avoid 0 id
	if(_idCounter == 0)
	{
		_idCounter++;
	}
	
	return _idCounter;
}

static inline uint32_t SwTimer_CalculateNewId(void)
{
	uint32_t id = SwTimer_IncreaseId();
	
	bool exitFlag = false;
	while (!exitFlag)
	{
		uint8_t i = 0;
		for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
		{
			if(_swTimer[i].State != SWTS_CLEAR)
			{
				if(_swTimer[i].Id == _idCounter)
				{
					id = SwTimer_IncreaseId();
					
					exitFlag = false;
					break;
				}
				else
				{
					exitFlag = true;
				}
			}
			else
			{
				exitFlag = true;
			}
		}
	}
	
	return id;
}

/*-- Exported functions -----------------------------------------------------*/
__WEAK void SwTimer_TickPause(void)
{
	
}

__WEAK void SwTimer_TickResume(void)
{
	
}

#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
__WEAK void SwTimer_TicksConfigure(uint16_t nextTickAfter_ms)
{
	
}

__WEAK uint16_t SwTimer_MilisecondsElapsedGet(void)
{
	return 0;
}
#endif

void SwTimer_Init(void)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		SwTimer_FreeTimer(i);
	}
	
#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
	for(i = 0; i < SW_TIMER__EXTERNAL_TIMERS_COUNT; i++)
	{
		_swTimer_ext[i] = 0;
	}
#endif
	
#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
	//Calculate next tick
	SwTimer_TicksCalculate();
#endif
}

bool SwTimer_IsPresent(uint32_t id)
{
	bool result = false;
	uint8_t i = 0;
	
	if(id > 0)
	{
		for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
		{
			if((_swTimer[i].Id == id) && ((_swTimer[i].WorkCountSetup == SWTT_CONTINUOUS) || (_swTimer[i].State == SWTS_RUNNING) || (_swTimer[i].State == SWTS_PAUSED)))
			{
				result = true;
				break;
			}
		}
	}
	
	return result;
}

uint32_t SwTimer_StartDelayed(uint8_t workCount, uint32_t startDelay_ms, uint32_t period_ms, uint8_t priority, SwTimerCallback_t callback, void *parameter)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if(_swTimer[i].State == SWTS_CLEAR)
		{
			_swTimer[i].Parameter = parameter;
			_swTimer[i].CallbackFunction = callback;
			_swTimer[i].Ticks = period_ms;
			_swTimer[i].Period = period_ms;
			_swTimer[i].StartDelay = startDelay_ms;
			_swTimer[i].WorkCount = workCount;
			_swTimer[i].WorkCountSetup = (SwTimerType_t)workCount;
			_swTimer[i].Priority = (SwTimerPriority_t)priority;
			_swTimer[i].Id = SwTimer_CalculateNewId();
			
			//Mark timer ready to work
			_swTimer[i].State = SWTS_RUNNING; //Must be last peration
			
		#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
			//Calculate next tick
			SwTimer_TicksCalculate();
		#endif
			
			//Return id
			return _swTimer[i].Id;
		}
	}

	return 0;
}

uint32_t SwTimer_Start(uint8_t workCount, uint32_t period_ms, uint8_t priority, SwTimerCallback_t callback, void *parameter)
{
	return SwTimer_StartDelayed(workCount, 0, period_ms, priority, callback, parameter);
}

void SwTimer_Pause(uint32_t id)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if((_swTimer[i].Id == id) && ((_swTimer[i].WorkCountSetup == SWTT_CONTINUOUS) || (_swTimer[i].State == SWTS_RUNNING)))
		{
			//Pause timer
			_swTimer[i].State = SWTS_PAUSED;
			break;
		}
	}
}

void SwTimer_PauseFor(uint32_t id, uint32_t delay_ms)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if((_swTimer[i].Id == id) && ((_swTimer[i].WorkCountSetup == SWTT_CONTINUOUS) || (_swTimer[i].State == SWTS_RUNNING)))
		{
			//Add pause ticks ms to timer
			_swTimer[i].Ticks += delay_ms;
			
			if(_swTimer[i].WorkCountSetup == SWTT_CONTINUOUS)
			{
				_swTimer[i].State = SWTS_RUNNING;
			}
			
			break;
		}
	}
}

void SwTimer_Resume(uint32_t id)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if((_swTimer[i].Id == id) && (_swTimer[i].State == SWTS_PAUSED))
		{
			//Resume timer
			if((_swTimer[i].WorkCountSetup == SWTT_CONTINUOUS) && (_swTimer[i].Priority != SWTP_LEVEL_HIGHEST) && (_swTimer[i].Ticks == 0))
			{
				_swTimer[i].State = SWTS_WAIT_CALLBACK_CALL;
			}
			else
			{
				_swTimer[i].State = SWTS_RUNNING;
			}
			break;
		}
	}
}

void SwTimer_Stop(uint32_t id)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if((_swTimer[i].Id == id) && ((_swTimer[i].State == SWTS_RUNNING) || (_swTimer[i].State == SWTS_PAUSED)))
		{
			//Free timer
			SwTimer_FreeTimer(i);
			break;
		}
	}
}

void SwTimer_Refresh(uint32_t id)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if((_swTimer[i].Id == id) && ((_swTimer[i].State == SWTS_RUNNING) || (_swTimer[i].State == SWTS_PAUSED)))
		{
			_swTimer[i].Ticks = _swTimer[i].Period;
			break;
		}
	}
}

void SwTimer_Reload(uint32_t id, uint32_t period_ms)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if(_swTimer[i].Id == id)
		{
			_swTimer[i].Ticks = period_ms;
			_swTimer[i].Period = period_ms;
			break;
		}
	}
}

void SwTimer_Delay(uint32_t period_ms)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if(_swTimer[i].State == SWTS_CLEAR)
		{
			_swTimer[i].Parameter = 0;
			_swTimer[i].CallbackFunction = 0;
			_swTimer[i].Ticks = period_ms;
			_swTimer[i].Period = period_ms;
			_swTimer[i].StartDelay = 0;
			_swTimer[i].WorkCount = SWTT_SINGLE;
			_swTimer[i].WorkCountSetup = SWTT_SINGLE;
			_swTimer[i].Priority = SWTP_LEVEL_HIGHEST;
			_swTimer[i].Id = SwTimer_CalculateNewId();
			
			//Mark timer ready to work
			_swTimer[i].State = SWTS_RUNNING; //Must be last peration
			
		#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
			//Calculate next tick
			SwTimer_TicksCalculate();
		#endif
			
			break;
		}
	}
	
	//Wait delay
	while(_swTimer[i].State != SWTS_CLEAR);
}

//---------------------------------------------------------------------------

#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
bool SwTimer_External_IsPresent(uint32_t *variable)
{
	bool result = false;
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__EXTERNAL_TIMERS_COUNT; i++)
	{
		if(_swTimer_ext[i] == variable)
		{
			result = true;
			break;
		}
	}
	
	return result;
}

bool SwTimer_External_Start(uint32_t *variable)
{
	bool result = false;
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__EXTERNAL_TIMERS_COUNT; i++)
	{
		if(_swTimer_ext[i] == 0)
		{
			_swTimer_ext[i] = variable;
			
		#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
			//Calculate next tick
			SwTimer_TicksCalculate();
		#endif
			
			result = true;
			break;
		}
	}
	
	return result;
}

void SwTimer_External_Stop(uint32_t *variable)
{
	uint8_t i = 0;
	
	for(i = 0; i < SW_TIMER__EXTERNAL_TIMERS_COUNT; i++)
	{
		if(_swTimer_ext[i] == variable)
		{
			_swTimer_ext[i] = 0;
			break;
		}
	}
}
#endif

//---------------------------------------------------------------------------

void SwTimer_TickCounter(uint32_t msPerTick)
{
	//Pause ticks
	SwTimer_TickPause();
	
	uint8_t i = 0;
	
	#if (SW_TIMER__EXTERNAL_TIMERS_SUPPORT)
	for(i = 0; i < SW_TIMER__EXTERNAL_TIMERS_COUNT; i++)
	{
		if(_swTimer_ext[i])
		{
			if((*_swTimer_ext[i]) > msPerTick)
			{
				(*_swTimer_ext[i]) -= msPerTick;
			}
			else
			{
				*_swTimer_ext[i] = 0;
				_swTimer_ext[i] = 0;
			}
		}
	}
	#endif
	
	for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
	{
		if(_swTimer[i].State != SWTS_RUNNING)
		{
			continue;
		}
		
		if(_swTimer[i].StartDelay > msPerTick)
		{
			_swTimer[i].StartDelay -= msPerTick;
		}
		else if (_swTimer[i].StartDelay > 0)
		{
			_swTimer[i].StartDelay = 0;
		}
		else
		{
			if (_swTimer[i].Ticks > msPerTick)
			{
				_swTimer[i].Ticks -= msPerTick;
			}
			else
			{
				_swTimer[i].Ticks = 0;
				
				//Call callback if it is a timer with a highest priority
				if(_swTimer[i].Priority == SWTP_LEVEL_HIGHEST)
				{
					SwTimer_RunTimerCalback(i);
				}
				else
				{
					_swTimer[i].State = SWTS_WAIT_CALLBACK_CALL;
				}
			}
		}
	}
	
#if (SW_TIMER__ADVANCED_TICK_CALCULATE_SUPPORT)
	//Recalculate next period
	SwTimer_TicksCalculate();
#endif
	
	//Resume ticks
	SwTimer_TickResume();
}

void SwTimer_Run(uint8_t priority)
{
	if(priority > SWTP_LEVEL_HIGHEST)
	{
		uint8_t i = 0;

		for(i = 0; i < SW_TIMER__TIMERS_COUNT; i++)
		{
			if(_swTimer[i].State != SWTS_WAIT_CALLBACK_CALL)
			{
				continue;
			}
			else
			{
				if(_swTimer[i].Priority == priority)
				{
					SwTimer_RunTimerCalback(i);
				}
			}
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

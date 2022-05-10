/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_hwtimer.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "if_hwtimer.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "nrf_drv_timer.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static const nrf_drv_timer_t timer = NRF_DRV_TIMER_INSTANCE(1);
static uint32_t time_period_ms = 1; //Time(in miliseconds) between consecutive compare events.
static If_HwTimer_Callback timer_callback = 0;

/*-- Local functions --------------------------------------------------------*/
void HwTimer_Event_Handler(nrf_timer_event_t event_type, void* p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
			if(timer_callback)
			{
				timer_callback(time_period_ms);
			}
            break;

        default:
            //Do nothing.
            break;
    }
}

/*-- Exported functions -----------------------------------------------------*/
void If_HwTimer_Init(uint32_t period_ms)
{
    uint32_t time_ticks = 0;
    uint32_t err_code = NRF_SUCCESS;

	//Set period
	if(period_ms > 0)
	{
		time_period_ms = period_ms;
	}

	//Init timer
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&timer, &timer_cfg, HwTimer_Event_Handler);
    APP_ERROR_CHECK(err_code);

	//Configure timer
    time_ticks = nrf_drv_timer_ms_to_ticks(&timer, time_period_ms);
    nrf_drv_timer_extended_compare(&timer, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
}

void If_HwTimer_DeInit(void)
{
	nrf_drv_timer_uninit(&timer);
}

void If_HwTimer_RegisterCallback(If_HwTimer_Callback callback)
{
	timer_callback = callback;
}

void If_HwTimer_Enable(void)
{
	nrf_drv_timer_enable(&timer);
}

void If_HwTimer_Disable(void)
{
	nrf_drv_timer_disable(&timer);
}

/*-- EOF --------------------------------------------------------------------*/

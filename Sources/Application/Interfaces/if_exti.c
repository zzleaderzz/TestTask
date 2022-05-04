/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_exti.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "if_exti.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "boards.h"
#include "nrf_drv_gpiote.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static If_Exti_Callback exti_callback_1 = 0;
static If_Exti_Callback exti_callback_2 = 0;
static If_Exti_Callback exti_callback_3 = 0;
static If_Exti_Callback exti_callback_4 = 0;

/*-- Local functions --------------------------------------------------------*/
void Exti_interrupt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	switch(pin)
	{
		case BUTTON_1:
		{
			if(exti_callback_1)
			{
				exti_callback_1();
			}
		}
		break;

		case BUTTON_2:
		{
			if(exti_callback_2)
			{
				exti_callback_2();
			}
		}
		break;

		case BUTTON_3:
		{
			if(exti_callback_3)
			{
				exti_callback_3();
			}
		}
		break;

		case BUTTON_4:
		{
			if(exti_callback_4)
			{
				exti_callback_4();
			}
		}
		break;

		default:
		break;
	}
}

/*-- Exported functions -----------------------------------------------------*/
void If_Exti_Init(void)
{
	ret_code_t err_code;
	err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

	//Config trigger
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
	
	//Init gpio
    err_code = nrf_drv_gpiote_in_init(BUTTON_1, &in_config, Exti_interrupt_handler);
	err_code = nrf_drv_gpiote_in_init(BUTTON_2, &in_config, Exti_interrupt_handler);
	err_code = nrf_drv_gpiote_in_init(BUTTON_3, &in_config, Exti_interrupt_handler);
	err_code = nrf_drv_gpiote_in_init(BUTTON_4, &in_config, Exti_interrupt_handler);
    APP_ERROR_CHECK(err_code);
}

void If_Exti_DeInit(void)
{
	nrf_drv_gpiote_in_uninit(BUTTON_1);
	nrf_drv_gpiote_in_uninit(BUTTON_2);
	nrf_drv_gpiote_in_uninit(BUTTON_3);
	nrf_drv_gpiote_in_uninit(BUTTON_4);
}

void If_Exti_RegisterCallback(If_Exti_pin_e pin, If_Exti_Callback callback)
{
	switch(pin)
	{
		case If_Exti_Button_1:
		{
			exti_callback_1 = callback;
		}
		break;

		case If_Exti_Button_2:
		{
			exti_callback_2 = callback;
		}
		break;

		case If_Exti_Button_3:
		{
			exti_callback_3 = callback;
		}
		break;

		case If_Exti_Button_4:
		{
			exti_callback_4 = callback;
		}
		break;

		default:
		break;
	}
}

void If_Exti_Enable(void)
{
	//Enable interrupt
    nrf_drv_gpiote_in_event_enable(BUTTON_1, true);
	nrf_drv_gpiote_in_event_enable(BUTTON_2, true);
	nrf_drv_gpiote_in_event_enable(BUTTON_3, true);
	nrf_drv_gpiote_in_event_enable(BUTTON_4, true);
}

void If_Exti_Disable(void)
{
	//Disable interrupt
    nrf_drv_gpiote_in_event_disable(BUTTON_1);
	nrf_drv_gpiote_in_event_disable(BUTTON_2);
	nrf_drv_gpiote_in_event_disable(BUTTON_3);
	nrf_drv_gpiote_in_event_disable(BUTTON_4);
}

/*-- EOF --------------------------------------------------------------------*/

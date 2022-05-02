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
//#define EXTI_GPIO_IN                        ARDUINO_AREF_PIN //No connection interrupt pins on accel6 pcb
#define EXTI_GPIO_IN                        BSP_BUTTON_0

/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static If_Exti_Callback exti_callback = 0;

/*-- Local functions --------------------------------------------------------*/
void Exti_interrupt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	if(exti_callback)
	{
		exti_callback();
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
    err_code = nrf_drv_gpiote_in_init(EXTI_GPIO_IN, &in_config, Exti_interrupt_handler);
    APP_ERROR_CHECK(err_code);
}

void If_Exti_DeInit(void)
{
	nrf_drv_gpiote_in_uninit(EXTI_GPIO_IN);
}

void If_Exti_RegisterCallback(If_Exti_Callback callback)
{
	exti_callback = callback;
}

void If_Exti_Enable(void)
{
	//Enable interrupt
    nrf_drv_gpiote_in_event_enable(EXTI_GPIO_IN, true);
}

void If_Exti_Disable(void)
{
	//Disable interrupt
    nrf_drv_gpiote_in_event_disable(EXTI_GPIO_IN);
}

/*-- EOF --------------------------------------------------------------------*/

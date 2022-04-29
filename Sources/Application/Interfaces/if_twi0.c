/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_twi0.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: NIK El.
 */

#include "if_twi0.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "boards.h"
#include "nrf_drv_twi.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define I2C_INSTANCE_ID						0

/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static const nrf_drv_twi_t twi = NRF_DRV_TWI_INSTANCE(I2C_INSTANCE_ID);
static const nrf_drv_twi_config_t twi_config = 
{
   .scl                = ARDUINO_SCL_PIN,		  //P0.27
   .sda                = ARDUINO_SDA_PIN,         //P0.26
   .frequency          = NRF_DRV_TWI_FREQ_100K,
   .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
   .clear_bus_init     = false
};

static volatile bool is_transfer_completed = false;

/*-- Local functions --------------------------------------------------------*/
void TWI_Handler(nrf_drv_twi_evt_t const *event, void *context)
{
    switch (event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            is_transfer_completed = true;
            break;

        default:
            break;
    }
}

/*-- Exported functions -----------------------------------------------------*/
void If_TWI0_Init(void)
{
    ret_code_t err_code = nrf_drv_twi_init(&twi, &twi_config, TWI_Handler, NULL);
    APP_ERROR_CHECK(err_code);
}

void If_TWI0_DeInit(void)
{
	nrf_drv_twi_uninit(&twi);
}

void If_TWI0_Enable(void)
{
	nrf_drv_twi_enable(&twi);
}

void If_TWI0_Disable(void)
{
	nrf_drv_twi_disable(&twi);
}

bool If_TWI0_Send(uint8_t address, uint8_t *data, uint8_t length)
{
	is_transfer_completed = false;

	ret_code_t err_code = nrf_drv_twi_tx(&twi, address, data, length, false);
	APP_ERROR_CHECK(err_code);

	while(nrf_drv_twi_is_busy(&twi));

    //while (is_transfer_completed == false);
}

bool If_TWI0_Receive(uint8_t address, uint8_t *data, uint8_t length)
{
	is_transfer_completed = false;

	ret_code_t err_code = nrf_drv_twi_rx(&twi, address, data, length);
	APP_ERROR_CHECK(err_code);

	while(nrf_drv_twi_is_busy(&twi));

	//while (is_transfer_completed == false);
}

/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_i2s.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "if_i2s.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "boards.h"
#include "nrf_drv_i2s.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
//#define IF_I2S_DATA_BLOCK_WORDS             10240
#define IF_I2S_DATA_BLOCK_WORDS             1024

/*-- Local Typedefs ---------------------------------------------------------*/
typedef enum
{
	If_I2S_Buffer_0,
	If_I2S_Buffer_1,
	If_I2S_Buffer_Count
} If_I2S_BufferNumber_e;

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static If_I2S_Buffer_item_t if_i2s_buffer_tx[If_I2S_Buffer_Count][IF_I2S_DATA_BLOCK_WORDS];
static bool if_i2s_first_update = true;
static const nrf_drv_i2s_buffers_t if_i2s_initial_buffers = {
	.p_tx_buffer = (uint32_t *)if_i2s_buffer_tx[If_I2S_Buffer_0],
	.p_rx_buffer = NULL
};

static const nrf_drv_i2s_buffers_t if_i2s_next_buffers = {
	.p_tx_buffer = (uint32_t *)if_i2s_buffer_tx[If_I2S_Buffer_1],
	.p_rx_buffer = NULL
};

static If_I2S_Buffer_item_t *i2s_buffer_to_fill = NULL;
static If_I2S_Callback i2s_callback = NULL;

/*-- Local functions --------------------------------------------------------*/
static void I2S_EventHandler(nrf_drv_i2s_buffers_t const * p_released, uint32_t status)
{
	// When the handler is called after the transfer has been stopped
	// (no next buffers are needed, only the used buffers are to be
	// released), there is nothing to do.
	if (!(status & NRFX_I2S_STATUS_NEXT_BUFFERS_NEEDED))
	{
		return;
	}
	
	if(if_i2s_first_update)
	{
		APP_ERROR_CHECK(nrf_drv_i2s_next_buffers_set(&if_i2s_next_buffers));
		if_i2s_first_update = false;

		i2s_buffer_to_fill = (If_I2S_Buffer_item_t *)if_i2s_buffer_tx[If_I2S_Buffer_1];
	}
	else
	{
		APP_ERROR_CHECK(nrf_drv_i2s_next_buffers_set(p_released));

		i2s_buffer_to_fill = (If_I2S_Buffer_item_t *)p_released->p_tx_buffer;
	}
}

/*-- Exported functions -----------------------------------------------------*/
void If_I2S_Init(If_I2S_Callback callback)
{
	if(callback)
	{
		nrf_drv_i2s_config_t config;

		config.sck_pin      = ARDUINO_A5_PIN; //P0.31
	    config.lrck_pin     = ARDUINO_A4_PIN; //P0.30
	    config.mck_pin      = 0xFF;
	    config.sdout_pin    = ARDUINO_A3_PIN; //P0.29
	    config.sdin_pin     = 0xFF; //P0.28
	    config.irq_priority = 5;
	    config.mode         = NRF_I2S_MODE_MASTER;
	    config.format       = NRF_I2S_FORMAT_I2S;
	    config.alignment    = NRF_I2S_ALIGN_LEFT;
	    config.sample_width = NRF_I2S_SWIDTH_16BIT;
		config.channels     = NRF_I2S_CHANNELS_STEREO;

		//Normal Sound quality (15625 Hz)
	    config.mck_setup    = NRF_I2S_MCK_32MDIV8;
	    config.ratio        = NRF_I2S_RATIO_256X;

		//Normal Sound quality but fast playing (16666 Hz)
	    //config.mck_setup    = NRF_I2S_MCK_32MDIV10;
	    //config.ratio        = NRF_I2S_RATIO_192X;

		//Bad Sound quality (15873 Hz)
		//config.mck_setup = NRF_I2S_MCK_32MDIV21;
		//config.ratio     = NRF_I2S_RATIO_96X;

		APP_ERROR_CHECK(nrf_drv_i2s_init(&config, I2S_EventHandler));

		i2s_callback = callback;
	}
}

void If_I2S_DeInit(void)
{
	nrfx_i2s_uninit();
}


void If_I2S_Enable(void)
{
	if(i2s_callback)
	{
		uint32_t err_code = NRF_SUCCESS;
	
		if(i2s_callback)
		{
			i2s_callback(if_i2s_buffer_tx[If_I2S_Buffer_0], IF_I2S_DATA_BLOCK_WORDS);
		}

		if_i2s_first_update = true;

		err_code = nrf_drv_i2s_start(&if_i2s_initial_buffers, IF_I2S_DATA_BLOCK_WORDS, 0);
		APP_ERROR_CHECK(err_code);

		i2s_buffer_to_fill = (If_I2S_Buffer_item_t *)if_i2s_buffer_tx[If_I2S_Buffer_1];
	}
}

void If_I2S_Disable(void)
{
	nrf_drv_i2s_stop();
}

void If_I2S_Run(void)
{
	if(i2s_buffer_to_fill)
	{
		if(i2s_callback)
		{
			i2s_callback(i2s_buffer_to_fill, IF_I2S_DATA_BLOCK_WORDS);
		}

		i2s_buffer_to_fill = NULL;
	}
}

/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_audio_player.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_audio_player.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "if_i2s.h"
#include "if_exti.h"
#include "boards.h"

/*-- Project specific includes ----------------------------------------------*/
#include "mod_audio_tracks.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint16_t *track_data = 0;
static uint32_t track_data_length = 0;
static uint32_t track_index_counter = 0;
static AudioPlayer_Status_e player_status = AudioPlayer_Status_Stopped;
static bool pause_requested = false;

/*-- Local functions --------------------------------------------------------*/
static void I2S_BufferFillNeededCallback(If_I2S_Buffer_item_t *buffer, const uint32_t max_size)
{
	if (pause_requested)
	{
		pause_requested = false;

		//Stop playing
		If_I2S_Disable();

		player_status = AudioPlayer_Status_Paused;

		bsp_board_led_off(3);
	}
	else
	{
		if(track_index_counter == track_data_length)
		{
			track_index_counter = 0;

			//Stop playing
			If_I2S_Disable();

			player_status = AudioPlayer_Status_Stopped;

			bsp_board_led_off(3);
		}
		else
		{
			for (uint16_t i = 0; i < max_size; ++i)
			{
				If_I2S_Buffer_item_t *p_word = &buffer[i];

				if(track_index_counter < track_data_length)
				{
					//p_word->ch_left = track_data[track_index_counter];
					p_word->ch_right = track_data[track_index_counter];
					track_index_counter++;
				}
				else
				{
					//p_word->ch_left = 0x0000;
					p_word->ch_right = 0x0000;
				}
			}

			bsp_board_led_invert(3);
		}
	}

	
}

static void AudioPlayer_Button2_Callback(void)
{
	static AudioPlayer_Track_e current_track = AudioPlayer_Track_1;

	Mod_AudioPlayer_Play(current_track);

	current_track++;

	if(current_track > AudioPlayer_Track_2)
	{
		current_track = AudioPlayer_Track_1;
	}
}

static void AudioPlayer_Button3_Callback(void)
{
	Mod_AudioPlayer_Stop();
}

static void AudioPlayer_Button4_Callback(void)
{
	if(player_status == AudioPlayer_Status_Play)
	{
		Mod_AudioPlayer_Pause();
	}
	else if (player_status == AudioPlayer_Status_Paused)
	{
		Mod_AudioPlayer_Resume();
	}
}

/*-- Exported functions -----------------------------------------------------*/
void Mod_AudioPlayer_Init(void)
{
	If_I2S_Init(I2S_BufferFillNeededCallback);

	If_Exti_RegisterCallback(If_Exti_Button_2, AudioPlayer_Button2_Callback);
	If_Exti_RegisterCallback(If_Exti_Button_3, AudioPlayer_Button3_Callback);
	If_Exti_RegisterCallback(If_Exti_Button_4, AudioPlayer_Button4_Callback);
}

AudioPlayer_Status_e Mod_AudioPlayer_GetStatus(void)
{
	return player_status;
}

void Mod_AudioPlayer_Play(AudioPlayer_Track_e track)
{
	if(player_status == AudioPlayer_Status_Stopped)
	{
		switch(track)
		{
			case AudioPlayer_Track_1:
			{
				track_data = (uint16_t *)Melody_U;
				track_data_length = (MELODY_U_SIZE / sizeof(uint16_t));
			}
			break;

			case AudioPlayer_Track_2:
			{
				track_data = (uint16_t *)Melody_Bergen;
				track_data_length = (MELODY_BERGEN_SIZE / sizeof(uint16_t));
			}
			break;

			default:
			{
				return;
			}
			break;
		}

		track_index_counter = 0;

		player_status = AudioPlayer_Status_Play;

		//Start playing
		If_I2S_Enable();
	}
}

void Mod_AudioPlayer_Pause(void)
{
	if(player_status == AudioPlayer_Status_Play)
	{
		pause_requested = true;
	}
}

void Mod_AudioPlayer_Resume(void)
{
	if(player_status == AudioPlayer_Status_Paused)
	{
		player_status = AudioPlayer_Status_Play;

		//Start playing
		If_I2S_Enable();
	}
}

void Mod_AudioPlayer_Stop(void)
{
	if(player_status == AudioPlayer_Status_Play)
	{
		//Stop playing
		If_I2S_Disable();

		player_status = AudioPlayer_Status_Stopped;

		bsp_board_led_off(3);
	}
	else if (player_status == AudioPlayer_Status_Paused)
	{
		player_status = AudioPlayer_Status_Stopped;
	}
}

void Mod_AudioPlayer_Tick(uint32_t ms)
{

}

void Mod_AudioPlayer_Run(void)
{
	If_I2S_Run();
}

/*-- EOF --------------------------------------------------------------------*/

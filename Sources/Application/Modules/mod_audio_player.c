/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_audio_player.c
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#include "mod_audio_player.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "if_i2s.h"
#include "if_exti.h"

/*-- Project specific includes ----------------------------------------------*/
#include "mod_indication.h"
#include "mod_audio_tracks.h"

/*-- Imported functions -----------------------------------------------------*/
#include "SysTick.h"

/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static bool module_busy = false;
static SysTick_WaitEntity_t button2_debounce_entity;
static SysTick_WaitEntity_t button3_debounce_entity;
static SysTick_WaitEntity_t button4_debounce_entity;

static uint16_t *track_data = 0;
static uint32_t track_data_length = 0;
static uint32_t track_index_counter = 0;
static AudioPlayer_Status_e player_status = AudioPlayer_Status_Stopped;
static AudioPlayer_Track_e player_track = AudioPlayer_Track_1;
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

		Mod_Indication_SetState_Audio(IndiStatus_Audio_Paused);
	}
	else
	{
		if(track_index_counter == track_data_length)
		{
			track_index_counter = 0;

			//Stop playing
			If_I2S_Disable();

			player_track = AudioPlayer_Track_None;
			player_status = AudioPlayer_Status_Stopped;

			Mod_Indication_SetState_Audio(IndiStatus_Audio_Idle);
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

			Mod_Indication_SetState_Audio(IndiStatus_Audio_Trigger);
		}
	}

	
}

static void AudioPlayer_Button2_Callback(void)
{
	if(SysTick_WaitAfter(&button2_debounce_entity, 250, true))
	{
		static AudioPlayer_Track_e current_track = AudioPlayer_Track_1;

		Mod_AudioPlayer_Play(current_track);

		current_track++;

		if(current_track > AudioPlayer_Track_2)
		{
			current_track = AudioPlayer_Track_1;
		}
	}
}

static void AudioPlayer_Button3_Callback(void)
{
	if(SysTick_WaitAfter(&button3_debounce_entity, 250, true))
	{
		Mod_AudioPlayer_Stop();
	}
}

static void AudioPlayer_Button4_Callback(void)
{
	if(SysTick_WaitAfter(&button4_debounce_entity, 250, true))
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
}

/*-- Exported functions -----------------------------------------------------*/
void Mod_AudioPlayer_Init(void)
{
	If_I2S_Init(I2S_BufferFillNeededCallback);

	//Register buttons callbacks
	If_Exti_RegisterCallback(If_Exti_Button_2, AudioPlayer_Button2_Callback);
	If_Exti_RegisterCallback(If_Exti_Button_3, AudioPlayer_Button3_Callback);
	If_Exti_RegisterCallback(If_Exti_Button_4, AudioPlayer_Button4_Callback);

	//Set status
	module_busy = false;
}

bool Mod_AudioPlayer_IsBusy(void)
{
	return module_busy;
}

void Mod_AudioPlayer_EnterSleepMode(void)
{
	button2_debounce_entity.Started = false;
	button3_debounce_entity.Started = false;
	button4_debounce_entity.Started = false;
}

AudioPlayer_Status_e Mod_AudioPlayer_GetStatus(void)
{
	return player_status;
}

AudioPlayer_Track_e Mod_AudioPlayer_GetTrack(void)
{
	return player_track;
}

void Mod_AudioPlayer_Play(AudioPlayer_Track_e track)
{
	if(player_status == AudioPlayer_Status_Stopped)
	{
		switch(track)
		{
			case AudioPlayer_Track_None:
			{
				player_track = AudioPlayer_Track_None;
				return;
			}
			break;

			case AudioPlayer_Track_1:
			{
				player_track = AudioPlayer_Track_1;
				track_data = (uint16_t *)Melody_U;
				track_data_length = (MELODY_U_SIZE / sizeof(uint16_t));
				//return;
			}
			break;

			case AudioPlayer_Track_2:
			{
				player_track = AudioPlayer_Track_2;
				track_data = (uint16_t *)Melody_Bergen;
				track_data_length = (MELODY_BERGEN_SIZE / sizeof(uint16_t));
				//return;
			}
			break;

			default:
			{
				return;
			}
			break;
		}

		//Set status
		module_busy = true;

		//Clear indexer
		track_index_counter = 0;

		player_status = AudioPlayer_Status_Play;
		Mod_Indication_SetState_Audio(IndiStatus_Audio_Play);

		//Start playing
		If_I2S_Enable();
	}
}

void Mod_AudioPlayer_Pause(void)
{
	if(player_status == AudioPlayer_Status_Play)
	{
		//Set status
		module_busy = true;

		pause_requested = true;
	}
}

void Mod_AudioPlayer_Resume(void)
{
	if(player_status == AudioPlayer_Status_Paused)
	{
		//Set status
		module_busy = true;

		player_status = AudioPlayer_Status_Play;
		Mod_Indication_SetState_Audio(IndiStatus_Audio_Play);

		//Start playing
		If_I2S_Enable();
	}
}

void Mod_AudioPlayer_Stop(void)
{
	if(player_status == AudioPlayer_Status_Play)
	{
		//Set status
		module_busy = true;

		//Stop playing
		If_I2S_Disable();

		player_track = AudioPlayer_Track_None;
		player_status = AudioPlayer_Status_Stopped;

		Mod_Indication_SetState_Audio(IndiStatus_Audio_Idle);
	}
	else if (player_status == AudioPlayer_Status_Paused)
	{
		//Set status
		module_busy = true;

		player_track = AudioPlayer_Track_None;
		player_status = AudioPlayer_Status_Stopped;

		Mod_Indication_SetState_Audio(IndiStatus_Audio_Idle);
	}
}

void Mod_AudioPlayer_Run(void)
{
	If_I2S_Run();

	if(player_status != AudioPlayer_Status_Play)
	{
		if(
			(SysTick_IsWaitElapsed(&button2_debounce_entity)) &&
			(SysTick_IsWaitElapsed(&button3_debounce_entity)) &&
			(SysTick_IsWaitElapsed(&button4_debounce_entity))
			)
		{
			//Set status
			module_busy = false;
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_audio_player.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_AUDIO_PLAYER_H
#define _MOD_AUDIO_PLAYER_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef enum
{
	AudioPlayer_Track_None,
	AudioPlayer_Track_1,
	AudioPlayer_Track_2
} AudioPlayer_Track_e;

typedef enum
{
	AudioPlayer_Status_Stopped,
	AudioPlayer_Status_Play,
	AudioPlayer_Status_Paused,
} AudioPlayer_Status_e;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void Mod_AudioPlayer_Init(void);

bool Mod_AudioPlayer_IsBusy(void);
void Mod_AudioPlayer_EnterSleepMode(void);

AudioPlayer_Status_e Mod_AudioPlayer_GetStatus(void);
AudioPlayer_Track_e Mod_AudioPlayer_GetTrack(void);

void Mod_AudioPlayer_Play(AudioPlayer_Track_e track);
void Mod_AudioPlayer_Pause(void);
void Mod_AudioPlayer_Resume(void);
void Mod_AudioPlayer_Stop(void);

void Mod_AudioPlayer_Run(void);

#endif // _MOD_AUDIO_PLAYER_H
/*-- EOF --------------------------------------------------------------------*/

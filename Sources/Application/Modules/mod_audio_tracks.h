/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_audio_tracks.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_AUDIO_TRACKS_H
#define _MOD_AUDIO_TRACKS_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
#define MELODY_U_SIZE                       475136u //~15 sec
#define MELODY_BERGEN_SIZE					358400u //~11 sec

/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
extern const uint8_t Melody_U[MELODY_U_SIZE];
extern const uint8_t Melody_Bergen[MELODY_BERGEN_SIZE];

/*-- Exported functions -----------------------------------------------------*/

#endif // _MOD_AUDIO_TRACKS_H
/*-- EOF --------------------------------------------------------------------*/

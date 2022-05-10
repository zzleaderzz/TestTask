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
#define MELODY_U_SIZE                       296960u //~10 sec (Luke Bond - U)
#define MELODY_BERGEN_SIZE					358400u //~11 sec (Purple Haze - Bergen)

/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
extern const uint8_t Melody_U[MELODY_U_SIZE];
extern const uint8_t Melody_Bergen[MELODY_BERGEN_SIZE];

/*-- Exported functions -----------------------------------------------------*/

#endif // _MOD_AUDIO_TRACKS_H
/*-- EOF --------------------------------------------------------------------*/

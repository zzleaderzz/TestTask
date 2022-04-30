/*-- File description -------------------------------------------------------*/
/**
 *   @file:    mod_swtimer.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _MOD_SWTIMER_H
#define _MOD_SWTIMER_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void Mod_SwTimer_Init(void);

void Mod_SwTimer_Start(void);
void Mod_SwTimer_Stop(void);

void Mod_SwTimer_Run(void);

#endif // _MOD_SWTIMER_H
/*-- EOF --------------------------------------------------------------------*/

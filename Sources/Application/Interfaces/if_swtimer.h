/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_swtimer.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _IF_SWTIMER_H
#define _IF_SWTIMER_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_SwTimer_Init(void);

void If_SwTimer_Start(void);
void If_SwTimer_Stop(void);

void If_SwTimer_Run(void);

#endif // _IF_SWTIMER_H
/*-- EOF --------------------------------------------------------------------*/

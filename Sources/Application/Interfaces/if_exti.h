/*-- File description -------------------------------------------------------*/
/**
 *   @file:    if_exti.h
 *
 *   @author:  valeriy.grimalskiy
 *   @company: Lab.
 */

#ifndef _IF_EXTI_H
#define _IF_EXTI_H

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef enum
{
	If_Exti_Button_1,
	If_Exti_Button_2,
	If_Exti_Button_3,
	If_Exti_Button_4,
} If_Exti_pin_e;

typedef void(*If_Exti_Callback)(void);

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void If_Exti_Init(void);
void If_Exti_DeInit(void);

void If_Exti_RegisterCallback(If_Exti_pin_e pin, If_Exti_Callback callback);

void If_Exti_Enable(void);
void If_Exti_Disable(void);

#endif // _IF_EXTI_H
/*-- EOF --------------------------------------------------------------------*/


#ifndef _LED_H //#include "led.h" 
#define _LED_H
/***************************************************************************/

#include "stm32f4xx.h"

/***************************************************************************/

#define LED_WORKING_PORT         	GPIOB                 
#define LED_WORKING_PIN          	GPIO_Pin_15                 
#define LED_WORKING_RCC          	RCC_AHB1Periph_GPIOB
#define WORKING_ON()              GPIO_ResetBits(LED_WORKING_PORT, LED_WORKING_PIN)    //0
#define WORKING_OFF()             GPIO_SetBits  (LED_WORKING_PORT, LED_WORKING_PIN)   //1FT
#define WORKING_TOGGLE()          GPIO_ToggleBits(LED_WORKING_PORT, LED_WORKING_PIN)

void LED_Init(void);

/***************************************************************************/
#endif /*_LED_H*/


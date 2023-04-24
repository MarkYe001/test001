
#ifndef _TIMER_H //#include "delay_tim4.h"
#define _TIMER_H
/***************************************************************************/

#include "stm32f4xx.h"
#include "task_priority_cofig.h"

/***************************************************************************/

#define tim4_us_enable   0
#define tim4_ms_enable   1
#define tim4_s_enable    0

//tim4 interrupt
//#define Delay_Tim4_PreemptionPriority         0x00         //抢占优先级
//#define Delay_Tim4_SubPriority                0x01         //子优先级

void Timedelay_tim4_init(void);
void TIM4_Init(u16 arr,u16 psc);
void TIM4_IRQHandler(void);
/***************************************************************************/

#if tim4_us_enable
void Timedelay_tim4_us(uint32_t nus);
void Timedelay_tim4_ms(uint32_t nms);
void Timedelay_tim4_s (uint32_t ns );
#endif

#if tim4_ms_enable
void Timedelay_tim4_ms(uint32_t nms);
void Timedelay_tim4_s (uint32_t ns);
#endif

#if tim4_s_enable
void Timedelay_tim4_s(uint32_t ns);
#endif

/***************************************************************************/
#endif /*_TIMER_H*/

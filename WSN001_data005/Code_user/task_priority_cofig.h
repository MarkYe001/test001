
#ifndef _TASK_PRIORITY_COFIG_H //#include "task_priority_cofig.h"
#define _TASK_PRIORITY_COFIG_H
/*****************************************************************************************************/

#include "stm32f4xx.h"

/*****************************************************************************************************/
/*    This file is used to configure interrupt priority                    */
/*    Set NVIC Grouping to 8 groups of interrupt without 2 sub-grouping    */
/*    3 bits for pre-emption priority,1 bits for subpriority               */
/*    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                      */
/*****************************************************************************************************/
//中断优先级设置 (PreemptionPriority: 0-15 ) (SubPriority: 0 ) 

//usart1
#define USARTx_PreemptionPriority             0x06    //抢占优先级
#define USARTx_SubPriority                    0x00		//子优先级

//tim4 interrupt
#define Delay_Tim4_PreemptionPriority         0x00         //抢占优先级
#define Delay_Tim4_SubPriority                0x00         //子优先级

//dwm1000 interrupt
#define DECARSTIRQ_IRQn_PreemptionPriority    0x07
#define DECARSTIRQ_IRQn_SubPriority           0x00

#define DECAIRQ_IRQn_PreemptionPriority       0x07
#define DECAIRQ_IRQn_SubPriority              0x01

/*****************************************************************************************************/
/*    DMA数据流优先级配置                                                  */
/*    软件优先级有4个级别(非常高,高,中,低)                                 */
/*    软件优先级相同则由硬件决定优先级,请求0的优先级高于请求1              */

//USART
#define USART_DMAx_RX_Priority    DMA_Priority_Medium;          //中等优先级
#define USART_DMAx_TX_Priority    DMA_Priority_Medium;          //中等优先级

/*****************************************************************************************************/
//任务优先级设置





/*****************************************************************************************************/
/*      整个系统的优先级设置,包括中断优先级和任务优先级                                              */
/*      应当在启动之后优先进行优先级设置,并且设置为Group_4,将4bit全用于抢占优先级                    */
/*****************************************************************************************************/
/*
       Cortex-M4的内核优先级规则:   0-15   数字越小逻辑优先级越高
       freeRTOS系统中断优先级规则:   
			 
			      1.使用内核的优先级规则
						
			      2.( 0 ) <= ( configMAX_SYSCALL_INTERRUPT_PRIORITY ) < ( KERNEL_INTERRUPT_PRIORITY) <= ( 15 ) (数值)
						  
							不调用任何FreeRTOS的API函数的中断可以使用所有的中断优先级,并且可以中断嵌套
						
						3.RTOS中断安全临界区:( configMAX_SYSCALL_INTERRUPT_PRIORITY ) <= X <= ( 15 )
						                        此中优先级中断将会在临界区开启时被屏蔽
															    		 临界区用于保护不能被打断的代码
																		此范围内中断可以调用以FromISR结尾的API函数,并且可以中断嵌套
																			 
						4.不受RTOS管理的更高优先级中断:( 0 ) <= X <= ( configMAX_SYSCALL_INTERRUPT_PRIORITY )
						                                不会因为执行FreeRTOS而延时
																  	但同时中断函数不可调用FreeRTOS的API函数(包括以FromISR结尾的也不能调用)
						                                                                                         */
/*****************************************************************************************************/
/*
       freeRTOS优先级级数  configMAX_PRIORITIES	( 5 )   数值不宜太大
			 0逻辑优先级最低,configMAX_PRIORITIES逻辑优先级最高
       即使是最低逻辑优先级的中断也可以打断最高逻辑优先级的任务
                                                                                                     */
/*****************************************************************************************************/
#endif /*_TASK_PRIORITY_COFIG_H*/

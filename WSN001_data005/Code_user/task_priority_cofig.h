
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
//�ж����ȼ����� (PreemptionPriority: 0-15 ) (SubPriority: 0 ) 

//usart1
#define USARTx_PreemptionPriority             0x06    //��ռ���ȼ�
#define USARTx_SubPriority                    0x00		//�����ȼ�

//tim4 interrupt
#define Delay_Tim4_PreemptionPriority         0x00         //��ռ���ȼ�
#define Delay_Tim4_SubPriority                0x00         //�����ȼ�

//dwm1000 interrupt
#define DECARSTIRQ_IRQn_PreemptionPriority    0x07
#define DECARSTIRQ_IRQn_SubPriority           0x00

#define DECAIRQ_IRQn_PreemptionPriority       0x07
#define DECAIRQ_IRQn_SubPriority              0x01

/*****************************************************************************************************/
/*    DMA���������ȼ�����                                                  */
/*    ������ȼ���4������(�ǳ���,��,��,��)                                 */
/*    ������ȼ���ͬ����Ӳ���������ȼ�,����0�����ȼ���������1              */

//USART
#define USART_DMAx_RX_Priority    DMA_Priority_Medium;          //�е����ȼ�
#define USART_DMAx_TX_Priority    DMA_Priority_Medium;          //�е����ȼ�

/*****************************************************************************************************/
//�������ȼ�����





/*****************************************************************************************************/
/*      ����ϵͳ�����ȼ�����,�����ж����ȼ����������ȼ�                                              */
/*      Ӧ��������֮�����Ƚ������ȼ�����,��������ΪGroup_4,��4bitȫ������ռ���ȼ�                    */
/*****************************************************************************************************/
/*
       Cortex-M4���ں����ȼ�����:   0-15   ����ԽС�߼����ȼ�Խ��
       freeRTOSϵͳ�ж����ȼ�����:   
			 
			      1.ʹ���ں˵����ȼ�����
						
			      2.( 0 ) <= ( configMAX_SYSCALL_INTERRUPT_PRIORITY ) < ( KERNEL_INTERRUPT_PRIORITY) <= ( 15 ) (��ֵ)
						  
							�������κ�FreeRTOS��API�������жϿ���ʹ�����е��ж����ȼ�,���ҿ����ж�Ƕ��
						
						3.RTOS�жϰ�ȫ�ٽ���:( configMAX_SYSCALL_INTERRUPT_PRIORITY ) <= X <= ( 15 )
						                        �������ȼ��жϽ������ٽ�������ʱ������
															    		 �ٽ������ڱ������ܱ���ϵĴ���
																		�˷�Χ���жϿ��Ե�����FromISR��β��API����,���ҿ����ж�Ƕ��
																			 
						4.����RTOS����ĸ������ȼ��ж�:( 0 ) <= X <= ( configMAX_SYSCALL_INTERRUPT_PRIORITY )
						                                ������Ϊִ��FreeRTOS����ʱ
																  	��ͬʱ�жϺ������ɵ���FreeRTOS��API����(������FromISR��β��Ҳ���ܵ���)
						                                                                                         */
/*****************************************************************************************************/
/*
       freeRTOS���ȼ�����  configMAX_PRIORITIES	( 5 )   ��ֵ����̫��
			 0�߼����ȼ����,configMAX_PRIORITIES�߼����ȼ����
       ��ʹ������߼����ȼ����ж�Ҳ���Դ������߼����ȼ�������
                                                                                                     */
/*****************************************************************************************************/
#endif /*_TASK_PRIORITY_COFIG_H*/

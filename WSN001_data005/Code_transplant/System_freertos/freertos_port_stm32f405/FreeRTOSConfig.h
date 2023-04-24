
#ifndef FREERTOS_CONFIG_H//#include "FreeRTOSConfig.h"
#define FREERTOS_CONFIG_H

/***************************************************************************/

#include "stm32f4xx.h"   //����ϵͳʱ�Ӻ�stdio
//��ֲ��Ҫ�����ж�(SVC_Handler,PendSV_Handler,SysTick_Handler)

/***************************************************************************/

#define configUSE_PREEMPTION						1   /**/ /*1:��ռʽ,0:Э��ʽ*/
#define configCPU_CLOCK_HZ							( SystemCoreClock )        /*ϵͳʱ��Ƶ��*/
#define configTICK_RATE_HZ							( ( TickType_t ) 1000 )    /*fsʱ��ƬƵ��,1/fsΪʱ��Ƭ����*/
#define configUSE_16_BIT_TICKS					0                          /*1:����TickType_tΪ�޷���16λ,0:32λ*/

#define configMAX_PRIORITIES						( 5 )    /*���ȼ�����*/
#define configUSE_TIME_SLICING          1        /*ͬ���ȼ�����ʱ��Ƭ����*/
#define configIDLE_SHOULD_YIELD					1        /*ͬ�������ȼ���,��������Ϊ�û�������·ʹ��,
                                                   ��ռʽ�ں�ʹ��*/
#define configUSE_IDLE_HOOK							0        /*���й���ʹ��,����ʱ���ô˺���*/
#define configUSE_TICK_HOOK							0        /*ʱ��Ƭ����ʹ��,ʱ��Ƭ�ж����ڵ��ô˺���*/

#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 130 )    /*��С���ж�ջ*/
#define configTOTAL_HEAP_SIZE						( ( size_t ) ( 75 * 1024 ) )  /*RTOS�ں��ܹ�����RAM*/
#define configMAX_TASK_NAME_LEN					( 10 )                        /*������������Ϣ����,����/0*/
#define configCHECK_FOR_STACK_OVERFLOW	0        /*��ջ�������,��Ҫ�ض����Ӻ���*/
#define configUSE_MALLOC_FAILED_HOOK		0        /*�ڴ����ʧ�ܹ���*/

#define configUSE_MUTEXES								1        /*�����ź���ʹ��(ͬһ�������벢�ͷ�)*/
#define configUSE_RECURSIVE_MUTEXES			0        /*�ݹ黥����ʹ��*/

#define configUSE_APPLICATION_TASK_TAG	0        /*ʹ�������ǩ*/
#define configUSE_COUNTING_SEMAPHORES		1
#define configGENERATE_RUN_TIME_STATS		0        /*����ʱ��ͳ��*/ 
#define configUSE_TRACE_FACILITY				0        /*���ӻ����ٵ���ʹ��*/
#define configQUEUE_REGISTRY_SIZE				0        /*���м�¼,�ں˵���ʱʹ��*/

/***************************************************************************/
//��ʹ��Э�̹���

/* Software timer definitions. *//*�����ʱ������,ʹ��ʱtimers.cҪ������*/
#define configUSE_TIMERS								1        /*�����ʱ��ʹ��*/
#define configTIMER_TASK_PRIORITY				( 2 )    /*�����ʱ���������ȼ�*/
#define configTIMER_QUEUE_LENGTH				10       /*�����ʱ��������г���*/
#define configTIMER_TASK_STACK_DEPTH		( configMINIMAL_STACK_SIZE * 2 )   /*�����ʱ�������ջ���*/

/***************************************************************************/

/*����ʹ�ü�����ʹ��*/
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete							1
#define INCLUDE_vTaskCleanUpResources		1
#define INCLUDE_vTaskSuspend						1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay							1

#define INCLUDE_xTimerPendFunctionCall  1  //���ж���ʹ���¼�����һ,ʵ���������ʱ��ʵ��,�������������ʱ����ʹ��

/***************************************************************************/

/* Cortex-M specific definitions. */
/*M4�жϼ���ʹ�ù�4bit����,��4bitȫ������ռ���ȼ�*/
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/***************************************************************************/

/*rtos���ں��ж����ȼ�����*/

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf        //������ȼ�

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5        //ϵͳ�ɹ����������ȼ�

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )     //�ں��ж����ȼ�

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )    //��������ж����ȼ�

/***************************************************************************/

/*����,�ж����������ʽ��ȷ��,�Ƽ��ڿ���ʱʹ��*/
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	

/***************************************************************************/

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */


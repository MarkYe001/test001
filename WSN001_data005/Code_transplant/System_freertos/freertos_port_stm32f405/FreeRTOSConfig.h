
#ifndef FREERTOS_CONFIG_H//#include "FreeRTOSConfig.h"
#define FREERTOS_CONFIG_H

/***************************************************************************/

#include "stm32f4xx.h"   //包含系统时钟和stdio
//移植需要挂载中断(SVC_Handler,PendSV_Handler,SysTick_Handler)

/***************************************************************************/

#define configUSE_PREEMPTION						1   /**/ /*1:抢占式,0:协作式*/
#define configCPU_CLOCK_HZ							( SystemCoreClock )        /*系统时钟频率*/
#define configTICK_RATE_HZ							( ( TickType_t ) 1000 )    /*fs时间片频率,1/fs为时间片长短*/
#define configUSE_16_BIT_TICKS					0                          /*1:定义TickType_t为无符号16位,0:32位*/

#define configMAX_PRIORITIES						( 5 )    /*优先级级数*/
#define configUSE_TIME_SLICING          1        /*同优先级任务时间片共享*/
#define configIDLE_SHOULD_YIELD					1        /*同空闲优先级下,空闲任务为用户任务让路使能,
                                                   抢占式内核使用*/
#define configUSE_IDLE_HOOK							0        /*空闲钩子使能,空闲时调用此函数*/
#define configUSE_TICK_HOOK							0        /*时间片钩子使能,时间片中断周期调用此函数*/

#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 130 )    /*最小空闲堆栈*/
#define configTOTAL_HEAP_SIZE						( ( size_t ) ( 75 * 1024 ) )  /*RTOS内核总共可用RAM*/
#define configMAX_TASK_NAME_LEN					( 10 )                        /*任务函数描述信息长度,包括/0*/
#define configCHECK_FOR_STACK_OVERFLOW	0        /*堆栈溢出处理,需要特定钩子函数*/
#define configUSE_MALLOC_FAILED_HOOK		0        /*内存分配失败钩子*/

#define configUSE_MUTEXES								1        /*互斥信号量使能(同一任务申请并释放)*/
#define configUSE_RECURSIVE_MUTEXES			0        /*递归互斥量使能*/

#define configUSE_APPLICATION_TASK_TAG	0        /*使用任务标签*/
#define configUSE_COUNTING_SEMAPHORES		1
#define configGENERATE_RUN_TIME_STATS		0        /*运行时间统计*/ 
#define configUSE_TRACE_FACILITY				0        /*可视化跟踪调试使能*/
#define configQUEUE_REGISTRY_SIZE				0        /*队列记录,内核调试时使用*/

/***************************************************************************/
//不使用协程功能

/* Software timer definitions. *//*软件定时器配置,使用时timers.c要被包含*/
#define configUSE_TIMERS								1        /*软件定时器使能*/
#define configTIMER_TASK_PRIORITY				( 2 )    /*软件定时器服务优先级*/
#define configTIMER_QUEUE_LENGTH				10       /*软件定时器命令队列长度*/
#define configTIMER_TASK_STACK_DEPTH		( configMINIMAL_STACK_SIZE * 2 )   /*软件定时器服务堆栈深度*/

/***************************************************************************/

/*函数使用及编译使能*/
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete							1
#define INCLUDE_vTaskCleanUpResources		1
#define INCLUDE_vTaskSuspend						1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay							1

#define INCLUDE_xTimerPendFunctionCall  1  //在中断中使用事件组置一,实际用软件定时器实现,此条件和软件定时器须使能

/***************************************************************************/

/* Cortex-M specific definitions. */
/*M4中断级数使用过4bit保存,将4bit全用于抢占优先级*/
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/***************************************************************************/

/*rtos和内核中断优先级配置*/

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf        //最低优先级

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5        //系统可管理的最大优先级

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )     //内核中断优先级

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )    //管理最高中断优先级

/***************************************************************************/

/*断言,判断输入参数格式正确性,推荐在开发时使用*/
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


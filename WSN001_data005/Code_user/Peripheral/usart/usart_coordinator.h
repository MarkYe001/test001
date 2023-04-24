
#ifndef _USART_H //#include "usart1.h"
#define _USART_H
/***************************************************************************/

#include "stdio.h"
#include "stm32f4xx.h"
#include "task_priority_cofig.h"

/***************************************************************************/
//选择USART1收发模式：中断或DMA,1为使能,0为禁止
#define USARTx_DMA_ENALBE         1   

//使用串口printf打印
#define USARTx_PRINTF             1

/***************************************************************************/
//USARTx硬件引脚配置
#if 1

//外设及引脚
#define   USARTx                    USART1
#define   USARTx_PORT               GPIOA
#define   USARTx_PIN_TX             GPIO_Pin_9    //GPIOA9 复用为USART1_TX
#define   USARTx_PIN_RX             GPIO_Pin_10   //GPIOA10复用为USART1_RX
//时钟
#define   USARTx_RCC_PORT           RCC_AHB1Periph_GPIOA
#define   USARTx_RCC_USART          RCC_APB2Periph_USART1
//引脚复用
#define   USARTx_GPIO_PinSource_TX  GPIO_PinSource9
#define   USARTx_GPIO_PinSource_RX  GPIO_PinSource10
#define   USARTx_AF                 GPIO_AF_USART1
//USART配置
#define   USARTx_Baudrate           460800
#define   USARTx_Mode               ( USART_Mode_Rx | USART_Mode_Tx )
//中断配置
#define   USARTx_IRQN               USART1_IRQn
//#define   USARTx_PreemptionPriority    7    //抢占优先级
//#define   USARTx_SubPriority           0		//子优先级

#endif /*1*/
/***************************************************************************/
//USART的DMA传输配置

#define   USART_DMA_RX_LEN    128
#define   USART_DMA_TX_LEN    1024

extern    uint8_t USARTDMA_RX_BUFF[USART_DMA_RX_LEN];
extern    uint8_t USARTDMA_TX_BUFF[USART_DMA_TX_LEN];

#if USARTx_DMA_ENALBE

#define   USART_DMAx_RCC                    RCC_AHB1Periph_DMA2

//DMA接收配置
#define   USART_DMAx_RX_Streamx             DMA2_Stream2                  //数据流选择  
#define   USART_DMAx_RX_Channel             DMA_Channel_4                 //通道选择
#define   USART_DMAx_RX_USARTAddr           ((uint32_t)&USART1->DR)       //DMA外设地址
#define   USART_DMAx_RX_MemoryAddr          ((uint32_t)USARTDMA_RX_BUFF)  //DMA 存储器地址
#define   USART_DMAx_RX_DIR                 DMA_DIR_PeripheralToMemory    //外设到存储器模式
#define   USART_DMAx_RX_BufferSize          USART_DMA_RX_LEN;             //数据传输总数目
#define   USART_DMAx_RX_PeripheralInc       DMA_PeripheralInc_Disable     //外设非增量模式
#define   USART_DMAx_RX_MemoryInc           DMA_MemoryInc_Enable;         //存储器增量模式
#define   USART_DMAx_RX_PeripheralDataSize  DMA_PeripheralDataSize_Byte;  //外设数据长度:8位
#define   USART_DMAx_RX_MemoryDataSize      DMA_MemoryDataSize_Byte;      //存储器数据长度:8位
#define   USART_DMAx_RX_Mode                DMA_Mode_Normal;              // 使用普通模式 
//#define   USART_DMAx_RX_Priority            DMA_Priority_Medium;          //中等优先级
#define   USART_DMAx_RX_FIFOMode            DMA_FIFOMode_Disable;         //不开启FIFO模式    
#define   USART_DMAx_RX_FIFOThreshold       DMA_FIFOThreshold_Full;       //FIFO阈值,未开启所以不关心
#define   USART_DMAx_RX_MemoryBurst         DMA_MemoryBurst_Single;       //存储器突发单次传输
#define   USART_DMAx_RX_PeripheralBurst     DMA_PeripheralBurst_Single;   //外设突发单次传输

//USART_IT_IDLE,空闲中断标志,硬件置1,软件清0
//DMA_IT_TCIFx,数据流x传输完成中断标志,硬件置1,软件清0
#define   USART_DMAx_RX_IT_TCIFx            DMA_IT_TCIF2

//DMA发送配置
#define   USART_DMAx_TX_Streamx             DMA2_Stream7                  //数据流选择  
#define   USART_DMAx_TX_Channel             DMA_Channel_4                 //通道选择
#define   USART_DMAx_TX_USARTAddr           ((uint32_t)&USART1->DR)       //DMA外设地址
#define   USART_DMAx_TX_MemoryAddr          ((uint32_t)USARTDMA_TX_BUFF)  //DMA 存储器地址
#define   USART_DMAx_TX_DIR                 DMA_DIR_MemoryToPeripheral    //存储器到外设模式
#define   USART_DMAx_TX_BufferSize          USART_DMA_TX_LEN;             //数据传输总数目
#define   USART_DMAx_TX_PeripheralInc       DMA_PeripheralInc_Disable     //外设非增量模式
#define   USART_DMAx_TX_MemoryInc           DMA_MemoryInc_Enable;         //存储器增量模式
#define   USART_DMAx_TX_PeripheralDataSize  DMA_PeripheralDataSize_Byte;  //外设数据长度:8位
#define   USART_DMAx_TX_MemoryDataSize      DMA_MemoryDataSize_Byte;      //存储器数据长度:8位
#define   USART_DMAx_TX_Mode                DMA_Mode_Normal;              // 使用普通模式 
//#define   USART_DMAx_TX_Priority            DMA_Priority_Medium;          //中等优先级
#define   USART_DMAx_TX_FIFOMode            DMA_FIFOMode_Disable;         
#define   USART_DMAx_TX_FIFOThreshold       DMA_FIFOThreshold_Full;
#define   USART_DMAx_TX_MemoryBurst         DMA_MemoryBurst_Single;       //存储器突发单次传输
#define   USART_DMAx_TX_PeripheralBurst     DMA_PeripheralBurst_Single;   //外设突发单次传输

//DMA_IT_TCIFx,数据流x传输完成中断标志,硬件置1,软件清0
#define   USART_DMAx_TX_IT_TCIFx            DMA_IT_TCIF7

#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/
//coordinator的USART数据传输配置
#if USARTx_DMA_ENALBE

//USART数据接收状态
typedef enum{
	
	USART_RX_STATE_IDLE           = 0x00,
  USART_RX_STATE_RX_HEAD_OK     = 0x01,
  USART_RX_STATE_RX_HEAD_FAIL   = 0x02,
  USART_RX_STATE_RX_TAIL_OK     = 0x03,
	USART_RX_STATE_RX_TAIL_FAIL   = 0x04
}USARTx_state ;

//待处理数据位置
typedef enum{
	
	DATA_RX_NO            =  0x00,
	DATA_RX_USART         =  0x01,
	DATA_RX_DATABUFFER    =  0x02
}DataAddr;

//USARTx数据DMA接收结构体
typedef struct
{
  USARTx_state   state;    //当前串口状态
  
  uint16_t  totalDataLen;  //总的数据长度
  uint16_t  currDataLoca;  //当前数据在BUFFER中的位置
  uint16_t  currDataLen;   //当前的数据长度
	
	DataAddr  dataaddr;      //当前数据地址:无,usart,databuffer
  
  uint8_t*  data;         //与uart_DataBuffer同地址,初始化时进行地址赋值
}UartRxDataParaStruct;

extern UartRxDataParaStruct UartRxDataPara;
extern uint8_t uart_DataBuffer[USART_DMA_RX_LEN];

#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/

void usartx_init(void);
void USARTx_DMA_RX_Config(void);
void USARTx_DMA_TX_Config(void);
void USARTx_DMA_Send_Enable(uint16_t ndtr);
void usart1_showarray(uint8_t *array,uint8_t len);
void usart1_showuintx (char *p,uint32_t dat,uint8_t len);

/***************************************************************************/
#endif /*#define _USART_H*/



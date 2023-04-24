
#ifndef _USART_H //#include "usart1.h"
#define _USART_H
/***************************************************************************/

#include "stdio.h"
#include "stm32f4xx.h"
#include "task_priority_cofig.h"

/***************************************************************************/
//ѡ��USART1�շ�ģʽ���жϻ�DMA,1Ϊʹ��,0Ϊ��ֹ
#define USARTx_DMA_ENALBE         1   

//ʹ�ô���printf��ӡ
#define USARTx_PRINTF             1

/***************************************************************************/
//USARTxӲ����������
#if 1

//���輰����
#define   USARTx                    USART1
#define   USARTx_PORT               GPIOA
#define   USARTx_PIN_TX             GPIO_Pin_9    //GPIOA9 ����ΪUSART1_TX
#define   USARTx_PIN_RX             GPIO_Pin_10   //GPIOA10����ΪUSART1_RX
//ʱ��
#define   USARTx_RCC_PORT           RCC_AHB1Periph_GPIOA
#define   USARTx_RCC_USART          RCC_APB2Periph_USART1
//���Ÿ���
#define   USARTx_GPIO_PinSource_TX  GPIO_PinSource9
#define   USARTx_GPIO_PinSource_RX  GPIO_PinSource10
#define   USARTx_AF                 GPIO_AF_USART1
//USART����
#define   USARTx_Baudrate           460800
#define   USARTx_Mode               ( USART_Mode_Rx | USART_Mode_Tx )
//�ж�����
#define   USARTx_IRQN               USART1_IRQn
//#define   USARTx_PreemptionPriority    7    //��ռ���ȼ�
//#define   USARTx_SubPriority           0		//�����ȼ�

#endif /*1*/
/***************************************************************************/
//USART��DMA��������

#define   USART_DMA_RX_LEN    128
#define   USART_DMA_TX_LEN    1024

extern    uint8_t USARTDMA_RX_BUFF[USART_DMA_RX_LEN];
extern    uint8_t USARTDMA_TX_BUFF[USART_DMA_TX_LEN];

#if USARTx_DMA_ENALBE

#define   USART_DMAx_RCC                    RCC_AHB1Periph_DMA2

//DMA��������
#define   USART_DMAx_RX_Streamx             DMA2_Stream2                  //������ѡ��  
#define   USART_DMAx_RX_Channel             DMA_Channel_4                 //ͨ��ѡ��
#define   USART_DMAx_RX_USARTAddr           ((uint32_t)&USART1->DR)       //DMA�����ַ
#define   USART_DMAx_RX_MemoryAddr          ((uint32_t)USARTDMA_RX_BUFF)  //DMA �洢����ַ
#define   USART_DMAx_RX_DIR                 DMA_DIR_PeripheralToMemory    //���赽�洢��ģʽ
#define   USART_DMAx_RX_BufferSize          USART_DMA_RX_LEN;             //���ݴ�������Ŀ
#define   USART_DMAx_RX_PeripheralInc       DMA_PeripheralInc_Disable     //���������ģʽ
#define   USART_DMAx_RX_MemoryInc           DMA_MemoryInc_Enable;         //�洢������ģʽ
#define   USART_DMAx_RX_PeripheralDataSize  DMA_PeripheralDataSize_Byte;  //�������ݳ���:8λ
#define   USART_DMAx_RX_MemoryDataSize      DMA_MemoryDataSize_Byte;      //�洢�����ݳ���:8λ
#define   USART_DMAx_RX_Mode                DMA_Mode_Normal;              // ʹ����ͨģʽ 
//#define   USART_DMAx_RX_Priority            DMA_Priority_Medium;          //�е����ȼ�
#define   USART_DMAx_RX_FIFOMode            DMA_FIFOMode_Disable;         //������FIFOģʽ    
#define   USART_DMAx_RX_FIFOThreshold       DMA_FIFOThreshold_Full;       //FIFO��ֵ,δ�������Բ�����
#define   USART_DMAx_RX_MemoryBurst         DMA_MemoryBurst_Single;       //�洢��ͻ�����δ���
#define   USART_DMAx_RX_PeripheralBurst     DMA_PeripheralBurst_Single;   //����ͻ�����δ���

//USART_IT_IDLE,�����жϱ�־,Ӳ����1,�����0
//DMA_IT_TCIFx,������x��������жϱ�־,Ӳ����1,�����0
#define   USART_DMAx_RX_IT_TCIFx            DMA_IT_TCIF2

//DMA��������
#define   USART_DMAx_TX_Streamx             DMA2_Stream7                  //������ѡ��  
#define   USART_DMAx_TX_Channel             DMA_Channel_4                 //ͨ��ѡ��
#define   USART_DMAx_TX_USARTAddr           ((uint32_t)&USART1->DR)       //DMA�����ַ
#define   USART_DMAx_TX_MemoryAddr          ((uint32_t)USARTDMA_TX_BUFF)  //DMA �洢����ַ
#define   USART_DMAx_TX_DIR                 DMA_DIR_MemoryToPeripheral    //�洢��������ģʽ
#define   USART_DMAx_TX_BufferSize          USART_DMA_TX_LEN;             //���ݴ�������Ŀ
#define   USART_DMAx_TX_PeripheralInc       DMA_PeripheralInc_Disable     //���������ģʽ
#define   USART_DMAx_TX_MemoryInc           DMA_MemoryInc_Enable;         //�洢������ģʽ
#define   USART_DMAx_TX_PeripheralDataSize  DMA_PeripheralDataSize_Byte;  //�������ݳ���:8λ
#define   USART_DMAx_TX_MemoryDataSize      DMA_MemoryDataSize_Byte;      //�洢�����ݳ���:8λ
#define   USART_DMAx_TX_Mode                DMA_Mode_Normal;              // ʹ����ͨģʽ 
//#define   USART_DMAx_TX_Priority            DMA_Priority_Medium;          //�е����ȼ�
#define   USART_DMAx_TX_FIFOMode            DMA_FIFOMode_Disable;         
#define   USART_DMAx_TX_FIFOThreshold       DMA_FIFOThreshold_Full;
#define   USART_DMAx_TX_MemoryBurst         DMA_MemoryBurst_Single;       //�洢��ͻ�����δ���
#define   USART_DMAx_TX_PeripheralBurst     DMA_PeripheralBurst_Single;   //����ͻ�����δ���

//DMA_IT_TCIFx,������x��������жϱ�־,Ӳ����1,�����0
#define   USART_DMAx_TX_IT_TCIFx            DMA_IT_TCIF7

#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/
//coordinator��USART���ݴ�������
#if USARTx_DMA_ENALBE

//USART���ݽ���״̬
typedef enum{
	
	USART_RX_STATE_IDLE           = 0x00,
  USART_RX_STATE_RX_HEAD_OK     = 0x01,
  USART_RX_STATE_RX_HEAD_FAIL   = 0x02,
  USART_RX_STATE_RX_TAIL_OK     = 0x03,
	USART_RX_STATE_RX_TAIL_FAIL   = 0x04
}USARTx_state ;

//����������λ��
typedef enum{
	
	DATA_RX_NO            =  0x00,
	DATA_RX_USART         =  0x01,
	DATA_RX_DATABUFFER    =  0x02
}DataAddr;

//USARTx����DMA���սṹ��
typedef struct
{
  USARTx_state   state;    //��ǰ����״̬
  
  uint16_t  totalDataLen;  //�ܵ����ݳ���
  uint16_t  currDataLoca;  //��ǰ������BUFFER�е�λ��
  uint16_t  currDataLen;   //��ǰ�����ݳ���
	
	DataAddr  dataaddr;      //��ǰ���ݵ�ַ:��,usart,databuffer
  
  uint8_t*  data;         //��uart_DataBufferͬ��ַ,��ʼ��ʱ���е�ַ��ֵ
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



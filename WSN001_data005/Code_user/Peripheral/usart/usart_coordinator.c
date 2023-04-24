/***************************************************************************/

//#include "head_file.h"
#include "usart_coordinator.h"
#include "wsn_hostcmd.h"

/***************************************************************************/
//ʹ��printf��ӡ��������Ҫ�ض���fputc���� 
#if USARTx_PRINTF
int fputc(int ch, FILE *f)
{ 	
	while((USARTx->SR&0X40)==0);//ѭ������,ֱ���������   
	USARTx->DR = (u8) ch;      
	return ch;
}//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���  
#endif /*USARTx_PRINTF*/
/***************************************************************************/
//USARTʹ��DMA�շ���ر���
#if USARTx_DMA_ENALBE

//USARTʹ��DMA���ͽ��յĻ�����
uint8_t USARTDMA_RX_BUFF[USART_DMA_RX_LEN];
uint8_t USARTDMA_TX_BUFF[USART_DMA_TX_LEN];

//����ʹ�õ���ر���
UartRxDataParaStruct UartRxDataPara;
uint8_t uart_DataBuffer[USART_DMA_RX_LEN] = {0};
//���쳤�����ݹ�����,����16λ����������2��8λ����ת��
union u16length{
	 uint16_t len;
	 struct
	 {			  			
		uint8_t byte0;
		uint8_t byte1;
	 }BYTES;
}DATALEN;

#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/
//��ʼ��USARTx

void usartx_init(void){

	//GPIO�˿�����
  GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(USARTx_RCC_PORT, ENABLE); //ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(USARTx_RCC_USART,ENABLE);//ʹ��USARTʱ��
 
  //USARTx ��ʼ������
	USART_InitStructure.USART_BaudRate = USARTx_Baudrate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USARTx_Mode;	//�շ�ģʽ
  USART_Init(USARTx, &USART_InitStructure); //��ʼ������
	
	//ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);

	//��ʼ�����ں��ٽ���IO������(���ڳ�ʼ�����п��ܷ���һ����Ч��0x00��0xfe)
	//USARTx GPIO����
  GPIO_InitStructure.GPIO_Pin   = USARTx_PIN_TX | USARTx_PIN_RX; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //����
	GPIO_Init(USARTx_PORT,&GPIO_InitStructure); //��ʼ��
	
	//���ڶ�Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(USARTx_PORT,USARTx_GPIO_PinSource_TX,USARTx_AF); //GPIOA9����ΪUSART1_TX
	GPIO_PinAFConfig(USARTx_PORT,USARTx_GPIO_PinSource_RX,USARTx_AF); //GPIOA10����ΪUSART1_RX

  //���TXE,��ֹ��һ�����ݷ���ʱ��©��
  USART_ClearFlag(USARTx,USART_FLAG_TC);

#if USARTx_DMA_ENALBE  //DMA�жϽ�����Ҫ����USART���߿����жϺ�DMA��Ӧ�����������ж�
/*---------------------------------------------------------------------*/

  //����USART���߿����ж�,��USART���߿���ʱ�����ж�
  USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	
	//Usartx NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQN;//����x�ж�ͨ��//USART1_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USARTx_PreemptionPriority;   //��ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = USARTx_SubPriority;	      	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	//����DMA����
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	USARTx_DMA_RX_Config();
	
	//����DMA����
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	USARTx_DMA_TX_Config();
	
	//���ô���״̬
	UartRxDataPara.state        = USART_RX_STATE_IDLE;
  UartRxDataPara.data         = uart_DataBuffer;
	UartRxDataPara.totalDataLen = 0;
	UartRxDataPara.dataaddr     = DATA_RX_NO;
	
/*---------------------------------------------------------------------*/
#endif /*USARTx_DMA_ENALBE*/
}

/***************************************************************************/
#if USARTx_DMA_ENALBE

//DMA�������ú���
void USARTx_DMA_RX_Config(void){
	
	DMA_InitTypeDef  DMA_InitStructure;
	
	//DMAʱ��ʹ��
	RCC_AHB1PeriphClockCmd(USART_DMAx_RCC,ENABLE);
  
	//��λ��ʼ��DMA������
  DMA_DeInit(USART_DMAx_RX_Streamx);
	
	//�ȴ�DMA��������λ���
	while (DMA_GetCmdStatus(USART_DMAx_RX_Streamx) != DISABLE){}
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel            = USART_DMAx_RX_Channel;        //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DMAx_RX_USARTAddr;      //DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr    = USART_DMAx_RX_MemoryAddr;     //DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR                = USART_DMAx_RX_DIR;            //�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize         = USART_DMAx_RX_BufferSize;     //���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc      = USART_DMAx_RX_PeripheralInc;  //���������ģʽ
  DMA_InitStructure.DMA_MemoryInc          = USART_DMAx_RX_MemoryInc;      //�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = USART_DMAx_RX_PeripheralDataSize;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize     = USART_DMAx_RX_MemoryDataSize; //�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode               = USART_DMAx_RX_Mode;           // ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority           = USART_DMAx_RX_Priority;       //�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode           = USART_DMAx_RX_FIFOMode;         
  DMA_InitStructure.DMA_FIFOThreshold      = USART_DMAx_RX_FIFOThreshold;
  DMA_InitStructure.DMA_MemoryBurst        = USART_DMAx_RX_MemoryBurst;    //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst    = USART_DMAx_RX_PeripheralBurst;//����ͻ�����δ���
		
  DMA_Init(USART_DMAx_RX_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
	
	//�˴����뿪���жϣ�����DMA_GetITStatus(USART_DMAx_RX_Streamx,USART_DMAx_RX_IT_TCIFx)һֱΪ0		
	DMA_ITConfig(USART_DMAx_RX_Streamx,DMA_IT_TC,ENABLE);
	
  //ʹ��DMA����
  DMA_Cmd(USART_DMAx_RX_Streamx, ENABLE);
}

//DMA�������ú���
void USARTx_DMA_TX_Config(void){
	
	DMA_InitTypeDef  DMA_InitStructure;
	
	//DMAʱ��ʹ��
	RCC_AHB1PeriphClockCmd(USART_DMAx_RCC,ENABLE);
  
	//��λ��ʼ��DMA������
  DMA_DeInit(USART_DMAx_TX_Streamx);
	
	//�ȴ�DMA��������λ���
	while (DMA_GetCmdStatus(USART_DMAx_TX_Streamx) != DISABLE){}
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel            = USART_DMAx_TX_Channel;        //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DMAx_TX_USARTAddr;      //DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr    = USART_DMAx_TX_MemoryAddr;     //DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR                = USART_DMAx_TX_DIR;            //�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize         = USART_DMAx_TX_BufferSize;     //���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc      = USART_DMAx_TX_PeripheralInc;  //���������ģʽ
  DMA_InitStructure.DMA_MemoryInc          = USART_DMAx_TX_MemoryInc;      //�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = USART_DMAx_TX_PeripheralDataSize;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize     = USART_DMAx_TX_MemoryDataSize; //�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode               = USART_DMAx_TX_Mode;           // ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority           = USART_DMAx_TX_Priority;       //�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode           = USART_DMAx_TX_FIFOMode;         
  DMA_InitStructure.DMA_FIFOThreshold      = USART_DMAx_TX_FIFOThreshold;
  DMA_InitStructure.DMA_MemoryBurst        = USART_DMAx_TX_MemoryBurst;    //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst    = USART_DMAx_TX_PeripheralBurst;//����ͻ�����δ���
		
  DMA_Init(USART_DMAx_TX_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream

  //�˴����뿪���жϣ�����DMA_GetITStatus(USART_DMAx_TX_Streamx,USART_DMAx_TX_IT_TCIFx)һֱΪ0		
	DMA_ITConfig(USART_DMAx_TX_Streamx,DMA_IT_TC,ENABLE);

  //ʹ��DMA����
  //DMA_Cmd(USART_DMAx_TX_Streamx, ENABLE);
	
}

#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#if USARTx_DMA_ENALBE
void USART1_IRQHandler(void){               	//����1�жϷ������

  if(USART_GetITStatus(USARTx, USART_IT_IDLE) != RESET)  //USART���߿����ж�
  { 
		//˳���ȡSR,DR�Ĵ���,��IDLE��־λ��0
		USARTx->SR;USARTx->DR;
		
		//�ر�DMA����
		DMA_Cmd(USART_DMAx_RX_Streamx, DISABLE);
		
		//���������������ɵ��жϱ�־
		DMA_ClearFlag(USART_DMAx_RX_Streamx,USART_DMAx_RX_IT_TCIFx);
		
		//�������ݴ������
		if((USARTDMA_RX_BUFF[0]==0xAA)&(USARTDMA_RX_BUFF[1]==0x55))//��֤����ͷ
		{
			//����ͷ��ȷ
			UartRxDataPara.state = USART_RX_STATE_RX_HEAD_OK;
			
			//��ѯ���ݳ���(��������+���� �ĳ���)
      DATALEN.BYTES.byte0 = USARTDMA_RX_BUFF[2];
		  DATALEN.BYTES.byte1 = USARTDMA_RX_BUFF[3];
		
			if((USARTDMA_RX_BUFF[DATALEN.len+4]==0x55)&(USARTDMA_RX_BUFF[DATALEN.len+5]==0xAA))//��֤����β
		  {
				//����β��ȷ
				UartRxDataPara.state    = USART_RX_STATE_RX_TAIL_OK;
				
				UartRxDataPara.dataaddr = DATA_RX_USART;//���ݴ�����USARTDMA_RX_BUFF
				UartRxDataPara.totalDataLen = DATALEN.len;//������������+���� �ĳ���,����������ͷβ�ͳ���
				
				//�������ݴ���
				{
				  uint16_t i;
					for(i=0;i<(UartRxDataPara.totalDataLen);i++)//����������ͷ,���ݳ���,����β
	        {
			      uart_DataBuffer[i]=USARTDMA_RX_BUFF[i+4];//�����ݿ�����uart_DataBuffer�ȴ�����
		      }
//				  printf("\r\nlen of all data: %d\r\n\r\n",UartRxDataPara.totalDataLen);
//		      for(i=0;i<(DATALEN.len+7);i++)
//	        {
//			      USARTDMA_TX_BUFF[i]=USARTDMA_RX_BUFF[i];
//		      }
//		      USARTx_DMA_Send_Enable(DATALEN.len+7);
			  }
				UartRxDataPara.dataaddr = DATA_RX_DATABUFFER;//�����Ѵ���uart_DataBuffer

				printf("save\r\n");		  
				
			  //���ô������ݴ�����
				HostCmd_Process();
				
				UartRxDataPara.dataaddr = DATA_RX_NO;				

			}
			else  //����β����
			{
				UartRxDataPara.state = USART_RX_STATE_RX_TAIL_FAIL;
				printf("����β����\r\n");
			}
	  }
		else   //����ͷ����
		{
			UartRxDataPara.state = USART_RX_STATE_RX_HEAD_FAIL;
			printf("����ͷ����\r\n");
		}
		
		//���ڿ���
		UartRxDataPara.state = USART_RX_STATE_IDLE;
		
		//��DMA����
		DMA_Cmd(USART_DMAx_RX_Streamx, ENABLE);
  }
} 
#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/
#if USARTx_DMA_ENALBE
//DMA���ͳ���,����һ��DMA����,Ҫ��ǰ׼���÷�������
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void USARTx_DMA_Send_Enable(uint16_t ndtr)
{

	DMA_Cmd(USART_DMAx_TX_Streamx, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(USART_DMAx_TX_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(USART_DMAx_TX_Streamx,ndtr);          //���ݴ�����  
	
	//���������������ɵ��жϱ�־
	DMA_ClearFlag(USART_DMAx_TX_Streamx,USART_DMAx_TX_IT_TCIFx); 
 
	DMA_Cmd(USART_DMAx_TX_Streamx, ENABLE);                      //����DMA���� 
}	
#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/


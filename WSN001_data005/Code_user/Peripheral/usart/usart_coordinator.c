/***************************************************************************/

//#include "head_file.h"
#include "usart_coordinator.h"
#include "wsn_hostcmd.h"

/***************************************************************************/
//使用printf打印到串口需要重定义fputc函数 
#if USARTx_PRINTF
int fputc(int ch, FILE *f)
{ 	
	while((USARTx->SR&0X40)==0);//循环发送,直到发送完毕   
	USARTx->DR = (u8) ch;      
	return ch;
}//注意,读取USARTx->SR能避免莫名其妙的错误  
#endif /*USARTx_PRINTF*/
/***************************************************************************/
//USART使用DMA收发相关变量
#if USARTx_DMA_ENALBE

//USART使用DMA发送接收的缓冲区
uint8_t USARTDMA_RX_BUFF[USART_DMA_RX_LEN];
uint8_t USARTDMA_TX_BUFF[USART_DMA_TX_LEN];

//接收使用的相关变量
UartRxDataParaStruct UartRxDataPara;
uint8_t uart_DataBuffer[USART_DMA_RX_LEN] = {0};
//构造长度数据共用体,用于16位长度数据与2个8位数据转化
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
//初始化USARTx

void usartx_init(void){

	//GPIO端口设置
  GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(USARTx_RCC_PORT, ENABLE); //使能GPIO时钟
	RCC_APB2PeriphClockCmd(USARTx_RCC_USART,ENABLE);//使能USART时钟
 
  //USARTx 初始化设置
	USART_InitStructure.USART_BaudRate = USARTx_Baudrate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USARTx_Mode;	//收发模式
  USART_Init(USARTx, &USART_InitStructure); //初始化串口
	
	//使能串口
	USART_Cmd(USARTx, ENABLE);

	//初始化串口后再进行IO口配置(串口初始化中有可能发出一个无效的0x00或0xfe)
	//USARTx GPIO配置
  GPIO_InitStructure.GPIO_Pin   = USARTx_PIN_TX | USARTx_PIN_RX; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //上拉
	GPIO_Init(USARTx_PORT,&GPIO_InitStructure); //初始化
	
	//串口对应引脚复用映射
	GPIO_PinAFConfig(USARTx_PORT,USARTx_GPIO_PinSource_TX,USARTx_AF); //GPIOA9复用为USART1_TX
	GPIO_PinAFConfig(USARTx_PORT,USARTx_GPIO_PinSource_RX,USARTx_AF); //GPIOA10复用为USART1_RX

  //清除TXE,防止第一个数据发送时被漏发
  USART_ClearFlag(USARTx,USART_FLAG_TC);

#if USARTx_DMA_ENALBE  //DMA中断接收需要配置USART总线空闲中断和DMA相应数据流传输中断
/*---------------------------------------------------------------------*/

  //开启USART总线空闲中断,当USART总线空闲时进入中断
  USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	
	//Usartx NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQN;//串口x中断通道//USART1_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USARTx_PreemptionPriority;   //抢占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = USARTx_SubPriority;	      	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	
	//配置DMA接收
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	USARTx_DMA_RX_Config();
	
	//配置DMA发送
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	USARTx_DMA_TX_Config();
	
	//配置串口状态
	UartRxDataPara.state        = USART_RX_STATE_IDLE;
  UartRxDataPara.data         = uart_DataBuffer;
	UartRxDataPara.totalDataLen = 0;
	UartRxDataPara.dataaddr     = DATA_RX_NO;
	
/*---------------------------------------------------------------------*/
#endif /*USARTx_DMA_ENALBE*/
}

/***************************************************************************/
#if USARTx_DMA_ENALBE

//DMA接收配置函数
void USARTx_DMA_RX_Config(void){
	
	DMA_InitTypeDef  DMA_InitStructure;
	
	//DMA时钟使能
	RCC_AHB1PeriphClockCmd(USART_DMAx_RCC,ENABLE);
  
	//复位初始化DMA数据流
  DMA_DeInit(USART_DMAx_RX_Streamx);
	
	//等待DMA数据流复位完成
	while (DMA_GetCmdStatus(USART_DMAx_RX_Streamx) != DISABLE){}
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel            = USART_DMAx_RX_Channel;        //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DMAx_RX_USARTAddr;      //DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr    = USART_DMAx_RX_MemoryAddr;     //DMA 存储器0地址
  DMA_InitStructure.DMA_DIR                = USART_DMAx_RX_DIR;            //存储器到外设模式
  DMA_InitStructure.DMA_BufferSize         = USART_DMAx_RX_BufferSize;     //数据传输量 
  DMA_InitStructure.DMA_PeripheralInc      = USART_DMAx_RX_PeripheralInc;  //外设非增量模式
  DMA_InitStructure.DMA_MemoryInc          = USART_DMAx_RX_MemoryInc;      //存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = USART_DMAx_RX_PeripheralDataSize;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize     = USART_DMAx_RX_MemoryDataSize; //存储器数据长度:8位
  DMA_InitStructure.DMA_Mode               = USART_DMAx_RX_Mode;           // 使用普通模式 
  DMA_InitStructure.DMA_Priority           = USART_DMAx_RX_Priority;       //中等优先级
  DMA_InitStructure.DMA_FIFOMode           = USART_DMAx_RX_FIFOMode;         
  DMA_InitStructure.DMA_FIFOThreshold      = USART_DMAx_RX_FIFOThreshold;
  DMA_InitStructure.DMA_MemoryBurst        = USART_DMAx_RX_MemoryBurst;    //存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst    = USART_DMAx_RX_PeripheralBurst;//外设突发单次传输
		
  DMA_Init(USART_DMAx_RX_Streamx, &DMA_InitStructure);//初始化DMA Stream
	
	//此处必须开启中断，否则DMA_GetITStatus(USART_DMAx_RX_Streamx,USART_DMAx_RX_IT_TCIFx)一直为0		
	DMA_ITConfig(USART_DMAx_RX_Streamx,DMA_IT_TC,ENABLE);
	
  //使能DMA接收
  DMA_Cmd(USART_DMAx_RX_Streamx, ENABLE);
}

//DMA发送配置函数
void USARTx_DMA_TX_Config(void){
	
	DMA_InitTypeDef  DMA_InitStructure;
	
	//DMA时钟使能
	RCC_AHB1PeriphClockCmd(USART_DMAx_RCC,ENABLE);
  
	//复位初始化DMA数据流
  DMA_DeInit(USART_DMAx_TX_Streamx);
	
	//等待DMA数据流复位完成
	while (DMA_GetCmdStatus(USART_DMAx_TX_Streamx) != DISABLE){}
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel            = USART_DMAx_TX_Channel;        //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DMAx_TX_USARTAddr;      //DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr    = USART_DMAx_TX_MemoryAddr;     //DMA 存储器0地址
  DMA_InitStructure.DMA_DIR                = USART_DMAx_TX_DIR;            //存储器到外设模式
  DMA_InitStructure.DMA_BufferSize         = USART_DMAx_TX_BufferSize;     //数据传输量 
  DMA_InitStructure.DMA_PeripheralInc      = USART_DMAx_TX_PeripheralInc;  //外设非增量模式
  DMA_InitStructure.DMA_MemoryInc          = USART_DMAx_TX_MemoryInc;      //存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = USART_DMAx_TX_PeripheralDataSize;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize     = USART_DMAx_TX_MemoryDataSize; //存储器数据长度:8位
  DMA_InitStructure.DMA_Mode               = USART_DMAx_TX_Mode;           // 使用普通模式 
  DMA_InitStructure.DMA_Priority           = USART_DMAx_TX_Priority;       //中等优先级
  DMA_InitStructure.DMA_FIFOMode           = USART_DMAx_TX_FIFOMode;         
  DMA_InitStructure.DMA_FIFOThreshold      = USART_DMAx_TX_FIFOThreshold;
  DMA_InitStructure.DMA_MemoryBurst        = USART_DMAx_TX_MemoryBurst;    //存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst    = USART_DMAx_TX_PeripheralBurst;//外设突发单次传输
		
  DMA_Init(USART_DMAx_TX_Streamx, &DMA_InitStructure);//初始化DMA Stream

  //此处必须开启中断，否则DMA_GetITStatus(USART_DMAx_TX_Streamx,USART_DMAx_TX_IT_TCIFx)一直为0		
	DMA_ITConfig(USART_DMAx_TX_Streamx,DMA_IT_TC,ENABLE);

  //使能DMA发送
  //DMA_Cmd(USART_DMAx_TX_Streamx, ENABLE);
	
}

#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#if USARTx_DMA_ENALBE
void USART1_IRQHandler(void){               	//串口1中断服务程序

  if(USART_GetITStatus(USARTx, USART_IT_IDLE) != RESET)  //USART总线空闲中断
  { 
		//顺序读取SR,DR寄存器,将IDLE标志位清0
		USARTx->SR;USARTx->DR;
		
		//关闭DMA接收
		DMA_Cmd(USART_DMAx_RX_Streamx, DISABLE);
		
		//清除数据流传输完成的中断标志
		DMA_ClearFlag(USART_DMAx_RX_Streamx,USART_DMAx_RX_IT_TCIFx);
		
		//接收数据处理程序
		if((USARTDMA_RX_BUFF[0]==0xAA)&(USARTDMA_RX_BUFF[1]==0x55))//验证数据头
		{
			//数据头正确
			UartRxDataPara.state = USART_RX_STATE_RX_HEAD_OK;
			
			//查询数据长度(数据性质+数据 的长度)
      DATALEN.BYTES.byte0 = USARTDMA_RX_BUFF[2];
		  DATALEN.BYTES.byte1 = USARTDMA_RX_BUFF[3];
		
			if((USARTDMA_RX_BUFF[DATALEN.len+4]==0x55)&(USARTDMA_RX_BUFF[DATALEN.len+5]==0xAA))//验证数据尾
		  {
				//数据尾正确
				UartRxDataPara.state    = USART_RX_STATE_RX_TAIL_OK;
				
				UartRxDataPara.dataaddr = DATA_RX_USART;//数据存在于USARTDMA_RX_BUFF
				UartRxDataPara.totalDataLen = DATALEN.len;//储存数据性质+数据 的长度,不包括数据头尾和长度
				
				//串口数据处理
				{
				  uint16_t i;
					for(i=0;i<(UartRxDataPara.totalDataLen);i++)//不拷贝数据头,数据长度,数据尾
	        {
			      uart_DataBuffer[i]=USARTDMA_RX_BUFF[i+4];//将数据拷贝到uart_DataBuffer等待处理
		      }
//				  printf("\r\nlen of all data: %d\r\n\r\n",UartRxDataPara.totalDataLen);
//		      for(i=0;i<(DATALEN.len+7);i++)
//	        {
//			      USARTDMA_TX_BUFF[i]=USARTDMA_RX_BUFF[i];
//		      }
//		      USARTx_DMA_Send_Enable(DATALEN.len+7);
			  }
				UartRxDataPara.dataaddr = DATA_RX_DATABUFFER;//数据已存入uart_DataBuffer

				printf("save\r\n");		  
				
			  //调用串口数据处理函数
				HostCmd_Process();
				
				UartRxDataPara.dataaddr = DATA_RX_NO;				

			}
			else  //数据尾错误
			{
				UartRxDataPara.state = USART_RX_STATE_RX_TAIL_FAIL;
				printf("数据尾错误\r\n");
			}
	  }
		else   //数据头错误
		{
			UartRxDataPara.state = USART_RX_STATE_RX_HEAD_FAIL;
			printf("数据头错误\r\n");
		}
		
		//串口空闲
		UartRxDataPara.state = USART_RX_STATE_IDLE;
		
		//打开DMA接收
		DMA_Cmd(USART_DMAx_RX_Streamx, ENABLE);
  }
} 
#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/
#if USARTx_DMA_ENALBE
//DMA发送程序,开启一次DMA传输,要提前准备好发送内容
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void USARTx_DMA_Send_Enable(uint16_t ndtr)
{

	DMA_Cmd(USART_DMAx_TX_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(USART_DMAx_TX_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(USART_DMAx_TX_Streamx,ndtr);          //数据传输量  
	
	//清除数据流传输完成的中断标志
	DMA_ClearFlag(USART_DMAx_TX_Streamx,USART_DMAx_TX_IT_TCIFx); 
 
	DMA_Cmd(USART_DMAx_TX_Streamx, ENABLE);                      //开启DMA传输 
}	
#endif /*USARTx_DMA_ENALBE*/
/***************************************************************************/


/***********************************************************************************/
//#include "head_file.h"
#include "dwm1000_api.h"
#include "delay_tim4.h"
#include "usart_coordinator.h"
/***********************************************************************************/

//node UWB configration
//UWB发送的消息为PHY payload,其中最后两个会被自动填充为FCS,所以要主动为IC芯片留出来

//static dwt_config_t uwb_node_std_config = {
//    2,               /* Channel number. *///选择收发信道
//    DWT_PRF_64M,     /* Pulse repetition frequency. *///选择重复频率
//    DWT_PLEN_256,    /* Preamble length. Used in TX only. *///上传网络前导码长度
//    DWT_PAC16,       /* Preamble acquisition chunk size. Used in RX only. *///下载网络前导码验证块大小
//    9,               /* TX preamble code. Used in TX only. *///上传网络参数,配合信道,重复频率选择
//    9,               /* RX preamble code. Used in RX only. *///下载网络参数,配合信道,重复频率选择
//    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. *///标准配置,默认选0
//    DWT_BR_6M8,      /* Data rate. *///发送速率
//    DWT_PHRMODE_STD, /* PHY header mode. *///标准配置,默认STD
//    (256 + 1 + 64 - 16) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
//};

//static dwt_config_t uwb_node_std_config = {
//    2,               /* Channel number. *///选择收发信道
//    DWT_PRF_64M,     /* Pulse repetition frequency. *///选择重复频率
//    DWT_PLEN_256,    /* Preamble length. Used in TX only. *///上传网络前导码长度
//    DWT_PAC16,       /* Preamble acquisition chunk size. Used in RX only. *///下载网络前导码验证块大小
//    9,               /* TX preamble code. Used in TX only. *///上传网络参数,配合信道,重复频率选择
//    12,              /* RX preamble code. Used in RX only. *///下载网络参数,配合信道,重复频率选择
//    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. *///标准配置,默认选0
//    DWT_BR_6M8,      /* Data rate. *///发送速率
//    DWT_PHRMODE_STD, /* PHY header mode. *///标准配置,默认STD
//    (256 + 1 + 64 - 16) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. *///下载sfd超时参数
//};

static dwt_config_t uwb_gate_std_config = {
    2,               /* Channel number. *///选择收发信道
    DWT_PRF_64M,     /* Pulse repetition frequency. *///选择重复频率
    DWT_PLEN_256,    /* Preamble length. Used in TX only. *///下发网络前导码长度
    DWT_PAC16,       /* Preamble acquisition chunk size. Used in RX only. *///上收网络前导码验证块大小
    12,              /* TX preamble code. Used in TX only. *///下发网络参数,配合信道,重复频率选择
    9,               /* RX preamble code. Used in RX only. *///上收网络参数,配合信道,重复频率选择
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. *///标准配置,默认选0
    DWT_BR_6M8,      /* Data rate. *///发送速率
    DWT_PHRMODE_STD, /* PHY header mode. *///标准配置,默认STD
    //(256 + 1 + 64 - 16) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. *///上收sfd超时参数
};

//panid和短地址位于MAC Header中,在帧过滤中会被使用
//static uint16 pan_id = 0xDECA;
//static uint16 short_addr = 0x5258; /* "RX" */
//static uint8  eui[]  = {'A', 'C', 'K', 'D', 'A', 'T', 'R', 'X'};

/* Receive response timeout, expressed in UWB microseconds (UUS, 1 uus = 512/499.2 us). See NOTE 3 below. */
//#define RX_RESP_TO_UUS 2200

/***********************************************************************************/

//dwm1000_init前需要进行延时初始化delay_tim3
void dwm1000_init(void)
{
	  DECA_WAKEUPSET();
	     
	  DECAIRQ_Configuration();  printf("irq is ok\r\n");
	  
	  SPI1_Configuration();     printf("spi is ok\r\n");
	
    deca_sleep(10);

    //可以用一个软件复位,复位前后要为PHA和POL置位选择SPI模式     dwt_softreset
    reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
	
	
	  /* Reset and initialise DW1000.
     * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum
     * performance. */
    spi_set_rate_low();
	
/*
 * For initialisation, DW1000 clocks must be temporarily set to crystal speed. 
 * After initialisation SPI rate can be increased for optimum performance. 
*/

    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR)
    {
       printf("DWT INIT FAILED\r\n");
        while (1);
    }
		
    spi_set_rate_high();
  
    dwt_configure(&uwb_gate_std_config);

			
//    dwt_setpanid(pan_id);
//    dwt_seteui(eui);
//    dwt_setaddress16(short_addr);

    /* Configure frame filtering. Only data frames are enabled in this example. Frame filtering must be enabled for Auto ACK to work. */
//    dwt_enableframefilter(DWT_FF_DATA_EN);

    /* Activate auto-acknowledgement. Time is set to 0 so that the ACK is sent as soon as possible after reception of a frame. */
//    dwt_enableautoack(0);		
		    /* Set delay to turn reception on immediately after transmission of the frame. See NOTE 6 below. */
//    dwt_setrxaftertxdelay(0);

    /* Set RX frame timeout for the response. */
//    dwt_setrxtimeout(RX_RESP_TO_UUS);
		
//		dwt_enablegpioclocks();//
//		dwt_setgpiodirection(GxM0,0);
//		
//    LED_RXOK_ON();

    dwt_setleds( DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK );//set led rx and tx
		
		printf("DWT INIT SUCCESSED\r\n");
}


/*********************************************************************************/
void EXTI0_IRQHandler(void)
{
    do
    {
        port_deca_isr();
    } while (port_CheckEXT_IRQ() == 1);
    /* Clear EXTI Line 5 Pending Bit */
    EXTI_ClearITPendingBit(DECAIRQ_EXTI);
}
/*********************************************************************************/

UWB_DATA_TypeDef  GATE_UWB;

static uint32 status_tx_reg = 0;
static uint32 status_rx_reg = 0;

void UWB_Send_Enable(void)
{
//	do
//	{
	//写数据
	dwt_writetxdata(sizeof(GATE_UWB.TXA.tx_msg), GATE_UWB.TXA.tx_msg, 0); 
	//数据格式与是否测距
  dwt_writetxfctrl(sizeof(GATE_UWB.TXA.tx_msg), 0, 0);

	//开始发送
  dwt_starttx(DWT_START_TX_IMMEDIATE);

	//发前读标志寄存器
//  status_tx_reg=dwt_read32bitreg(SYS_STATUS_ID);
//  printf("status_reg:%x\r\n",(unsigned int)status_tx_reg);
	
	//等待发送完成
  while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS));
	
	//发送后读标志寄存器
//	status_tx_reg=dwt_read32bitreg(SYS_STATUS_ID);
//	printf("status_reg:%x\r\n",(unsigned int)status_tx_reg);
	
	//发送完后清除标志位
  dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
	
	
//	dwt_rxenable(DWT_START_RX_IMMEDIATE);

//	//判断是否接收成功或者是接收错误
//  while (!((status_rx_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR))){}
//		
//	if(status_rx_reg & SYS_STATUS_RXFCG)//接收成功
//  {
//    GATE_UWB.frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
//    if (GATE_UWB.frame_len <= FRAME_LEN_MAX)
//    {
//			//将数据读到buf中
//      dwt_readrxdata(GATE_UWB.RXB.rx_buffer, GATE_UWB.frame_len, 0);
//			if((GATE_UWB.RXB.rx_buffer[2])=='a')break;
//    }
//		//写1清除成功标志
//    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);
//		
//  }
//	}while(status_rx_reg & SYS_STATUS_ALL_RX_ERR);
}

/*********************************************************************************/
void UWB_Receive_Enable(void)
{
	//立即开始接收
	dwt_rxenable(DWT_START_RX_IMMEDIATE);

	//接收前读标志寄存器
//  status_rx_reg=dwt_read32bitreg(SYS_STATUS_ID);
//  printf("status_reg_beforewhile:%x\r\n",(unsigned int)status_rx_reg);
	
	//判断是否接收成功或者是接收错误
  while (!((status_rx_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR))){}

	//接收后读标志寄存器
//  printf("status_reg_afterwhile:%x\r\n",(unsigned int)status_rx_reg);
			
	if(status_rx_reg & SYS_STATUS_RXFCG)//接收成功
  {
    GATE_UWB.frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
    if (GATE_UWB.frame_len <= FRAME_LEN_MAX)
    {
			//将数据读到buf中
      dwt_readrxdata(GATE_UWB.RXB.rx_buffer, GATE_UWB.frame_len, 0);
    }
		//写1清除成功标志
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);
		
//		//发送接收成功标志
//	  GATE_UWB.RXB.rx_buffer[2]='a';
//	  dwt_writetxdata(sizeof(GATE_UWB.TXA.tx_msg), GATE_UWB.TXA.tx_msg, 0); 
//	  //数据格式与是否测距
//    dwt_writetxfctrl(sizeof(GATE_UWB.TXA.tx_msg), 0, 0);
//	  //开始发送
//    dwt_starttx(DWT_START_TX_IMMEDIATE);
//	  //等待发送完成
//    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS));
//	  //发送完后清除标志位
//    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

  }
  else//接收失败
  {
//	  printf("fail:%x\r\n",(unsigned int)(status_reg & SYS_STATUS_ALL_RX_ERR));
		
		//错误也写1清除错误标志
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
  }
}
/*********************************************************************************/

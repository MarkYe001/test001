/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************//*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

	uint16_t i=1;
	
	uint16_t j=0;

	GATE_UWB.TXA.time = 0;

  for(i=2;i<16;i++)
	GATE_UWB.TXA.tx_msg[i] = 'H';
	
	i=1;
	
	Timedelay_tim4_s(1);
	
     while(1)
    {
			GATE_UWB.TXA.time = i ;
      GATE_UWB.TXA.tx_msg[2] = 'H';
			
			UWB_Send_Enable();
			  
			printf("send %d\r\n",i++);
			
      Timedelay_tim4_ms(10);//1s发送一次
			
			if(i==1749)break;
    }
		Timedelay_tim4_s(1);	
  while(1)
{
	
     UWB_Receive_Enable();
	   if(j!=GATE_UWB.RXB.time)
		 {
			if((GATE_UWB.RXB.time-j)>1)printf("%d-%d\r\n",j,GATE_UWB.RXB.time);
			j=GATE_UWB.RXB.time;
		 }
			if(GATE_UWB.RXB.time==1748){printf("over");break;}
	
}

/*********************************************************************/

//查时钟源和时钟频率
	uint8_t syssource;
	uint32_t sys,hclk,pclk1,pclk2;
	RCC_ClocksTypeDef aaa;
	
	syssource=RCC_GetSYSCLKSource();
	
	RCC_GetClocksFreq(&aaa);
	
	sys=aaa.SYSCLK_Frequency;
	hclk=aaa.HCLK_Frequency;
	pclk1=aaa.PCLK1_Frequency;
	pclk2=aaa.PCLK2_Frequency;
	
/*********************************************************************/

	uint8_t    IRQCLA = 0,IRQCLA1=0;
  uint8_t    FLAG1 = 0,FLAG2=0,FLAG3=0,FLAG4=0;
  uint8_t    IRQTIME = 0;
	uint8_t    SPITIME_T=0;
	uint8_t    SPITIME_R=0;

	uint8_t    theadbuffer1[]={0xff};
	uint8_t    rheadbuffer2[]={0xfa};
	
	uint8_t    SPI1tbuffer[10]={'1','2','3','4','5','6','7','8','9','A'};;
	uint8_t    SPI2tbuffer[9]= {'H','e','l','l','o','!','s','p','i'};

	uint8_t    SPI2rbuffer[10]={'0','0','0','0','0','0','0','0','0','0'};
	uint8_t    SPI1rbuffer[10] ={'0','0','0','0','0','0','0','0','0','0'};
	
	SPI2_Init();
	
	printf("SPI2rbuffer before\r\n");
	Timedelay_tim3_ms(20);
	usart1_showarray(SPI2rbuffer,10);
	Timedelay_tim3_ms(20);
	printf("\r\nSPI1rbuffer before\r\n");
	Timedelay_tim3_ms(20);
	usart1_showarray(SPI1rbuffer,10);
	Timedelay_tim3_ms(20);
	printf("\r\n");
	Timedelay_tim3_ms(20);

//	writetospi(1,theadbuffer1,10,SPI1tbuffer);
//	Timedelay_tim3_ms(20);
//	
//	printf("\r\nSPI2rbuffer after\r\n");
//	Timedelay_tim3_ms(20);
//	usart1_showarray(SPI2rbuffer,10);
//	Timedelay_tim3_ms(20);
//	printf("\r\n");
//	Timedelay_tim3_ms(20);
//	
//	printf("IRQTIME:%d\r\n",IRQTIME);
	printf("IRQTIME:%d   SPITIME_R:%d   SPITIME_T:%d\r\n",IRQTIME,SPITIME_R,SPITIME_T);
	printf("FLAG1:%d   FLAG2:%d   FLAG3:%d   FLAG4:%d\r\n",FLAG1,FLAG2,FLAG3,FLAG4);
	
	readfromspi(1,rheadbuffer2,10,SPI1rbuffer);
  Timedelay_tim3_ms(20);
	
	printf("\r\nSPI1rbuffer after\r\n");
	Timedelay_tim3_ms(20);
	usart1_showarray(SPI1rbuffer,10);
	Timedelay_tim3_ms(20);
	printf("\r\n");
	Timedelay_tim3_ms(20);

	printf("IRQTIME:%d   SPITIME_R:%d   SPITIME_T:%d\r\n",IRQTIME,SPITIME_R,SPITIME_T);
	printf("FLAG1:%d   FLAG2:%d   FLAG3:%d   FLAG4:%d\r\n",FLAG1,FLAG2,FLAG3,FLAG4);
/*********************************************************************/
  uint8_t    j;
	uint32_t   a1=0;
	uint8_t    headcnt=1;
	uint8_t    headbuffer[3]={0};
	uint8_t    length=4;
	uint8_t    buffer[4];
	
	init_all();
		//dwm1000_init();
	DECA_WAKEUPSET();
	DECAIRQ_Configuration();  printf("irq is ok\r\n");
	  
  SPI1_Configuration();     printf("spi is ok\r\n");
	
  deca_sleep(10);
	
	reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
                            printf("reset is ok\r\n");
	
	Timedelay_tim3_s(1);
//  spi_set_rate_low();      	printf("low is ok\r\n");
	readfromspi(headcnt, headbuffer, length, buffer);   printf("read is ok\r\n");
	for (j = 4 ; j > 0 ; j --)
    {
        a1 = (a1 << 8) + buffer[j-1] ;
    }                                                 printf("add is ok\r\n");

//	a1=dwt_readdevid();
	usart1_showuintx ("a1           ",a1,32);
	usart1_showuintx ("DWT_DEVICE_ID",DWT_DEVICE_ID,32);
/*********************************************************************/
		//	strcpy(myfile.FileName,"test.txt");
//	myfile.res=f_open (&(myfile.fsrc),myfile.FileName,FA_CREATE_ALWAYS|FA_WRITE);
//	f_write (&(myfile.fsrc),"This is a test.\r\nhahahaha",24,&(myfile.bw));
//	f_close (&(myfile.fsrc));
	uint8_t buffer1[]={0,1,2,3,4,5,6,7,8,9,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  uint8_t buffer2[]="12323";
	init_all();               
	

	
	Get_filename(&myfile,01,0001);
	CreateNewFile(&myfile);
	InputFile(&myfile,buffer1,32);

/*********************************************************************/
	//uint8_t buffer1[]={'A','B','C','D'};
	//uint8_t buffer2[]="0123456789";
	//char *buffer4="KLMNOPQRSTUVWXYZ";(uint8_t *)buffer4
	
  uint8_t l2,l3,l4,l5;
	char *buffer;
	uint8_t buffer1[]={'A','B','c','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  uint8_t buffer2[]="ABCDEFGHIJ";//"0123456789";
	uint8_t buffer3[]="ABCDEFGHIJ";
	char *buffer4="KLMNOPQRSTUVWXYZ";
	uint8_t buffer5[]="ABCdEFGHIJKLMNOPQRSTUVWXYZ";
	init_all();               

		buffer = (char *)buffer2;
	l2=sizeof(buffer2);
	l3=sizeof(buffer3);
	l4=16;
	l5=36;
	//l1,l3,l4
		
		printf("l2:%d,l3:%d,l4:%d,l5:%d\r\n",l2,l3,l4,l5);
		
		printf("buffer2[]:%d,buffer:%d,(uint8_t)buffer:%d\r\n",(int)buffer2,(int)buffer,(int)((uint8_t*)buffer));
	
//	Get_filename(&myfile,1,1);
//	CreateNewFile(&myfile);
//	InputFile(&myfile,buffer1,26);
	
	Get_filename(&myfile,1,1);
	CreateNewFile(&myfile);
	InputFile(&myfile,buffer2,l2);
	
	Get_filename(&myfile,1,2);
	CreateNewFile(&myfile);
	InputFile(&myfile,((uint8_t *)buffer),l2);
		
//	Get_filename(&myfile,1,3);
//	CreateNewFile(&myfile);
//	InputFile(&myfile,buffer3,l3);
	
	Get_filename(&myfile,1,4);
	CreateNewFile(&myfile);
	InputFile(&myfile,(uint8_t *)buffer4,l4);
	
//  Get_filename(&myfile,1,3);
//	InputFile(&myfile,buffer5,l5);
/*********************************************************************/
//uint8_t buffer1[]={'A','B','C','D'};
	//uint8_t buffer2[]="0123456789";
	//uint8_t buffer2[]="汉字也可以";
	//char *buffer4="KLMNOPQRSTUVWXYZ";(uint8_t *)buffer4
	
  uint8_t l2,l3,l4,l5;
	char *buffer;
	uint8_t buffer1[]={'A','B','c','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  uint8_t buffer2[]="汉字怎么样";
	uint8_t buffer3[]="ABCDEFGHIJ";
	char *buffer4="0000OPQRSTUVWXYZ";
	uint8_t buffer5[]="ABCdEFGHIJKLMNOPQRSTUVWXYZ";
	init_all();               

	l2=sizeof(buffer2);
	l3=sizeof(buffer3);
	l4=16;
	l5=36;
	//l1,l3,l4
		
		printf("l2:%d,l3:%d,l4:%d,l5:%d\r\n",l2,l3,l4,l5);
		
		printf("buffer2[]:%0x,buffer:%0x,(uint8_t)buffer:%0x\r\n",(int)buffer2,(int)buffer,(int)((uint8_t*)buffer));
	
	Get_filename(&myfile,1,1);
	CreateNewFile(&myfile);
	myfile.res=InputFile(&myfile,buffer1,26);
  if(myfile.res)printf("buffer1");
//	InputFile(&myfile,buffer2,l2);
	
	myfile.res=InputFile(&myfile,(uint8_t *)buffer4,l4);
	if(myfile.res)printf("buffer2");
//	Get_filename(&myfile,1,3);
//	CreateNewFile(&myfile);
//	InputFile(&myfile,buffer3,l3);
	
//	Get_filename(&myfile,1,4);
//	CreateNewFile(&myfile);
//	InputFile(&myfile,(uint8_t *)buffer4,l4);
	
//  Get_filename(&myfile,1,3);
//	InputFile(&myfile,buffer5,l5);
/*********************************************************************/
/*********************************************************************/
#include "head_file.h" //2020.8.16 15:25
/*********************************************************************/

/* The frame sent in this example is a data frame encoded as per the IEEE 802.15.4-2011 standard. It is a 21-byte frame composed of the following
 * fields:
 *     - byte 0/1: frame control (0x8861 to indicate a data frame using 16-bit addressing and requesting ACK).
 *     - byte 2: sequence number, incremented for each new frame.
 *     - byte 3/4: PAN ID (0xDECA)
 *     - byte 5/6: destination address, see NOTE 2 below.
 *     - byte 7/8: source address, see NOTE 2 below.
 *     - byte 9 to 18: MAC payload, see NOTE 1 below.
 *     - byte 19/20: frame check-sum, automatically set by DW1000. */
static uint8 tx_msg[] = {0x61, 0x88, 0, 0xCA, 0xDE, 'X', 'R', 'X', 'T', 'h', 'e', 'l', 'l', 'o', ',', 'k', 'u', 'g', 'o', 0, 0};
/* Index to access the sequence number and frame control fields in frames sent and received. */
#define FRAME_FC_IDX 0
#define FRAME_SN_IDX 2
/* ACK frame control value. */
#define ACK_FC_0 0x02
#define ACK_FC_1 0x00

/* Inter-frame delay period, in milliseconds. */
#define TX_DELAY_MS 1000

/* Buffer to store received frame. See NOTE 4 below. */
#define ACK_FRAME_LEN 5
static uint8 rx_buffer[ACK_FRAME_LEN];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 status_reg = 0;

/* Hold copy of frame length of frame received (if good) so that it can be examined at a debug breakpoint. */
static uint16 frame_len = 0;

/* ACK status for last transmitted frame. */
static int tx_frame_acked = 0;

/* Counters of frames sent, frames ACKed and frame retransmissions. See NOTE 1 below. */
static uint32 tx_frame_nb = 0;
static uint32 tx_frame_ack_nb = 0;
static uint32 tx_frame_retry_nb = 0;

/**
 * Application entry point.
 */
int main(void)
{
   init_all();

	 dwm1000_init();
	
    /* Loop forever transmitting data. */
    while (1)
    {
        /* TESTING BREAKPOINT LOCATION #1 */

        /* Write frame data to DW1000 and prepare transmission. See NOTE 7 below.*/
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0); /* Zero offset in TX buffer. */
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0); /* Zero offset in TX buffer, no ranging. */

        /* Start transmission, indicating that a response is expected so that reception is enabled immediately after the frame is sent. */
        dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);

        /* We assume that the transmission is achieved normally, now poll for reception of a frame or error/timeout. See NOTE 8 below. */
        while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
        { };

        if (status_reg & SYS_STATUS_RXFCG)
        {
            /* Clear good RX frame event in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

            /* A frame has been received, check frame length is correct for ACK, then read and verify the ACK. */
            frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
            if (frame_len == ACK_FRAME_LEN)
            {
                dwt_readrxdata(rx_buffer, frame_len, 0);

                /* Check if it is the expected ACK. */
                if ((rx_buffer[FRAME_FC_IDX] == ACK_FC_0) && (rx_buffer[FRAME_FC_IDX + 1] == ACK_FC_1)
                    && (rx_buffer[FRAME_SN_IDX] == tx_msg[FRAME_SN_IDX]))
                {
                    tx_frame_acked = 1;
									  printf("get ack successed! \r\n");
                }
            }
        }
        else
        {
            /* Clear RX error/timeout events in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
        }

        /* Update number of frames sent. */
        tx_frame_nb++;
				printf("tx_frame_nb:%d\r\n",(int)tx_frame_nb);
				printf("tx_frame_acked:%d\r\n",(int)tx_frame_acked);
				Timedelay_tim4_s(2);

        if (tx_frame_acked)
        {
            /* Execute a delay between transmissions. See NOTE 1 below. */
            sleep_ms(TX_DELAY_MS);

            /* Increment the sent frame sequence number (modulo 256). */
            tx_msg[FRAME_SN_IDX]++;

            /* Update number of frames acknowledged. */
            tx_frame_ack_nb++;
        }
        else
        {
            /* Update number of retransmissions. */
            tx_frame_retry_nb++;
        }

        /* Reset acknowledged frame flag. */
        tx_frame_acked = 0;
    }
}




/*********************************************************************/
/*********************************************************************/
#include "head_file.h" //2020.8.16 15:25
/*********************************************************************/

int main(void)
{
   init_all();
	
	
	
	
	
	
	return 0;
	
}
/*********************************************************************/
/*********************************************************************/
#include "head_file.h" //2020.8.22 15:36
/*********************************************************************/

//static uint8 tx_msg[] = {0x61, 0x88, 0, 0xCA, 0xDE, 'X', 'R', 'X', 'T', 'h', 'e', 'l', 'l', 'o', ',', 'k', 'u', 'g', 'o', 0, 0};

static uint8 tx_msg[]={'M','o','l','i','a','m','!'};

int main(void)
{
	 uint8_t i=0;
	
   init_all();

	 dwm1000_init();
	
   while(1)
    {
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0); //写数据
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0);//数据格式与是否测距
			
        dwt_starttx(DWT_START_TX_IMMEDIATE);//开始发送
			
        while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS));//等待发送完成
			
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);//清除标志位
			  
			  printf("send %d\r\n",i++);
			
      //Timedelay_tim4_s(1);//1s发送一次
    }
}


/*********************************************************************/
/*********************************************************************/
#include "head_file.h" //2020.8.22 15:36
/*********************************************************************/

//static uint8 tx_msg[] = {0x61, 0x88, 0, 0xCA, 0xDE, 'X', 'R', 'X', 'T', 'h', 'e', 'l', 'l', 'o', ',', 'k', 'u', 'g', 'o', 0, 0};

static uint8 tx_msg[]={'M','o','l','i','a','m','!','M','o','l','i','a','m','!'};
static uint32 status_reg = 0;
int main(void)
{
	 uint8_t i=0;
	
   init_all();

	 dwm1000_init();
	
   while(1)
    {
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0); //写数据
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0);//数据格式与是否测距
			
        dwt_starttx(DWT_START_TX_IMMEDIATE);//开始发送
			//  printf("???\r\n");
			status_reg=dwt_read32bitreg(SYS_STATUS_ID);
		  	printf("status_reg:%x\r\n",(unsigned int)status_reg);
        while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS));//等待发送完成
			status_reg=dwt_read32bitreg(SYS_STATUS_ID);
		  	printf("status_reg:%x\r\n",(unsigned int)status_reg);
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);//清除标志位
			  
			  printf("send %d\r\n",i++);
			
      Timedelay_tim4_s(1);//1s发送一次
    }
}

/*********************************************************************/
/*********************************************************************/
#include "head_file.h" //2020.8.16 15:25
/*********************************************************************/

/* Example application name and version to display on LCD screen. */
#define APP_NAME "ACK DATA TX v1.0"

/* Default communication configuration. We use here EVK1000's default mode (mode 3). See NOTE 1 below. */
static dwt_config_t config = {
    2,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_110K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* The frame sent in this example is a data frame encoded as per the IEEE 802.15.4-2011 standard. It is a 21-byte frame composed of the following
 * fields:
 *     - byte 0/1: frame control (0x8861 to indicate a data frame using 16-bit addressing and requesting ACK).
 *     - byte 2: sequence number, incremented for each new frame.
 *     - byte 3/4: PAN ID (0xDECA)
 *     - byte 5/6: destination address, see NOTE 2 below.
 *     - byte 7/8: source address, see NOTE 2 below.
 *     - byte 9 to 18: MAC payload, see NOTE 1 below.
 *     - byte 19/20: frame check-sum, automatically set by DW1000. */
static uint8 tx_msg[] = {0x61, 0x88, 0, 0xCA, 0xDE, 'X', 'R', 'X', 'T', 'm', 'a', 'c', 'p', 'a', 'y', 'l', 'o', 'a', 'd', 0, 0};
/* Index to access the sequence number and frame control fields in frames sent and received. */
#define FRAME_FC_IDX 0
#define FRAME_SN_IDX 2
/* ACK frame control value. */
#define ACK_FC_0 0x02
#define ACK_FC_1 0x00

/* Inter-frame delay period, in milliseconds. */
#define TX_DELAY_MS 1000

/* Receive response timeout, expressed in UWB microseconds (UUS, 1 uus = 512/499.2 祍). See NOTE 3 below. */
#define RX_RESP_TO_UUS 2200

/* Buffer to store received frame. See NOTE 4 below. */
#define ACK_FRAME_LEN 5
static uint8 rx_buffer[ACK_FRAME_LEN];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 status_reg = 0;

/* Hold copy of frame length of frame received (if good) so that it can be examined at a debug breakpoint. */
static uint16 frame_len = 0;

/* ACK status for last transmitted frame. */
static int tx_frame_acked = 0;

/* Counters of frames sent, frames ACKed and frame retransmissions. See NOTE 1 below. */
static uint32 tx_frame_nb = 0;
static uint32 tx_frame_ack_nb = 0;
static uint32 tx_frame_retry_nb = 0;

/**
 * Application entry point.
 */
int main(void)
{
	 
	 uint8_t i=0;
   init_all();

    /* Start with board specific hardware init. */
    peripherals_init();
	
	  printf(APP_NAME);printf("\r\n");

    reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
    spi_set_rate_low();
    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR)
    {
        printf("DWT INIT FAILED\r\n");
        while (1)
        { };
    }
    spi_set_rate_high();

    /* Configure DW1000. See NOTE 5 below. */
    dwt_configure(&config);
		
		dwt_setleds( DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK );

    /* Set delay to turn reception on immediately after transmission of the frame. See NOTE 6 below. */
    dwt_setrxaftertxdelay(0);

    /* Set RX frame timeout for the response. */
    dwt_setrxtimeout(RX_RESP_TO_UUS);
		
		printf("DWT INIT SUCCESSED\r\n");

    /* Loop forever transmitting data. */
    while (1)
    {
        /* TESTING BREAKPOINT LOCATION #1 */

        /* Write frame data to DW1000 and prepare transmission. See NOTE 7 below.*/
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0); /* Zero offset in TX buffer. */
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0); /* Zero offset in TX buffer, no ranging. */

        /* Start transmission, indicating that a response is expected so that reception is enabled immediately after the frame is sent. */
        dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);

			  printf("send %d\r\n",i++);
			
        /* We assume that the transmission is achieved normally, now poll for reception of a frame or error/timeout. See NOTE 8 below. */
        while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
        { };
				

        if (status_reg & SYS_STATUS_RXFCG)
        {
            /* Clear good RX frame event in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

            /* A frame has been received, check frame length is correct for ACK, then read and verify the ACK. */
            frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
            if (frame_len == ACK_FRAME_LEN)
            {
                dwt_readrxdata(rx_buffer, frame_len, 0);

                /* Check if it is the expected ACK. */
                if ((rx_buffer[FRAME_FC_IDX] == ACK_FC_0) && (rx_buffer[FRAME_FC_IDX + 1] == ACK_FC_1)
                    && (rx_buffer[FRAME_SN_IDX] == tx_msg[FRAME_SN_IDX]))
                {
                    tx_frame_acked = 1;
									printf("ack get\r\n");
                }
            }
        }
        else
        {
					 printf("fail resson:%x\r\n",(unsigned int)(status_reg & (SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)));
            /* Clear RX error/timeout events in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
        }

        /* Update number of frames sent. */
        tx_frame_nb++;

        if (tx_frame_acked)
        {
            /* Execute a delay between transmissions. See NOTE 1 below. */
            sleep_ms(TX_DELAY_MS);

            /* Increment the sent frame sequence number (modulo 256). */
            tx_msg[FRAME_SN_IDX]++;

            /* Update number of frames acknowledged. */
            tx_frame_ack_nb++;
        }
        else
        {
            /* Update number of retransmissions. */
            tx_frame_retry_nb++;
        }

        /* Reset acknowledged frame flag. */
        tx_frame_acked = 0;
    }
}


/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

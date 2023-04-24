/*********************************************************************/
#include "head_file.h" //2020.8.16 15:25
/*********************************************************************/
//注意:186号warning被设置忽略
//float为4字节  浮点除法必须被除数是float
/*********************************************************************/
//开始任务

#define START_TASK_PRIO			5         //开始任务任务优先级
#define START_TK_SIZE 			1024       //任务堆栈大小
TaskHandle_t StartTask_Handler;       //任务句柄
void start_task(void *pvParameters);  //任务函数

#define TEST_TASK_PRIO     2
#define TEST_TK_SIZE       1024
TaskHandle_t   TESTTask_Handler;      //测试任务句柄
void test_task(void *pvParameters);   //测试任务函数

extern uint8_t    dwm1000_rx_data[128];
extern uint16_t   dwm1000_rx_data_len;
extern  UWB_DATA_TypeDef  GATE_UWB;
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
static uint8 tx_msg[] = {0x61, 0x88, 0, 0xCA, 0xDE, 'X', 'R', 'X', 'T', 'm', 'a', 'c', 'p', 'a', 'y', 'l', 'o', 'a', 'd', 0, 0};
/* Index to access the sequence number and frame control fields in frames sent and received. */
#define FRAME_FC_IDX 0
#define FRAME_SN_IDX 2
/* ACK frame control value. */
#define ACK_FC_0 0x02
#define ACK_FC_1 0x00

/* Inter-frame delay period, in milliseconds. */
#define TX_DELAY_MS 1000

/* Receive response timeout, expressed in UWB microseconds (UUS, 1 uus = 512/499.2 ?). See NOTE 3 below. */
#define RX_RESP_TO_UUS 5000

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

/*********************************************************************/	
int main(void)
{
/*********************************************************************/	
//优先级位数设置,根据rtos需求设置为4
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

/*********************************************************************/	
//普通外设初始化

	//串口初始化(调试)
	usartx_init();         printf("\r\n\r\n");   printf("usart1_init is over!\r\n");

	//定时器初始化(普通定时)
	Timedelay_tim4_init();     printf("delay_init  is over!\r\n");

/*********************************************************************/	
//采集,文件系统,无线通讯模块初始化

//	//文件系统初始化
//  FileSystem_Init();     printf("FileSystem_Init is over!\r\n");
//	                       printf("init_all    is over!\r\n");
												 
	//无线模块初始化
	dwm1000_init();
	
  //WSN初始化
//	ACQ_Temp.ACQ_Mode       = ACQ_Mode_FixedLength;    /*!< 采集模式 */
//	ACQ_Temp.NodeID         = 0;                       /*!< 节点ID */
//	ACQ_Temp.FileID         = 1;                       /*!< 文件ID */
//	ACQ_Temp.ACQ_CoupleMode = ACQ_CoupleMode_AC;       /*!< 耦合方式选择 */
//	ACQ_Temp.Frequence      = 1024 ;                   /*!< 采样率 */
//	ACQ_Temp.ACQ_Length     = 10240  ;                 /*!< 定长采样长度 */	
	
/*********************************************************************/

  //创建相关任务
  xTaskCreate((TaskFunction_t )start_task,           
              (const char*    )"start_task",         
              (uint16_t       )START_TK_SIZE,        
              (void*          )NULL,                 
              (UBaseType_t    )START_TASK_PRIO,      
              (TaskHandle_t*  )&StartTask_Handler);   

  //开启任务调度							
  vTaskStartScheduler();   

/*********************************************************************/
}

//开始任务函数
void start_task(void *pvParameters)
{
	//进入临界区
  taskENTER_CRITICAL();         
	
/*********************************************************************/
	xTaskCreate((TaskFunction_t )test_task,
              (const char *   )"test_task",
              (uint16_t       )TEST_TK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )TEST_TASK_PRIO,
              (TaskHandle_t*  )&TESTTask_Handler);
  printf("test_task\r\n");
	
/*********************************************************************/
  
	//删除当前任务,会在函数尾端立即进行任务切换						
	vTaskDelete(NULL); 						
							
	//退出临界区,上行任务切换因临界区而被推迟,临界区执行完后立即进行任务切换
  taskEXIT_CRITICAL();

}

//	xTaskCreate((TaskFunction_t )test_task,
//              (const char *   )"test_task",
//              (uint16_t       )TEST_TK_SIZE,
//              (void *         )NULL,
//              (UBaseType_t    )TEST_TASK_PRIO,
//              (TaskHandle_t*  )&TESTTask_Handler);


void test_task(void *pvParameters)
{
    /* Set delay to turn reception on immediately after transmission of the frame. See NOTE 6 below. */
    dwt_setrxaftertxdelay(0);

    /* Set RX frame timeout for the response. */
    dwt_setrxtimeout(RX_RESP_TO_UUS);

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
					
								printf("len:%d	sn:%d\r\n",frame_len,rx_buffer[FRAME_SN_IDX]);

                /* Check if it is the expected ACK. */
                if ((rx_buffer[FRAME_FC_IDX] == ACK_FC_0) && (rx_buffer[FRAME_FC_IDX + 1] == ACK_FC_1)
                    && (rx_buffer[FRAME_SN_IDX] == tx_msg[FRAME_SN_IDX]))
                {
                    tx_frame_acked = 1;
//									printf("len:%d	sn:%d\r\n",frame_len,rx_buffer[FRAME_SN_IDX]);
                }
            }
						
        }
        else
        {
            /* Clear RX error/timeout events in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
            //printf("ack wrong\r\n");
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
		
	while(1){}
	
}

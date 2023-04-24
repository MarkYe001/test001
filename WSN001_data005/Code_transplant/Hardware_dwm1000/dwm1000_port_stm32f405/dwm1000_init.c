/*********************************************************************/
//#include "head_file.h"
#include "dwm1000_init.h"
#include "delay_tim4.h"
#include "usart_coordinator.h"
/*********************************************************************/
/* Example application name and version to display on PC screen. */
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

/* Receive response timeout, expressed in UWB microseconds (UUS, 1 uus = 512/499.2 us). See NOTE 3 below. */
#define RX_RESP_TO_UUS 2200

//dwm1000_init前需要进行延时初始化delay_tim3
void dwm1000_init(void)
{
	  DECA_WAKEUPSET();
	     /* Start with board specific hardware init. */
	  DECAIRQ_Configuration();  printf("irq is ok\r\n");
	  
	  SPI1_Configuration();     printf("spi is ok\r\n");
	
    deca_sleep(10);

    /* Display application name on PC. */
    printf(APP_NAME);printf("\r\n");

    /* Reset and initialise DW1000.
     * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum
     * performance. */
    reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
    spi_set_rate_low();
    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR)
    {
       printf("DWT INIT FAILED\r\n");
        while (1);
    }
		
    spi_set_rate_high();

    /* Configure DW1000. See NOTE 5 below. */
    dwt_configure(&config);
		
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

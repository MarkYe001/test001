/*********************************************************************/

//HW specific definitions and functions for portability

#include "sleep.h"
#include "deca_port_stm32f405.h"

/*********************************************************************/

#define NULL ((void *)0UL)
	
/* DW1000 IRQ handler definition. */
port_deca_isr_t port_deca_isr = NULL;
/*******************************************************************************/

void DECA_WAKEUPSET(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(DECA_WAKEUP_RCC, ENABLE);//使能GPIO时钟
	
	GPIO_InitStructure.GPIO_Pin   = DECA_WAKEUP_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(DECA_WAKEUP_GPIO, &GPIO_InitStructure);//初始化
		
  GPIO_ResetBits(DECA_WAKEUP_GPIO, DECA_WAKEUP_PIN);
}	
/*******************************************************************************/

void DECA_SPIMODESET(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SPIx_MODE_PHA_RCC, ENABLE);//使能GPIO时钟
	RCC_AHB1PeriphClockCmd(SPIx_MODE_POL_RCC, ENABLE);//使能GPIO时钟
	
	GPIO_InitStructure.GPIO_Pin   = SPIx_MODE_PHA_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SPIx_MODE_PHA_GPIO, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin   = SPIx_MODE_POL_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SPIx_MODE_POL_GPIO, &GPIO_InitStructure);//初始化
	
	if(DECA_SPIPHA==0)GPIO_ResetBits(SPIx_MODE_PHA_GPIO, SPIx_MODE_PHA_PIN);
	else              GPIO_SetBits  (SPIx_MODE_PHA_GPIO, SPIx_MODE_PHA_PIN);
	
	if(DECA_SPIPOL==0)GPIO_ResetBits(SPIx_MODE_POL_GPIO, SPIx_MODE_POL_PIN);
	else              GPIO_SetBits  (SPIx_MODE_POL_GPIO, SPIx_MODE_POL_PIN);
	
}

void DECA_SPIMODEover(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SPIx_MODE_PHA_RCC, ENABLE);//使能GPIO时钟
	RCC_AHB1PeriphClockCmd(SPIx_MODE_POL_RCC, ENABLE);//使能GPIO时钟
	
	GPIO_InitStructure.GPIO_Pin   = SPIx_MODE_PHA_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SPIx_MODE_PHA_GPIO, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin   = SPIx_MODE_POL_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SPIx_MODE_POL_GPIO, &GPIO_InitStructure);//初始化
	
}
/*******************************************************************************/

int SPI1_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(SPIx_CS_RCC,         ENABLE);
	RCC_AHB1PeriphClockCmd(SPIx_GPIO_RCC,       ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
	
	GPIO_InitStructure.GPIO_Pin   = SPIx_CS;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//上拉
  GPIO_Init(SPIx_CS_GPIO, &GPIO_InitStructure);//初始化
	
		// Set CS high
	GPIO_SetBits(SPIx_CS_GPIO, SPIx_CS);
	
	GPIO_InitStructure.GPIO_Pin   = SPIx_SCK|SPIx_MISO|SPIx_MOSI;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//上拉
  GPIO_Init(SPIx_GPIO, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(SPIx_GPIO,SPIx_SCK_PinSource, GPIO_AF_SPI1); //PA5复用为 SPI1_SCK
	GPIO_PinAFConfig(SPIx_GPIO,SPIx_MISO_PinSource,GPIO_AF_SPI1); //PA6复用为 SPI1_MISO
	GPIO_PinAFConfig(SPIx_GPIO,SPIx_MOSI_PinSource,GPIO_AF_SPI1); //PA7复用为 SPI1_MOSI
	
	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = DECA_SPIPOL;		//串行同步时钟的空闲状态
	SPI_InitStructure.SPI_CPHA = DECA_SPIPHA;	  //串行同步时钟的第?个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPIx_PRESCALER;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设

    return 0;
}
/*******************************************************************************/

void SPI_ChangeRate(uint16_t scalingfactor)
{
	uint16_t tmpreg = 0;

	/* Get the SPIx CR1 value */
	tmpreg = SPIx->CR1;

	/*clear the scaling bits*/
	tmpreg &= 0xFFC7;

	/*set the scaling bits*/
	tmpreg |= scalingfactor;

	/* Write to SPIx CR1 */
	SPIx->CR1 = tmpreg;
}

void SPI_ConfigFastRate(uint16_t scalingfactor)
{
	SPI_InitTypeDef SPI_InitStructure;

	SPI_I2S_DeInit(SPIx);

	// SPIx Mode setup
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = DECA_SPIPOL;	 
	SPI_InitStructure.SPI_CPHA = DECA_SPIPHA;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = scalingfactor; //sets BR[2:0] bits - baudrate in SPI_CR1 reg bits 4-6
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPIx, &SPI_InitStructure);

	// Enable SPIx
	SPI_Cmd(SPIx, ENABLE);
}

//Set SPI rate to less than 3 MHz to properly perform DW1000 initialisation.
void spi_set_rate_low (void)
{
    SPI_ChangeRate(SPI_BaudRatePrescaler_32);
}


//Set SPI rate as close to 20 MHz as possible for optimum performances.
void spi_set_rate_high (void)
{
    SPI_ChangeRate(SPI_BaudRatePrescaler_4);
}


/*******************************************************************************/

void setup_DW1000RSTnIRQ(int enable)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_DW1000_RSTn_GPIO,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	if(enable)
	{
		
		// Enable GPIO used as DECA IRQ for interrupt
		GPIO_InitStructure.GPIO_Pin   = DECARSTIRQ;	
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		//GPIO_InitStructure.GPIO_PuPd = 	GPIO_Mode_DOWN;	//IRQ pin should be Pull Down to prevent unnecessary EXT IRQ while DW1000 goes to sleep mode
		GPIO_Init(DECARSTIRQ_GPIO, &GPIO_InitStructure);

		/* Connect EXTI Line to GPIO Pin */
		SYSCFG_EXTILineConfig(DECARSTIRQ_EXTI_PORT, DECARSTIRQ_EXTI_PIN);

		/* Configure EXTI line */
		EXTI_InitStructure.EXTI_Line    = DECARSTIRQ_EXTI;
		EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//MP IRQ polarity is high by default
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		/* Enable and set EXTI Interrupt to the lowest priority */
		NVIC_InitStructure.NVIC_IRQChannel = DECARSTIRQ_EXTI_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DECARSTIRQ_IRQn_PreemptionPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = DECARSTIRQ_IRQn_SubPriority;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

		NVIC_Init(&NVIC_InitStructure);
	}
	else
	{
		//put the pin back to tri-state ... as input
		GPIO_InitStructure.GPIO_Pin   = DW1000_RSTn;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_Init(DW1000_RSTn_GPIO, &GPIO_InitStructure);

		/* Configure EXTI line */
		EXTI_InitStructure.EXTI_Line = DECARSTIRQ_EXTI;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//MP IRQ polarity is high by default
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
}

/*------------------------------------------------------------*/
//Checks whether the specified EXTI line is enabled or not.
//EXTI_Line: specifies the EXTI line to check.
//This parameter can be:
//EXTI_Linex: External interrupt line x where x(0..19)
//The "enable" state of EXTI_Line (SET or RESET).

ITStatus EXTI_GetITEnStatus(uint32_t EXTI_Line)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* Check the parameters */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));

  enablestatus =  EXTI->IMR & EXTI_Line;
  if (enablestatus != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

int DECAIRQ_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_DECAIRQ_GPIO,      ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	// Enable GPIO used as DECA IRQ for interrupt
	GPIO_InitStructure.GPIO_Pin  = DECAIRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//IRQ pin should be Pull Down to prevent unnecessary EXT IRQ while DW1000 goes to sleep mode
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(DECAIRQ_GPIO, &GPIO_InitStructure);

	/* Connect EXTI Line to GPIO Pin */
	SYSCFG_EXTILineConfig(DECAIRQ_EXTI_PORT, DECAIRQ_EXTI_PIN);

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = DECAIRQ_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//MPW3 IRQ polarity is high by default
	EXTI_InitStructure.EXTI_LineCmd = DECAIRQ_EXTI_USEIRQ;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = DECAIRQ_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DECAIRQ_IRQn_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = DECAIRQ_IRQn_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DECAIRQ_EXTI_USEIRQ;

	NVIC_Init(&NVIC_InitStructure);

	return 0;
}

int NVIC_DisableDECAIRQ(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = DECAIRQ_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//MPW3 IRQ polarity is high by default
	EXTI_InitStructure.EXTI_LineCmd = DECAIRQ_EXTI_NOIRQ;
	EXTI_Init(&EXTI_InitStructure);

	return 0;
}

int is_IRQ_enabled(void)
{
	return ((   NVIC->ISER[((uint32_t)(DECAIRQ_EXTI_IRQn) >> 5)]
	           & (uint32_t)0x01 << (DECAIRQ_EXTI_IRQn & (uint8_t)0x1F)  ) ? 1 : 0) ;
}

/*------------------------------------------------------------*/
//This function is used to install the handling function for DW1000 IRQ.
// NOTE:
//   - As EXTI9_5_IRQHandler does not check that port_deca_isr is not null, the user application must ensure that a
//     proper handler is set by calling this function before any DW1000 IRQ occurs!
//   - This function makes sure the DW1000 IRQ line is deactivated while the handler is installed.
//deca_isr function pointer to DW1000 interrupt handler to install

void port_set_deca_isr(port_deca_isr_t deca_isr)
{
    /* Check DW1000 IRQ activation status. */
    ITStatus en = port_GetEXT_IRQStatus();

    /* If needed, deactivate DW1000 IRQ during the installation of the new handler. */
    if (en)
    {
        port_DisableEXT_IRQ();
    }
    port_deca_isr = deca_isr;
    if (en)
    {
        port_EnableEXT_IRQ();
    }
}
/*******************************************************************************/

void reset_DW1000(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_DW1000_RSTn_GPIO, ENABLE);

	DECA_SPIMODESET();
	// Enable GPIO used for DW1000 reset
	GPIO_InitStructure.GPIO_Pin   = DW1000_RSTn;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DW1000_RSTn_GPIO, &GPIO_InitStructure);

	//drive the RSTn pin low
	GPIO_ResetBits(DW1000_RSTn_GPIO, DW1000_RSTn);
	sleep_ms(2);

	//put the pin back to tri-state ... as input
	GPIO_InitStructure.GPIO_Pin = DW1000_RSTn;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(DW1000_RSTn_GPIO, &GPIO_InitStructure);

  sleep_ms(2);
	
	//DECA_SPIMODEover();
	
}
/*******************************************************************************/

void peripherals_init (void)
{
//提前开启Time_delay
	DECAIRQ_Configuration();
	SPI1_Configuration();
  sleep_ms(10);
}
/*******************************************************************************/

/*********************************************************************/
//HW specific definitions and functions for portability
/*********************************************************************/
#ifndef PORT_H_
#define PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "task_priority_cofig.h"
	
//#define NULL ((void *)0UL)
/*******************************************************************************/
#define DECA_WAKEUP_GPIO      GPIOA
#define DECA_WAKEUP_PIN		    GPIO_Pin_0										
#define DECA_WAKEUP_RCC       RCC_AHB1Periph_GPIOA  	

void DECA_WAKEUPSET(void);
/*******************************************************************************/
//Configure DECA SPI MODE
															
#define SPIx_MODE_PHA_GPIO    GPIOB
#define SPIx_MODE_PHA_PIN     GPIO_Pin_10
#define SPIx_MODE_PHA_RCC		  RCC_AHB1Periph_GPIOB										

#define SPIx_MODE_POL_GPIO    GPIOA
#define SPIx_MODE_POL_PIN     GPIO_Pin_3
#define SPIx_MODE_POL_RCC			RCC_AHB1Periph_GPIOA	
															
#define DECA_SPIPOL           SPI_CPOL_High	  	//串行同步时钟的空闲状态为高电平
//#define DECA_SPIPOL           SPI_CPOL_Low	   	//串行同步时钟的空闲状态为低电平
#define DECA_SPIPHA           SPI_CPHA_2Edge 	  //串行同步时钟的第二个跳变沿（上升或下降）数据被采样 
//#define DECA_SPIPHA           SPI_CPHA_1Edge  	//串行同步时钟的第一个跳变沿（上升或下降）数据被采样 

void DECA_SPIMODESET(void);
void DECA_SPIMODEover(void);	
/*******************************************************************************/

#define SPIx_CS_GPIO		        	GPIOA
#define SPIx_CS					        	GPIO_Pin_4
#define SPIx_CS_RCC       	      RCC_AHB1Periph_GPIOA
	
#define SPIx_PRESCALER	        	SPI_BaudRatePrescaler_256

#define SPIx					           	SPI1
#define SPIx_GPIO				        	GPIOA
#define SPIx_GPIO_RCC       	    RCC_AHB1Periph_GPIOA
#define SPIx_SCK					        GPIO_Pin_5
#define SPIx_MISO				        	GPIO_Pin_6
#define SPIx_MOSI				        	GPIO_Pin_7
#define SPIx_SCK_PinSource       	GPIO_PinSource5
#define SPIx_MISO_PinSource       GPIO_PinSource6
#define SPIx_MOSI_PinSource       GPIO_PinSource7
	
int SPI1_Configuration(void);
	
#define port_SPIx_busy_sending()		    (SPI_I2S_GetFlagStatus((SPIx),(SPI_I2S_FLAG_TXE))==(RESET))
#define port_SPIx_no_data()			  	    (SPI_I2S_GetFlagStatus((SPIx),(SPI_I2S_FLAG_RXNE))==(RESET))
#define port_SPIx_send_data(x)	 	    	SPI_I2S_SendData((SPIx),(x))
#define port_SPIx_receive_data()		    SPI_I2S_ReceiveData(SPIx)
#define port_SPIx_disable()			      	SPI_Cmd(SPIx,DISABLE)
#define port_SPIx_enable()              SPI_Cmd(SPIx,ENABLE)
#define port_SPIx_set_chip_select()		  GPIO_SetBits(SPIx_CS_GPIO,SPIx_CS)
#define port_SPIx_clear_chip_select() 	GPIO_ResetBits(SPIx_CS_GPIO,SPIx_CS)
/*******************************************************************************/

void SPI_ChangeRate(uint16_t scalingfactor);
void SPI_ConfigFastRate(uint16_t scalingfactor);

//Set SPI rate to less than 3 MHz to properly perform DW1000 initialisation.
void spi_set_rate_low (void);
//Set SPI rate as close to 20 MHz as possible for optimum performances.
void spi_set_rate_high (void);
/*******************************************************************************/

#define DW1000_RSTn				        	GPIO_Pin_1
#define DW1000_RSTn_GPIO		      	GPIOA
#define RCC_DW1000_RSTn_GPIO        RCC_AHB1Periph_GPIOA

#define DECARSTIRQ                  GPIO_Pin_1
#define DECARSTIRQ_GPIO             GPIOA
#define DECARSTIRQ_EXTI             EXTI_Line1
#define DECARSTIRQ_EXTI_PORT        EXTI_PortSourceGPIOA
#define DECARSTIRQ_EXTI_PIN         GPIO_PinSource1
#define DECARSTIRQ_EXTI_IRQn        EXTI1_IRQn

//#define DECARSTIRQ_IRQn_PreemptionPriority     7
//#define DECARSTIRQ_IRQn_SubPriority            0

void reset_DW1000(void);
void setup_DW1000RSTnIRQ(int enable);//设置复位引脚工作模式
/*********************************************************************/

#define DECAIRQ                     GPIO_Pin_2       //DECAIRQ = DWM_IRQ
#define DECAIRQ_GPIO                GPIOA
#define RCC_DECAIRQ_GPIO            RCC_AHB1Periph_GPIOA
#define DECAIRQ_EXTI                EXTI_Line2
#define DECAIRQ_EXTI_PORT           EXTI_PortSourceGPIOA
#define DECAIRQ_EXTI_PIN            GPIO_PinSource2
#define DECAIRQ_EXTI_IRQn           EXTI2_IRQn
#define DECAIRQ_EXTI_USEIRQ         ENABLE
#define DECAIRQ_EXTI_NOIRQ          DISABLE

//#define DECAIRQ_IRQn_PreemptionPriority       7
//#define DECAIRQ_IRQn_SubPriority             0

ITStatus EXTI_GetITEnStatus(uint32_t x);

#define port_GetEXT_IRQStatus()             EXTI_GetITEnStatus(DECAIRQ_EXTI_IRQn)
#define port_DisableEXT_IRQ()               NVIC_DisableIRQ(DECAIRQ_EXTI_IRQn)
#define port_EnableEXT_IRQ()                NVIC_EnableIRQ(DECAIRQ_EXTI_IRQn)
#define port_CheckEXT_IRQ()                 GPIO_ReadInputDataBit(DECAIRQ_GPIO, DECAIRQ)

int DECAIRQ_Configuration(void);
int NVIC_DisableDECAIRQ(void);

int is_IRQ_enabled(void);

/* DW1000 IRQ (EXTI9_5_IRQ) handler type. */
typedef void (*port_deca_isr_t)(void);

/* DW1000 IRQ handler declaration. */
extern port_deca_isr_t   port_deca_isr;

//This function is used to install the handling function for DW1000 IRQ.
// NOTE:
//   - As EXTI9_5_IRQHandler does not check that port_deca_isr is not null, the user application must ensure that a
//     proper handler is set by calling this function before any DW1000 IRQ occurs!
//   - This function makes sure the DW1000 IRQ line is deactivated while the handler is installed.
//deca_isr function pointer to DW1000 interrupt handler to install

void port_set_deca_isr(port_deca_isr_t deca_isr);

/*******************************************************************************/

void peripherals_init (void);
/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PORT_H_ */

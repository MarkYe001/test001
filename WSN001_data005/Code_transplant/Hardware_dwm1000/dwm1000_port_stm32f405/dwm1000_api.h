
#ifndef _DWM1000_API_H //#include "dwm1000_api.h"
#define _DWM1000_API_H
/*****************************************************************************************************/

#include "stm32f4xx.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "sleep.h"
#include "deca_port_stm32f405.h"

/*****************************************************************************************************/
#define FRAME_LEN_MAX 127

typedef struct
{
  union addA
  {
	 uint8 tx_msg[16];
	 uint16_t time;
  }TXA;

	uint16 frame_len;
	
  union addB
  {
	 uint8 rx_buffer[FRAME_LEN_MAX];
	 uint16_t time;
  }RXB;
	
}UWB_DATA_TypeDef;

#define 	LED_RXOK_ON()   	dwt_setgpiovalue(GxM0,1)
#define 	LED_RXOK_OFF()  	dwt_setgpiovalue(GxM0,0)


void dwm1000_init(void);

//UWB发送函数,发送位于GATE_UWB.TXA.tx_msg中的数据,长度由sizeof指定
void UWB_Send_Enable(void);

void UWB_Receive_Enable(void);
/*****************************************************************************************************/
#endif /*_DWM1000_API_H*/

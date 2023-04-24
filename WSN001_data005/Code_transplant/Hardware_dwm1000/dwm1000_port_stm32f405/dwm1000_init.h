
#ifndef _DWM1000_INIT_H //#include "dwm1000_init.h"
#define _DWM1000_INIT_H
/***************************************************************************/

#include "stm32f4xx.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "sleep.h"
#include "deca_port_stm32f405.h"

/***************************************************************************/


#define 	LED_RXOK_ON()   	dwt_setgpiovalue(GxM0,1)
#define 	LED_RXOK_OFF()  	dwt_setgpiovalue(GxM0,0)


void dwm1000_init(void);

/***************************************************************************/
#endif /*_DWM1000_INIT_H*/


#ifndef _WSN_HOSTCMD_H //#include "wsn_hostcmd.h"
#define _WSN_HOSTCMD_H
/***************************************************************************/

#include "stm32f4xx.h"

/***************************************************************************/

void HostCmd_Process(void);

void UART_CoordSelfNetform( uint8_t *data, uint16_t dataLen );

void UART_TransCommand2AcqNode( uint8_t *data, uint16_t dataLen );

void UART_TransBackoffTime2Router( uint8_t *data, uint16_t dataLen );
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/***************************************************************************/
#endif /*_WSN_H*/

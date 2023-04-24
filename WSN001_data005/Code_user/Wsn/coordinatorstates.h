
#ifndef _COORDINATORSTATES_H //#include "coordinatorstates.h"
#define _COORDINATORSTATES_H
/*********************************************************************/

#include "stm32f4xx.h"

/***************************************************************************/
typedef enum
{
  COORD_START_NET_STATES_WAIT_NETFORM_COMMAND = 0x01,  //网关初始化,等待组网命令
  COORD_START_NET_STATES_WAIT_START_CNF       = 0x02,
  COORD_START_NET_STATES_OK                   = 0x03
}COORD_STATES;

typedef struct
{
  COORD_STATES startNetStates;   //coordinator网络初始化状态
  
  uint8_t      isTxDataWithBeac;
  
  uint8_t      beaconOrder;      //信标周期
  uint8_t      superFrameOrder;  //超帧周期
  
  uint8_t      pendingDataNum;
}CoordStatesParaSturct;

extern CoordStatesParaSturct CoordStatesPara;

//---------------函数声明----------------
void coordStates_StatesPareInit(void);

/*********************************************************************/
#endif










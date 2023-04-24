
#ifndef _COORDINATORSTATES_H //#include "coordinatorstates.h"
#define _COORDINATORSTATES_H
/*********************************************************************/

#include "stm32f4xx.h"

/***************************************************************************/
typedef enum
{
  COORD_START_NET_STATES_WAIT_NETFORM_COMMAND = 0x01,  //���س�ʼ��,�ȴ���������
  COORD_START_NET_STATES_WAIT_START_CNF       = 0x02,
  COORD_START_NET_STATES_OK                   = 0x03
}COORD_STATES;

typedef struct
{
  COORD_STATES startNetStates;   //coordinator�����ʼ��״̬
  
  uint8_t      isTxDataWithBeac;
  
  uint8_t      beaconOrder;      //�ű�����
  uint8_t      superFrameOrder;  //��֡����
  
  uint8_t      pendingDataNum;
}CoordStatesParaSturct;

extern CoordStatesParaSturct CoordStatesPara;

//---------------��������----------------
void coordStates_StatesPareInit(void);

/*********************************************************************/
#endif










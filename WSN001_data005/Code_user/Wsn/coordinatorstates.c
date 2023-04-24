/*********************************************************************/

#include "coordinatorstates.h"

/*********************************************************************/

CoordStatesParaSturct CoordStatesPara;

void coordStates_StatesPareInit(void)
{
  CoordStatesPara.startNetStates   = COORD_START_NET_STATES_WAIT_NETFORM_COMMAND;
  CoordStatesPara.isTxDataWithBeac = 0;
  CoordStatesPara.pendingDataNum   = 0;
}

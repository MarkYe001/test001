/***************************************************************************/

#include "wsn_hostcmd.h"
#include "usart_coordinator.h"
#include "usart_protocol.h"
#include "coordinatorstates.h"
#include "beacon_protocol.h"

/***************************************************************************/



/***************************************************************************/

void HostCmd_Process(void){
//	if(UartRxDataPara.dataaddr == DATA_RX_DATABUFFER)//Buffer��������
//	{
//		
//	}
	//��ַ������ǿ��ת��
	UART_DataStruct *pUartData = (UART_DataStruct *)(UartRxDataPara.data);
	//printf("type is %d\r\n",(pUartData->header.dataType));
  switch( pUartData->header.dataType )
  {
//-------------------------------����������---------------------------------
  case UART_DATA_TYPE_SELF_NETWORK_FORMATION:
	  printf("UART_DATA_TYPE_SELF_NETWORK_FORMATION\r\n");
    
    if( CoordStatesPara.startNetStates == COORD_START_NET_STATES_WAIT_NETFORM_COMMAND )  //�������״̬Ϊ�ȴ������齨����
    {
      UART_CoordSelfNetform( (uint8_t *)&pUartData->data, sizeof( pUartData->data.selfNetForm ) );
      printf("self network is ok\r\n");
		}
    break;
    
//-------------------------------���˹�ϵ-----------------------------------
  case UART_DATA_TYPE_TOPOLOGY_RELATIONSHIP:
		printf("UART_DATA_TYPE_TOPOLOGY_RELATIONSHIP\r\n");
    
    break;
    
//-------------------------------�����-------------------------------------
  case UART_DATA_TYPE_COMMAND:
		printf("UART_DATA_TYPE_COMMAND\r\n");
    
//    if( (CoordStatesPara.startNetStates == COORD_START_NET_STATES_OK) && (CoordStatesPara.isTxDataWithBeac == 0) )
//    {
//      //UART_TransCommand2AcqNode( (uint8_t *)&pUartData->data, sizeof( pUartData->data.dataCommand ) );
//    }
    break;
    
//-------------------------------·�����˱�ʱ��-----------------------------
  case UART_DATA_TYPE_ROUTER_BACKOFF_TIME:
		printf("UART_DATA_TYPE_ROUTER_BACKOFF_TIME\r\n");
    
    //UART_TransBackoffTime2Router( (uint8_t *)&pUartData->data, sizeof( pUartData->data.routerBckoffTime ) );
    break;
  }
	//printf("type is %d\r\n",(pUartData->header.dataType));
}

/***************************************************************************/
//���ؽ��յ���������������

void UART_CoordSelfNetform( uint8_t *data, uint16_t dataLen )
{
  
  UART_DataUnion *pUartData = (UART_DataUnion *)data;
  
  SelfNetBeaclodBaseInforStruct  baseBeaclod;  //·������Ҫ�㲥���ű긺��
  
  //����coordinator��ַ,��ʼ��UWBģ��
  
  
  //���� �ű긺��  ��������
  baseBeaclod.beaconType = BEACON_DATA_TYPE_SELF_NETFORM;
  baseBeaclod.depth = 0;  //���ؽڵ����Ϊ0
  baseBeaclod.maxDepth = pUartData->selfNetForm.maxDepth;//����������
  baseBeaclod.beaconTc = pUartData->selfNetForm.beaconTc;//������ʱʱ��ת��Ϊ��˱�ʱ�䳣��
  
  //�����ű긺�ػ�������(ʹ��ȫ�ֱ������������ĸ�����)
	//beaclodSet_SelfNetCoordSetBaseBeaclod( &baseBeaclod );
  
  //�� �ű긺������ ���͵� MAC��
  //beaclodSet_SelfNetSetBeaclod2Mac();
  
  //������������
  //coordInit_SetupNetwork( pUartData->selfNetForm.beaconOrder, pUartData->selfNetForm.superFrameOrder );
  
  CoordStatesPara.beaconOrder = pUartData->selfNetForm.beaconOrder; //�ű�����
  CoordStatesPara.superFrameOrder = pUartData->selfNetForm.superFrameOrder; //��֡����
  
  CoordStatesPara.startNetStates = COORD_START_NET_STATES_WAIT_START_CNF;  //�ȴ���������ظ�
}
/***************************************************************************/
////���յ������������ת�������

//void UART_TransCommand2AcqNode( uint8_t *data, uint16_t dataLen )
//{
//  UART_DataUnion *pUartData = (UART_DataUnion *)data;  //�������ݽṹ��ָ��
//  BeaclodStruct *pBeaclod = (BeaclodStruct *)payLoad;  //�ű�����ṹ��ָ��
//  DataPktTxInforSturct pktInfor;  //���ݰ�������Ϣ
//  uint8 destNodeID, commandLen, beaconPayloadLen;
//  
//  //-------------�������������ȷ����Ҫ���͵�Ŀ�ĵ�ַ---------------
//  if( pUartData->dataCommand.command.acqCommand.commandType == COMMAND_TYPE_DATA_ACQ )  //Ϊ���ݲɼ�����
//  {
//    destNodeID = pUartData->dataCommand.command.acqCommand.nodeID;  //Ŀ�Ľڵ�ID
//    commandLen = sizeof( pUartData->dataCommand.command.acqCommand );  //�����
//  }
//  else if( pUartData->dataCommand.command.transCommand.commandType == COMMAND_TYPE_DATA_TRANS )  //Ϊ���ݴ�������
//  {
//    destNodeID = pUartData->dataCommand.command.transCommand.nodeID;
//    commandLen = sizeof( pUartData->dataCommand.command.transCommand );
//  }
//    
//  //-----------------------ת�������-------------------------------
//  if( destNodeID == UART_DATA_DEST_ALL )  //ͨ���ű귢��
//  {
//    pBeaclod->beaconType = BEACON_DATA_TYPE_COMMAND;  //�ű��������� ����� 
//    osal_memcpy( &pBeaclod->data, &(pUartData->dataCommand.command), commandLen );  //����
//    
//    beaconPayloadLen = commandLen+1;
//    
//      //��������
//    beaclodSet_TxDataWithBeaclod( (uint8*)pBeaclod, beaconPayloadLen, 1 );  //����һ��
//    CoordStatesPara.isTxDataWithBeac = 1;
//  }
//  
//  else  //ͨ�����ݰ����͵�ȷ���ڵ�
//  {
//    pktInfor.pktHeader.dataType = DATA_PKT_TYPE_COMMAND;  //���ݰ����� �����
//    
//    pktInfor.pktHeader.ultiSourceAddr = 0x00;  //Դ��ַΪ����
//    pktInfor.pktHeader.ultiDestAddr = destNodeID;  //Ŀ�ĵ�ַ
//    pktInfor.nextDestAddr = pUartData->dataCommand.nextHopNodeID;  //��Ҫ���͵�Ŀ�Ľڵ�ID
//    
//    pktInfor.dataLen = commandLen;  //���������˱�ʱ�估�˱�ʱ�������ֽ�
//    osal_memcpy( pktInfor.data, &(pUartData->dataCommand.command), commandLen );  //����
//    
//    pktInfor.dataDirection = DATA_PKT_DIRECTIOIN_DOWN;
//    
//    dataPkt_TxIsolatedPkt( &pktInfor );
//    
//    LED_Flicker(1);
//  }
//}
///***************************************************************************/
////��·���������˱�ʱ��

//void UART_TransBackoffTime2Router( uint8_t *data, uint16_t dataLen )
//{
//  UART_DataUnion *pUartData = (UART_DataUnion *)data;
//  DataPktTxInforSturct pktInfor;  //���ݰ�������Ϣ
//  
//  pktInfor.pktHeader.dataType = DATA_PKT_TYPE_ROUTER_BACKOFF_TIME;
//  
//  pktInfor.pktHeader.ultiSourceAddr = 0x00;  //Դ��ַΪ����
//  pktInfor.pktHeader.ultiDestAddr = pUartData->routerBckoffTime.desAddr;  //Ŀ�ĵ�ַ
//  pktInfor.nextDestAddr = pUartData->routerBckoffTime.nextHopAddr;
//  
//  pktInfor.dataLen = 2;  //���������˱�ʱ�估�˱�ʱ�������ֽ�
//  pktInfor.data[0] = pUartData->routerBckoffTime.baseBackoffTime;
//  pktInfor.data[1] = pUartData->routerBckoffTime.backoffTime;
//  
//  pktInfor.dataDirection = DATA_PKT_DIRECTIOIN_DOWN;
//  
//  dataPkt_TxIsolatedPkt( &pktInfor );
//}
/***************************************************************************/

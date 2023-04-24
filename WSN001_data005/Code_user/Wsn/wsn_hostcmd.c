/***************************************************************************/

#include "wsn_hostcmd.h"
#include "usart_coordinator.h"
#include "usart_protocol.h"
#include "coordinatorstates.h"
#include "beacon_protocol.h"

/***************************************************************************/



/***************************************************************************/

void HostCmd_Process(void){
//	if(UartRxDataPara.dataaddr == DATA_RX_DATABUFFER)//Buffer中有命令
//	{
//		
//	}
	//地址拷贝与强制转化
	UART_DataStruct *pUartData = (UART_DataStruct *)(UartRxDataPara.data);
	//printf("type is %d\r\n",(pUartData->header.dataType));
  switch( pUartData->header.dataType )
  {
//-------------------------------自组网命令---------------------------------
  case UART_DATA_TYPE_SELF_NETWORK_FORMATION:
	  printf("UART_DATA_TYPE_SELF_NETWORK_FORMATION\r\n");
    
    if( CoordStatesPara.startNetStates == COORD_START_NET_STATES_WAIT_NETFORM_COMMAND )  //如果网关状态为等待网络组建命令
    {
      UART_CoordSelfNetform( (uint8_t *)&pUartData->data, sizeof( pUartData->data.selfNetForm ) );
      printf("self network is ok\r\n");
		}
    break;
    
//-------------------------------拓扑关系-----------------------------------
  case UART_DATA_TYPE_TOPOLOGY_RELATIONSHIP:
		printf("UART_DATA_TYPE_TOPOLOGY_RELATIONSHIP\r\n");
    
    break;
    
//-------------------------------命令包-------------------------------------
  case UART_DATA_TYPE_COMMAND:
		printf("UART_DATA_TYPE_COMMAND\r\n");
    
//    if( (CoordStatesPara.startNetStates == COORD_START_NET_STATES_OK) && (CoordStatesPara.isTxDataWithBeac == 0) )
//    {
//      //UART_TransCommand2AcqNode( (uint8_t *)&pUartData->data, sizeof( pUartData->data.dataCommand ) );
//    }
    break;
    
//-------------------------------路由器退避时间-----------------------------
  case UART_DATA_TYPE_ROUTER_BACKOFF_TIME:
		printf("UART_DATA_TYPE_ROUTER_BACKOFF_TIME\r\n");
    
    //UART_TransBackoffTime2Router( (uint8_t *)&pUartData->data, sizeof( pUartData->data.routerBckoffTime ) );
    break;
  }
	//printf("type is %d\r\n",(pUartData->header.dataType));
}

/***************************************************************************/
//网关接收到串口自组网命令

void UART_CoordSelfNetform( uint8_t *data, uint16_t dataLen )
{
  
  UART_DataUnion *pUartData = (UART_DataUnion *)data;
  
  SelfNetBeaclodBaseInforStruct  baseBeaclod;  //路由器需要广播的信标负载
  
  //设置coordinator地址,初始化UWB模块
  
  
  //设置 信标负载  基本参数
  baseBeaclod.beaconType = BEACON_DATA_TYPE_SELF_NETFORM;
  baseBeaclod.depth = 0;  //网关节点深度为0
  baseBeaclod.maxDepth = pUartData->selfNetForm.maxDepth;//允许最大深度
  baseBeaclod.beaconTc = pUartData->selfNetForm.beaconTc;//启动延时时间转化为最长退避时间常数
  
  //保存信标负载基本参数(使用全局变量保存上面四个参数)
	//beaclodSet_SelfNetCoordSetBaseBeaclod( &baseBeaclod );
  
  //将 信标负载内容 发送到 MAC层
  //beaclodSet_SelfNetSetBeaclod2Mac();
  
  //网关启动网络
  //coordInit_SetupNetwork( pUartData->selfNetForm.beaconOrder, pUartData->selfNetForm.superFrameOrder );
  
  CoordStatesPara.beaconOrder = pUartData->selfNetForm.beaconOrder; //信标周期
  CoordStatesPara.superFrameOrder = pUartData->selfNetForm.superFrameOrder; //超帧周期
  
  CoordStatesPara.startNetStates = COORD_START_NET_STATES_WAIT_START_CNF;  //等待启动网络回复
}
/***************************************************************************/
////接收到串口命令包后转发命令包

//void UART_TransCommand2AcqNode( uint8_t *data, uint16_t dataLen )
//{
//  UART_DataUnion *pUartData = (UART_DataUnion *)data;  //串口数据结构体指针
//  BeaclodStruct *pBeaclod = (BeaclodStruct *)payLoad;  //信标命令结构体指针
//  DataPktTxInforSturct pktInfor;  //数据包发送信息
//  uint8 destNodeID, commandLen, beaconPayloadLen;
//  
//  //-------------根据命令包类型确定需要发送的目的地址---------------
//  if( pUartData->dataCommand.command.acqCommand.commandType == COMMAND_TYPE_DATA_ACQ )  //为数据采集命令
//  {
//    destNodeID = pUartData->dataCommand.command.acqCommand.nodeID;  //目的节点ID
//    commandLen = sizeof( pUartData->dataCommand.command.acqCommand );  //命令长度
//  }
//  else if( pUartData->dataCommand.command.transCommand.commandType == COMMAND_TYPE_DATA_TRANS )  //为数据传输命令
//  {
//    destNodeID = pUartData->dataCommand.command.transCommand.nodeID;
//    commandLen = sizeof( pUartData->dataCommand.command.transCommand );
//  }
//    
//  //-----------------------转发命令包-------------------------------
//  if( destNodeID == UART_DATA_DEST_ALL )  //通过信标发送
//  {
//    pBeaclod->beaconType = BEACON_DATA_TYPE_COMMAND;  //信标数据类型 命令包 
//    osal_memcpy( &pBeaclod->data, &(pUartData->dataCommand.command), commandLen );  //命令
//    
//    beaconPayloadLen = commandLen+1;
//    
//      //启动网络
//    beaclodSet_TxDataWithBeaclod( (uint8*)pBeaclod, beaconPayloadLen, 1 );  //发送一次
//    CoordStatesPara.isTxDataWithBeac = 1;
//  }
//  
//  else  //通过数据包发送到确定节点
//  {
//    pktInfor.pktHeader.dataType = DATA_PKT_TYPE_COMMAND;  //数据包类型 命令包
//    
//    pktInfor.pktHeader.ultiSourceAddr = 0x00;  //源地址为网关
//    pktInfor.pktHeader.ultiDestAddr = destNodeID;  //目的地址
//    pktInfor.nextDestAddr = pUartData->dataCommand.nextHopNodeID;  //将要发送的目的节点ID
//    
//    pktInfor.dataLen = commandLen;  //包括基本退避时间及退避时间两个字节
//    osal_memcpy( pktInfor.data, &(pUartData->dataCommand.command), commandLen );  //命令
//    
//    pktInfor.dataDirection = DATA_PKT_DIRECTIOIN_DOWN;
//    
//    dataPkt_TxIsolatedPkt( &pktInfor );
//    
//    LED_Flicker(1);
//  }
//}
///***************************************************************************/
////向路由器发送退避时间

//void UART_TransBackoffTime2Router( uint8_t *data, uint16_t dataLen )
//{
//  UART_DataUnion *pUartData = (UART_DataUnion *)data;
//  DataPktTxInforSturct pktInfor;  //数据包发送信息
//  
//  pktInfor.pktHeader.dataType = DATA_PKT_TYPE_ROUTER_BACKOFF_TIME;
//  
//  pktInfor.pktHeader.ultiSourceAddr = 0x00;  //源地址为网关
//  pktInfor.pktHeader.ultiDestAddr = pUartData->routerBckoffTime.desAddr;  //目的地址
//  pktInfor.nextDestAddr = pUartData->routerBckoffTime.nextHopAddr;
//  
//  pktInfor.dataLen = 2;  //包括基本退避时间及退避时间两个字节
//  pktInfor.data[0] = pUartData->routerBckoffTime.baseBackoffTime;
//  pktInfor.data[1] = pUartData->routerBckoffTime.backoffTime;
//  
//  pktInfor.dataDirection = DATA_PKT_DIRECTIOIN_DOWN;
//  
//  dataPkt_TxIsolatedPkt( &pktInfor );
//}
/***************************************************************************/

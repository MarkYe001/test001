#ifndef _USART_PROTOCOL_H //#include "usart_protocol.h"
#define _USART_PROTOCOL_H
/***************************************************************************/

#include "stm32f4xx.h"
#include "dataacq_protocol.h"

/***************************************************************************/
//0x01:数据包

typedef enum
{
  UART_RDP_TYPE_NORMAL      = 0x01, //正常的原始数据包
  UART_RDP_TYPE_LAST        = 0x02, //最后一个原始数据包
  UART_RDP_TYPE_ERROR_INFOR = 0x03  //获取原始数据发生错误(此时原始数据为错误类型)
}ptk_trait;

typedef struct
{
  ptk_trait rdpType;               //原始数据包类型  rdp:raw data pkt
  uint16_t  pktSerial;             //包序号
  uint8_t   rawData[ RAWDATALEN ]; //原始数据
}UART_DeviceRDP_ts;             //采集节点无线模块接收到的采集模块原始数据包  
                                // RDP: Raw Data Pkt ts: typedef struct

//网关向上位机传输原始数据结构体
typedef struct
{
  uint8_t   nodeID;                 //节点ID
  uint16_t  pktSerial;              //包序号
  uint8_t   rawData[ RAWDATALEN ];  //原始数据
}UART_CoordRDP_ts;               //网关向上位机传输原始数据结构体
                                 // RDP: Raw Data Pkt   ts:typedef struct

//串口原始数据包共同体
typedef union
{
  UART_CoordRDP_ts   coordRDP;   //网关的原始数据包结构体
  UART_DeviceRDP_ts  deviceRDP;  //采集节点无线模块的原始数据包结构体
}UART_RDP_tu;                    //串口原始数据包共同体
                                 // RDP: Raw Data Pkt  tu:typedef union 
/***************************************************************************/
//0x02:命令包

typedef struct
{
  CommandUnion  command;        //数据采集命令
  uint8_t       nextHopNodeID;  //下一跳节点ID
}UART_CommandStruct;

/***************************************************************************/
//0x04:关系地址包

typedef struct
{
  uint8_t  nodeType;  //节点类型，路由节点或采集节点 
  uint8_t  fatherID;  //父节点ID
  uint8_t  childID;   //子节点ID
}UART_AddrRelateStruct;

/***************************************************************************/
//0x05:拓扑关系包结构体

typedef struct
{
  uint8_t data[52];
}UART_TopologyRelationshipStruct;

/***************************************************************************/
//0x06:自组网命令结构体

typedef struct
{
  uint8_t beaconOrder;      //信标周期
  uint8_t superFrameOrder;  //超帧周期
  uint8_t maxDepth;         //允许最大深度
  uint8_t beaconTc;         //启动延时时间
}UART_SelfNetFormStruct;

/***************************************************************************/
//0x07:路由节点退避时间结构体

typedef struct
{
  uint8_t desAddr;           //路由器地址
  uint8_t nextHopAddr;       //下一跳地址
  uint8_t baseBackoffTime;   //基本退避时间间隔
  uint8_t backoffTime;       //退避时间
}UART_RoutBckoffTimStruct;

/***************************************************************************/
//0x08:请求采集模块上传数据 命令 结构体

typedef struct
{
  uint8_t storage;      //存储位置
  uint8_t nodeID;       //本节点ID
  uint8_t fileID;       //文件ID
  uint8_t reserved0;    //为字节对其而保留
  uint16_t pktSerial;   //包序号
  uint16_t dataLen;     //数据长度
  uint32_t startLoca;   //起始位置
}UART_ReqTxRawDataStruct;

/***************************************************************************/
//串口数据头

typedef struct
{
	uint8_t dataType;  //数据类型
}UART_DataHeader;

//串口接收数据共同体
typedef union
{ 
  UART_RDP_tu                      rawData;              //(0x01)数据包
  UART_CommandStruct               dataCommand;          //(0x02)命令包
                                                         //(0x03)回复包
  UART_AddrRelateStruct            addrRelate;           //(0x04)关系地址包
  UART_TopologyRelationshipStruct  topoRelationship;     //(0x05)拓扑关系包
  UART_SelfNetFormStruct           selfNetForm;          //(0x06)自组网命令包
  UART_RoutBckoffTimStruct         routerBckoffTime;     //(0x07)路由器退避时间
  UART_ReqTxRawDataStruct          reqTxRawDataCommand;  //(0x08)请求采集模块上传原始数据命令
                                                         //(0x09)链路质量检测包
	                                                       //(0x0A)重组网命令包
	                                                       //(0x0B)数据融合包
	                                                       //(0x0C)请求采集模块上传特征数据
}UART_DataUnion;

/***************************************************************************/
//串口数据结构体，为串口数据提供统一的数据结构索引

typedef struct
{
  UART_DataHeader          header;  //数据类型
  UART_DataUnion           data;      //数据
}UART_DataStruct;

/***************************************************************************/
//串口数据类型

typedef enum
{
  UART_DATA_TYPE_RAW_DATA               = 0x01,   //采集模块上传的数据
  UART_DATA_TYPE_COMMAND                = 0x02,   //命令包
  UART_DATA_TYPE_COMMAND_REPLY          = 0x03,   //采集模块的命令回复
  UART_DATA_TYPE_ADDR_RELATIONSHIP      = 0x04,   //网关向上位机上传关系地址包
  UART_DATA_TYPE_TOPOLOGY_RELATIONSHIP  = 0x05,   //上位机向网关下发的拓扑关系包
  UART_DATA_TYPE_SELF_NETWORK_FORMATION = 0x06,   //上位机下发的自组网命令
  UART_DATA_TYPE_ROUTER_BACKOFF_TIME    = 0x07,   //上位机下发的路由器退避时间
  UART_DATA_TYPE_REQ_ACQ_TX_RAW_DATA    = 0x08    //请求 采集模块上传原始数据命令
}UartDataTypeEnum;

/***************************************************************************/
/***************************************************************************/
#endif /*_USART_PROTOCOL_H*/


#ifndef _BEACON_PROTOCOL_H //#include "beacon_protocol.h"
#define _BEACON_PROTOCOL_H
/***************************************************************************/

#include "stm32f4xx.h"
#include "usart_protocol.h"

/***************************************************************************/

//-------------------枚举类型声明-------------------
//x_x 信标负载类型
/*协调器广播的信标负载首地址应标明该信标负载类型
  信标负载类型如下
*/
typedef enum 
{
  BEACON_DATA_TYPE_SELF_NETFORM          = 0x01,    //自组网
  BEACON_DATA_TYPE_TOPOLOGY_RELATIONSHIP = 0x02,    //拓扑关系
  BEACON_DATA_TYPE_COMMAND               = 0x03     //命令
}BEACON_TAPE;

//x_x 协调器指定节点允许发送的数据类型
/*子节点不能直接向父节点发送数据包，只能在得到父节点允许的情况下才能发送相应类型的数据包
  数据包类型如下
*/
enum 
{
  COORD_ALLOW_NODE_TX_PKT_TYPE_NONE                 = 0x00,
  COORD_ALLOW_NODE_TX_PKT_TYPE_ANY                  = 0x01,  //任意数据包
  COORD_ALLOW_NODE_TX_PKT_TYPE_ADDR_RELATE          = 0x02,  //关系地址包
  COORD_ALLOW_NODE_TX_PKT_TYPE_ADDR_RELATE_RECEIVED = 0x03
};

/***************************************************************************/
//------------------自组网 信标负载-------------------------

//自组网时信标负载基本信息
/*在通常情况下（空闲状态），协调器会广播自组网信标负载
  其信标负载内容如下
*/
typedef struct
{
  BEACON_TAPE  beaconType;  //信标负载类型  见 x_x 信标负载类型
  uint8_t depth;            //本节点深度
  uint8_t maxDepth;         //网络允许最大深度
  uint8_t beaconTc;         //最长退避时间常数
  
  uint8_t backoffDescDataLoca;  //路由节点退避时间 描述符 位置（在信标负载中位置）
  uint8_t backoffDescLen;       //路由节点退避时间 描述符 数据长度
  
  uint8_t transManageDescLoca;  //协调器允许子节点发送数据包 描述符 位置（在信标负载中位置）
  uint8_t transManageDescLen;   //协调器允许子节点发送数据包 描述符 数据长度（BYTE）
}SelfNetBeaclodBaseInforStruct;
 
////路由器退避时间及晶振漂移结构体
///*路由节点退避时间 描述符
//  包括路由器退避理想时间 及 晶振漂移
//  采集节点通过这两个信息可计算出路由节点实际退避时间长度
//*/
//typedef struct
//{
//  uint16_t backoffTime;  //路由器理想退避时间
//  uint16_t COdrift;      //路由器晶振漂移
//}BackoffDescStruct;

////协调器指定节点发送指定类型数据包结构体
///*包括指定节点ID 及 指定数据包类型
//*/
//typedef struct
//{
//  uint8_t nodeID;   //指定节点ID
//  uint8_t pktType;  //指定数据包类型 见 x_x 协调器指定节点允许发送的数据类型
//}TransManageDescStruct;

//------------------------信标负载联合体 自组网除外---------------
typedef union
{
  CommandUnion command; 
}beaclodDataUnion;

//------------------------信标负载联合体 自组网除外---------------
typedef struct
{
  uint8_t              beaconType;  //信标类型
  beaclodDataUnion   data;        //信标数据
}BeaclodStruct;

////------------------------信标描述符------------------------------
///*用于保存符合要求的信标基本信息
//  包括 信标MAC层信息 和 信标用户信息
//*/
//#define BEACON_DESCR_NUM 5  //存储信标数量

////信标MAC层信息结构体
//typedef struct
//{
//  uint8_t permitJoin;      //是否允许加入
//  uint8_t logicalChannel;  //信道
//  uint8_t beaconOrder;     //信标周期
//  uint8_t superFrameOrder; //超帧周期
//  uint8_t linkQuality;     //信道质量
//  
//  uint16_t coordPanId;     //PANID
//  uint16_t coordShortAddr; //协调器ID
//  uint32_t timestamp;      //接收到该信标时间戳
//}BeaconMacInforStruct;

////自组网时信标用户信息
///*自组网模式下，节点可以自由的加入网络
//*/
//typedef struct
//{
//  uint8_t beaconType;  //信标负载类型  见 x_x 信标负载类型
//  uint8_t depth;       //本节点深度
//  uint8_t maxDepth;    //网络允许最大深度
//  uint8_t beaconTc;    //最长退避时间常数
//}SelfNetformBeacUserInforStruct;

////信标用户信息共同体
//typedef union
//{
//  SelfNetformBeacUserInforStruct selfNetform;  //自组网模式下 信标用户信息
//}BeaconUserInforUnion;

////信标信息描述符
//typedef struct
//{                 
//  BeaconMacInforStruct macInfor;
//  BeaconUserInforUnion userInfor;
//}BeaconDescrStruct;

////保存 信标信息 的结构体
///*该结构体用户保存 信标信息 和 相关控制信息
//*/
//typedef struct
//{
//  uint8_t isSaveBeacon;
//  
//  uint8_t beaconNum;
//  uint8_t currentBeacon;  //0xff表示还没有尝试加入的信标，<BEACON_DESCR_NUM 表示 已经尝试了第几个信标
//  
//  BeaconDescrStruct beaconDescr[BEACON_DESCR_NUM];
//}SavedBeaconDescrStruct;  //已经存储的信标信息描述结构体  

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#endif /*_BEACON_PROTOCOL_H*/


#ifndef _DATAACQ_PROTOCOL_H //#include "dataacq_protocol.h"
#define _DATAACQ_PROTOCOL_H
/***************************************************************************/

#include "stm32f4xx.h"

/***************************************************************************/
//上位机发送到采集节点命令

#define RAWDATALEN 96

//采集命令结构体
typedef struct
{
  uint8_t commandType;    //命令类型, 0x01为采集命令
  uint8_t dataAcqType;    //采样模式
  uint8_t nodeID;         //节点ID，为0表示发给所有节点，其他代表发给指定节点
  uint8_t fileID;         //文件ID
  uint8_t coupleMode;     //耦合方式
  float sampleRate;       //采样率
  uint32_t sampleLen;     //采样长度
  uint8_t ampFactor;      //放大倍数
  uint8_t trigrVoltage;   //触发电压
  uint8_t storLoca;       //存储位置
  uint8_t acqNodeType;    //采集节点类型
}DataAcqCommandStruct;

//传输命令结构体
typedef struct
{
  uint8_t commandType;    //命令类型, 0x02为数据传输命令
  uint8_t transType;      //传输模式
  uint8_t nodeID;         //节点ID，为0表示发给所有节点，其他代表发给指定节点
  uint8_t fileID;         //文件ID
  uint8_t storage;
  uint32_t startLoca;     //需要传输的数据起始位置
  uint32_t dataLen;       //需要传输的数据长度
}DataTransCommandStruct;

typedef union
{
  DataAcqCommandStruct acqCommand;
  DataTransCommandStruct transCommand;
}CommandUnion;

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#endif /*_DATAACQ_PROTOCOL_H*/


#ifndef _DATAACQ_PROTOCOL_H //#include "dataacq_protocol.h"
#define _DATAACQ_PROTOCOL_H
/***************************************************************************/

#include "stm32f4xx.h"

/***************************************************************************/
//��λ�����͵��ɼ��ڵ�����

#define RAWDATALEN 96

//�ɼ�����ṹ��
typedef struct
{
  uint8_t commandType;    //��������, 0x01Ϊ�ɼ�����
  uint8_t dataAcqType;    //����ģʽ
  uint8_t nodeID;         //�ڵ�ID��Ϊ0��ʾ�������нڵ㣬����������ָ���ڵ�
  uint8_t fileID;         //�ļ�ID
  uint8_t coupleMode;     //��Ϸ�ʽ
  float sampleRate;       //������
  uint32_t sampleLen;     //��������
  uint8_t ampFactor;      //�Ŵ���
  uint8_t trigrVoltage;   //������ѹ
  uint8_t storLoca;       //�洢λ��
  uint8_t acqNodeType;    //�ɼ��ڵ�����
}DataAcqCommandStruct;

//��������ṹ��
typedef struct
{
  uint8_t commandType;    //��������, 0x02Ϊ���ݴ�������
  uint8_t transType;      //����ģʽ
  uint8_t nodeID;         //�ڵ�ID��Ϊ0��ʾ�������нڵ㣬����������ָ���ڵ�
  uint8_t fileID;         //�ļ�ID
  uint8_t storage;
  uint32_t startLoca;     //��Ҫ�����������ʼλ��
  uint32_t dataLen;       //��Ҫ��������ݳ���
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

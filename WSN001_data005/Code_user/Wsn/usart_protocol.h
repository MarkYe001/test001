#ifndef _USART_PROTOCOL_H //#include "usart_protocol.h"
#define _USART_PROTOCOL_H
/***************************************************************************/

#include "stm32f4xx.h"
#include "dataacq_protocol.h"

/***************************************************************************/
//0x01:���ݰ�

typedef enum
{
  UART_RDP_TYPE_NORMAL      = 0x01, //������ԭʼ���ݰ�
  UART_RDP_TYPE_LAST        = 0x02, //���һ��ԭʼ���ݰ�
  UART_RDP_TYPE_ERROR_INFOR = 0x03  //��ȡԭʼ���ݷ�������(��ʱԭʼ����Ϊ��������)
}ptk_trait;

typedef struct
{
  ptk_trait rdpType;               //ԭʼ���ݰ�����  rdp:raw data pkt
  uint16_t  pktSerial;             //�����
  uint8_t   rawData[ RAWDATALEN ]; //ԭʼ����
}UART_DeviceRDP_ts;             //�ɼ��ڵ�����ģ����յ��Ĳɼ�ģ��ԭʼ���ݰ�  
                                // RDP: Raw Data Pkt ts: typedef struct

//��������λ������ԭʼ���ݽṹ��
typedef struct
{
  uint8_t   nodeID;                 //�ڵ�ID
  uint16_t  pktSerial;              //�����
  uint8_t   rawData[ RAWDATALEN ];  //ԭʼ����
}UART_CoordRDP_ts;               //��������λ������ԭʼ���ݽṹ��
                                 // RDP: Raw Data Pkt   ts:typedef struct

//����ԭʼ���ݰ���ͬ��
typedef union
{
  UART_CoordRDP_ts   coordRDP;   //���ص�ԭʼ���ݰ��ṹ��
  UART_DeviceRDP_ts  deviceRDP;  //�ɼ��ڵ�����ģ���ԭʼ���ݰ��ṹ��
}UART_RDP_tu;                    //����ԭʼ���ݰ���ͬ��
                                 // RDP: Raw Data Pkt  tu:typedef union 
/***************************************************************************/
//0x02:�����

typedef struct
{
  CommandUnion  command;        //���ݲɼ�����
  uint8_t       nextHopNodeID;  //��һ���ڵ�ID
}UART_CommandStruct;

/***************************************************************************/
//0x04:��ϵ��ַ��

typedef struct
{
  uint8_t  nodeType;  //�ڵ����ͣ�·�ɽڵ��ɼ��ڵ� 
  uint8_t  fatherID;  //���ڵ�ID
  uint8_t  childID;   //�ӽڵ�ID
}UART_AddrRelateStruct;

/***************************************************************************/
//0x05:���˹�ϵ���ṹ��

typedef struct
{
  uint8_t data[52];
}UART_TopologyRelationshipStruct;

/***************************************************************************/
//0x06:����������ṹ��

typedef struct
{
  uint8_t beaconOrder;      //�ű�����
  uint8_t superFrameOrder;  //��֡����
  uint8_t maxDepth;         //����������
  uint8_t beaconTc;         //������ʱʱ��
}UART_SelfNetFormStruct;

/***************************************************************************/
//0x07:·�ɽڵ��˱�ʱ��ṹ��

typedef struct
{
  uint8_t desAddr;           //·������ַ
  uint8_t nextHopAddr;       //��һ����ַ
  uint8_t baseBackoffTime;   //�����˱�ʱ����
  uint8_t backoffTime;       //�˱�ʱ��
}UART_RoutBckoffTimStruct;

/***************************************************************************/
//0x08:����ɼ�ģ���ϴ����� ���� �ṹ��

typedef struct
{
  uint8_t storage;      //�洢λ��
  uint8_t nodeID;       //���ڵ�ID
  uint8_t fileID;       //�ļ�ID
  uint8_t reserved0;    //Ϊ�ֽڶ��������
  uint16_t pktSerial;   //�����
  uint16_t dataLen;     //���ݳ���
  uint32_t startLoca;   //��ʼλ��
}UART_ReqTxRawDataStruct;

/***************************************************************************/
//��������ͷ

typedef struct
{
	uint8_t dataType;  //��������
}UART_DataHeader;

//���ڽ������ݹ�ͬ��
typedef union
{ 
  UART_RDP_tu                      rawData;              //(0x01)���ݰ�
  UART_CommandStruct               dataCommand;          //(0x02)�����
                                                         //(0x03)�ظ���
  UART_AddrRelateStruct            addrRelate;           //(0x04)��ϵ��ַ��
  UART_TopologyRelationshipStruct  topoRelationship;     //(0x05)���˹�ϵ��
  UART_SelfNetFormStruct           selfNetForm;          //(0x06)�����������
  UART_RoutBckoffTimStruct         routerBckoffTime;     //(0x07)·�����˱�ʱ��
  UART_ReqTxRawDataStruct          reqTxRawDataCommand;  //(0x08)����ɼ�ģ���ϴ�ԭʼ��������
                                                         //(0x09)��·��������
	                                                       //(0x0A)�����������
	                                                       //(0x0B)�����ںϰ�
	                                                       //(0x0C)����ɼ�ģ���ϴ���������
}UART_DataUnion;

/***************************************************************************/
//�������ݽṹ�壬Ϊ���������ṩͳһ�����ݽṹ����

typedef struct
{
  UART_DataHeader          header;  //��������
  UART_DataUnion           data;      //����
}UART_DataStruct;

/***************************************************************************/
//������������

typedef enum
{
  UART_DATA_TYPE_RAW_DATA               = 0x01,   //�ɼ�ģ���ϴ�������
  UART_DATA_TYPE_COMMAND                = 0x02,   //�����
  UART_DATA_TYPE_COMMAND_REPLY          = 0x03,   //�ɼ�ģ�������ظ�
  UART_DATA_TYPE_ADDR_RELATIONSHIP      = 0x04,   //��������λ���ϴ���ϵ��ַ��
  UART_DATA_TYPE_TOPOLOGY_RELATIONSHIP  = 0x05,   //��λ���������·������˹�ϵ��
  UART_DATA_TYPE_SELF_NETWORK_FORMATION = 0x06,   //��λ���·�������������
  UART_DATA_TYPE_ROUTER_BACKOFF_TIME    = 0x07,   //��λ���·���·�����˱�ʱ��
  UART_DATA_TYPE_REQ_ACQ_TX_RAW_DATA    = 0x08    //���� �ɼ�ģ���ϴ�ԭʼ��������
}UartDataTypeEnum;

/***************************************************************************/
/***************************************************************************/
#endif /*_USART_PROTOCOL_H*/

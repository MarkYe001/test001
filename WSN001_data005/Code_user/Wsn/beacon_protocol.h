
#ifndef _BEACON_PROTOCOL_H //#include "beacon_protocol.h"
#define _BEACON_PROTOCOL_H
/***************************************************************************/

#include "stm32f4xx.h"
#include "usart_protocol.h"

/***************************************************************************/

//-------------------ö����������-------------------
//x_x �ű긺������
/*Э�����㲥���ű긺���׵�ַӦ�������ű긺������
  �ű긺����������
*/
typedef enum 
{
  BEACON_DATA_TYPE_SELF_NETFORM          = 0x01,    //������
  BEACON_DATA_TYPE_TOPOLOGY_RELATIONSHIP = 0x02,    //���˹�ϵ
  BEACON_DATA_TYPE_COMMAND               = 0x03     //����
}BEACON_TAPE;

//x_x Э����ָ���ڵ������͵���������
/*�ӽڵ㲻��ֱ���򸸽ڵ㷢�����ݰ���ֻ���ڵõ����ڵ����������²��ܷ�����Ӧ���͵����ݰ�
  ���ݰ���������
*/
enum 
{
  COORD_ALLOW_NODE_TX_PKT_TYPE_NONE                 = 0x00,
  COORD_ALLOW_NODE_TX_PKT_TYPE_ANY                  = 0x01,  //�������ݰ�
  COORD_ALLOW_NODE_TX_PKT_TYPE_ADDR_RELATE          = 0x02,  //��ϵ��ַ��
  COORD_ALLOW_NODE_TX_PKT_TYPE_ADDR_RELATE_RECEIVED = 0x03
};

/***************************************************************************/
//------------------������ �ű긺��-------------------------

//������ʱ�ű긺�ػ�����Ϣ
/*��ͨ������£�����״̬����Э������㲥�������ű긺��
  ���ű긺����������
*/
typedef struct
{
  BEACON_TAPE  beaconType;  //�ű긺������  �� x_x �ű긺������
  uint8_t depth;            //���ڵ����
  uint8_t maxDepth;         //��������������
  uint8_t beaconTc;         //��˱�ʱ�䳣��
  
  uint8_t backoffDescDataLoca;  //·�ɽڵ��˱�ʱ�� ������ λ�ã����ű긺����λ�ã�
  uint8_t backoffDescLen;       //·�ɽڵ��˱�ʱ�� ������ ���ݳ���
  
  uint8_t transManageDescLoca;  //Э���������ӽڵ㷢�����ݰ� ������ λ�ã����ű긺����λ�ã�
  uint8_t transManageDescLen;   //Э���������ӽڵ㷢�����ݰ� ������ ���ݳ��ȣ�BYTE��
}SelfNetBeaclodBaseInforStruct;
 
////·�����˱�ʱ�估����Ư�ƽṹ��
///*·�ɽڵ��˱�ʱ�� ������
//  ����·�����˱�����ʱ�� �� ����Ư��
//  �ɼ��ڵ�ͨ����������Ϣ�ɼ����·�ɽڵ�ʵ���˱�ʱ�䳤��
//*/
//typedef struct
//{
//  uint16_t backoffTime;  //·���������˱�ʱ��
//  uint16_t COdrift;      //·��������Ư��
//}BackoffDescStruct;

////Э����ָ���ڵ㷢��ָ���������ݰ��ṹ��
///*����ָ���ڵ�ID �� ָ�����ݰ�����
//*/
//typedef struct
//{
//  uint8_t nodeID;   //ָ���ڵ�ID
//  uint8_t pktType;  //ָ�����ݰ����� �� x_x Э����ָ���ڵ������͵���������
//}TransManageDescStruct;

//------------------------�ű긺�������� ����������---------------
typedef union
{
  CommandUnion command; 
}beaclodDataUnion;

//------------------------�ű긺�������� ����������---------------
typedef struct
{
  uint8_t              beaconType;  //�ű�����
  beaclodDataUnion   data;        //�ű�����
}BeaclodStruct;

////------------------------�ű�������------------------------------
///*���ڱ������Ҫ����ű������Ϣ
//  ���� �ű�MAC����Ϣ �� �ű��û���Ϣ
//*/
//#define BEACON_DESCR_NUM 5  //�洢�ű�����

////�ű�MAC����Ϣ�ṹ��
//typedef struct
//{
//  uint8_t permitJoin;      //�Ƿ��������
//  uint8_t logicalChannel;  //�ŵ�
//  uint8_t beaconOrder;     //�ű�����
//  uint8_t superFrameOrder; //��֡����
//  uint8_t linkQuality;     //�ŵ�����
//  
//  uint16_t coordPanId;     //PANID
//  uint16_t coordShortAddr; //Э����ID
//  uint32_t timestamp;      //���յ����ű�ʱ���
//}BeaconMacInforStruct;

////������ʱ�ű��û���Ϣ
///*������ģʽ�£��ڵ�������ɵļ�������
//*/
//typedef struct
//{
//  uint8_t beaconType;  //�ű긺������  �� x_x �ű긺������
//  uint8_t depth;       //���ڵ����
//  uint8_t maxDepth;    //��������������
//  uint8_t beaconTc;    //��˱�ʱ�䳣��
//}SelfNetformBeacUserInforStruct;

////�ű��û���Ϣ��ͬ��
//typedef union
//{
//  SelfNetformBeacUserInforStruct selfNetform;  //������ģʽ�� �ű��û���Ϣ
//}BeaconUserInforUnion;

////�ű���Ϣ������
//typedef struct
//{                 
//  BeaconMacInforStruct macInfor;
//  BeaconUserInforUnion userInfor;
//}BeaconDescrStruct;

////���� �ű���Ϣ �Ľṹ��
///*�ýṹ���û����� �ű���Ϣ �� ��ؿ�����Ϣ
//*/
//typedef struct
//{
//  uint8_t isSaveBeacon;
//  
//  uint8_t beaconNum;
//  uint8_t currentBeacon;  //0xff��ʾ��û�г��Լ�����ű꣬<BEACON_DESCR_NUM ��ʾ �Ѿ������˵ڼ����ű�
//  
//  BeaconDescrStruct beaconDescr[BEACON_DESCR_NUM];
//}SavedBeaconDescrStruct;  //�Ѿ��洢���ű���Ϣ�����ṹ��  

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#endif /*_BEACON_PROTOCOL_H*/

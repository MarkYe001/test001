/***********************************************************************************/
//#include "head_file.h"
#include "delay_tim4.h"
/***********************************************************************************/

uint32_t Timedelay_tim4 = 0;
/***********************************************************************************/

void Timedelay_tim4_init(void)
{
#if tim4_us_enable
	
	TIM4_Init(9,7);      //   1us 1���ж�
	
#elif tim4_ms_enable
	
	TIM4_Init(999,79);   //   1ms 1���ж�
	
#else //tim4_s_enable
	
	TIM4_Init(9999,7999);//   1s  1���ж�
	
#endif
	
	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
	
}

//ͨ�ö�ʱ��4�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//                       Tout=((9+1)*(7+1))/80 us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Delay_Tim4_PreemptionPriority; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Delay_Tim4_SubPriority; //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		if(Timedelay_tim4!=0)Timedelay_tim4--;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //��Ҫ�������жϱ�־λ
}
/***********************************************************************************/

#if tim4_us_enable
void Timedelay_tim4_us(uint32_t nus)
{
	Timedelay_tim4 = nus;
	TIM_Cmd(TIM4,ENABLE); //�򿪶�ʱ��4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
}
void Timedelay_tim4_ms(uint32_t nms)
{
	Timedelay_tim4 = 1000*nms;
	TIM_Cmd(TIM4,ENABLE); //�򿪶�ʱ��4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
}
void Timedelay_tim4_s(uint32_t ns)
{
	Timedelay_tim4 = 1000000*ns;
	TIM_Cmd(TIM4,ENABLE); //�򿪶�ʱ��4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
}
#endif
/***********************************************************************************/

#if tim4_ms_enable

void Timedelay_tim4_ms(uint32_t nms)
{
	Timedelay_tim4 = nms;
	TIM_Cmd(TIM4,ENABLE); //�򿪶�ʱ��4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
}
void Timedelay_tim4_s(uint32_t ns)
{
	Timedelay_tim4 = 1000*ns;
	TIM_Cmd(TIM4,ENABLE); //�򿪶�ʱ��4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
}
#endif
/***********************************************************************************/

#if tim4_s_enable

void Timedelay_tim4_s(uint32_t ns)
{
	Timedelay_tim4 = ns;
	TIM_Cmd(TIM4,ENABLE); //�򿪶�ʱ��4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
}
#endif
/***********************************************************************************/

/***********************************************************************************/
//#include "head_file.h"
#include "delay_tim4.h"
/***********************************************************************************/

uint32_t Timedelay_tim4 = 0;
/***********************************************************************************/

void Timedelay_tim4_init(void)
{
#if tim4_us_enable
	
	TIM4_Init(9,7);      //   1us 1次中断
	
#elif tim4_ms_enable
	
	TIM4_Init(999,79);   //   1ms 1次中断
	
#else //tim4_s_enable
	
	TIM4_Init(9999,7999);//   1s  1次中断
	
#endif
	
	TIM_Cmd(TIM4,DISABLE); //关闭定时器4
	
}

//通用定时器4中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//                       Tout=((9+1)*(7+1))/80 us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器4!
void TIM4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Delay_Tim4_PreemptionPriority; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Delay_Tim4_SubPriority; //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		if(Timedelay_tim4!=0)Timedelay_tim4--;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //需要软件清除中断标志位
}
/***********************************************************************************/

#if tim4_us_enable
void Timedelay_tim4_us(uint32_t nus)
{
	Timedelay_tim4 = nus;
	TIM_Cmd(TIM4,ENABLE); //打开定时器4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //关闭定时器4
}
void Timedelay_tim4_ms(uint32_t nms)
{
	Timedelay_tim4 = 1000*nms;
	TIM_Cmd(TIM4,ENABLE); //打开定时器4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //关闭定时器4
}
void Timedelay_tim4_s(uint32_t ns)
{
	Timedelay_tim4 = 1000000*ns;
	TIM_Cmd(TIM4,ENABLE); //打开定时器4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //关闭定时器4
}
#endif
/***********************************************************************************/

#if tim4_ms_enable

void Timedelay_tim4_ms(uint32_t nms)
{
	Timedelay_tim4 = nms;
	TIM_Cmd(TIM4,ENABLE); //打开定时器4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //关闭定时器4
}
void Timedelay_tim4_s(uint32_t ns)
{
	Timedelay_tim4 = 1000*ns;
	TIM_Cmd(TIM4,ENABLE); //打开定时器4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //关闭定时器4
}
#endif
/***********************************************************************************/

#if tim4_s_enable

void Timedelay_tim4_s(uint32_t ns)
{
	Timedelay_tim4 = ns;
	TIM_Cmd(TIM4,ENABLE); //打开定时器4
	while(Timedelay_tim4!=0);
	TIM_Cmd(TIM4,DISABLE); //关闭定时器4
}
#endif
/***********************************************************************************/

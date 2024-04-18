#include "moto.h"


/**************************************************************************
函数功能：电机的正反转
入口参数：mode   mode=0时为正转  mode=1时反转
返回  值：无
**************************************************************************/


void moto(int mode)
{
	if(mode==0)
	{
	 GPIO_SetBits(GPIOE, GPIO_Pin_9);	 // 高电平
	 GPIO_ResetBits(GPIOE, GPIO_Pin_11);	 // 低电平
		
	 TIM_SetCompare1(TIM4,3000);   //设置TIM4通道1的占空比  3000/7200
	}
	 if(mode==1)
	{
	 GPIO_SetBits(GPIOE, GPIO_Pin_11);	 // 高电平
	 GPIO_ResetBits(GPIOE, GPIO_Pin_9);	 // 低电平}
		
	 TIM_SetCompare1(TIM4,4000);   //设置TIM4通道1的占空比  4000/7200
	 }
	if(mode==2)
	{
	 GPIO_ResetBits(GPIOE, GPIO_Pin_11);	 // 高电平
	 GPIO_ResetBits(GPIOE, GPIO_Pin_9);	 // 低电平}
		
	 TIM_SetCompare1(TIM4,4000);   //设置TIM4通道1的占空比  4000/7200
	 }
 
}


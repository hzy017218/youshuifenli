#include "moto.h"


/**************************************************************************
�������ܣ����������ת
��ڲ�����mode   mode=0ʱΪ��ת  mode=1ʱ��ת
����  ֵ����
**************************************************************************/


void moto(int mode)
{
	if(mode==0)
	{
	 GPIO_SetBits(GPIOE, GPIO_Pin_9);	 // �ߵ�ƽ
	 GPIO_ResetBits(GPIOE, GPIO_Pin_11);	 // �͵�ƽ
		
	 TIM_SetCompare1(TIM4,3000);   //����TIM4ͨ��1��ռ�ձ�  3000/7200
	}
	 if(mode==1)
	{
	 GPIO_SetBits(GPIOE, GPIO_Pin_11);	 // �ߵ�ƽ
	 GPIO_ResetBits(GPIOE, GPIO_Pin_9);	 // �͵�ƽ}
		
	 TIM_SetCompare1(TIM4,4000);   //����TIM4ͨ��1��ռ�ձ�  4000/7200
	 }
	if(mode==2)
	{
	 GPIO_ResetBits(GPIOE, GPIO_Pin_11);	 // �ߵ�ƽ
	 GPIO_ResetBits(GPIOE, GPIO_Pin_9);	 // �͵�ƽ}
		
	 TIM_SetCompare1(TIM4,4000);   //����TIM4ͨ��1��ռ�ձ�  4000/7200
	 }
 
}


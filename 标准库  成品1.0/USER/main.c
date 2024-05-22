/************************************************************************************
							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "HX711.h"
#include "usart.h"
#include "gpio.h"
#include "moto.h"
//#include "key.h"
//#include "pwm.h"

void Delay_ms( uint16_t time_ms );
int main(void)
{		
	Init_HX711pin();
	SystemInit(); //����ϵͳʱ��Ϊ72M 
	delay_init();
	Gpio_Init();    //��ʼ��gpio��B pin_7/pin_8
//	KEY_GPIO_Config();/*��ʼ��KEY�˿�*/	
//	pwm_int();      //��ʼ��pwm��� 72000 000 /7199+1=10000 
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	
	Get_Maopi();				//��ëƤ����
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//���»�ȡëƤ����

   		     
	while(1)
	{
		
//		if(KEY1_Status_Read()==0||KEY2_Status_Read()==0||KEY3_Status_Read()==0)//�ж�3�������Ƿ��а���
//		{
//			Delay_ms(20);
//			if(KEY1_Status_Read()==0)//ȷ��S1������
//			{
//				Delay_ms(250);
//					moto(0);;
//			}
//			else if(KEY2_Status_Read()==0)//ȷ��S2������
//			{
//				Delay_ms(250);
//	     	moto(1);
//			}
//			else if(KEY3_Status_Read()==0)//ȷ��S3������
//			{
//				Delay_ms(250);
//					moto(2);
//			}
		Get_Weight();

	//	printf("������ = %d g\r\n",Weight_Shiwu); //��ӡ 
		Delay_ms(300); 
		printf("%d\n",Weight_Shiwu);
//		moto(0);      //down
//		while(Weight_Shiwu > 500)//�Ƿ����
//		{
//			Get_Weight();
//			Delay_ms(100);
//			printf("%d\n",Weight_Shiwu);
//		}
//		Get_Weight();
//		Delay_ms(100);
//		moto(2);
//		Delay_ms(100);
//		moto(1);
//		while(Weight_Shiwu < 1000) //�Ƿ��ˮ
//			{
//			Delay_ms(1000);
//			Get_Weight();				
//			Delay_ms(100);
//			printf("%d\n",Weight_Shiwu);
//			Get_Weight();				
//			Delay_ms(100);
//			printf("%d\n",Weight_Shiwu);
//		}
//			Delay_ms(1000);
//  	moto(2);
//		Delay_ms(5000);
	}
}
// /**
//  * @file   Delay_ms������ȷ��
//  * @brief  ������ʱtime_ms ms
//  * @param   time_ms ��ʱʱ��
//  * @retval ��
//  */
void Delay_ms( uint16_t time_ms )
{
  uint16_t i,j;
  for( i=0;i<time_ms;i++ )
  {
		for( j=0;j<10309;j++ );//��Լ1ms
  }



}
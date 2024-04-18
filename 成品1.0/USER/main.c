/************************************************************************************
							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "HX711.h"
#include "usart.h"
#include "gpio.h"
#include "moto.h"
//#include "pwm.h"


int main(void)
{		
	Init_HX711pin();
	SystemInit(); //����ϵͳʱ��Ϊ72M 
	delay_init();
	Gpio_Init();    //��ʼ��gpio��B pin_7/pin_8
//	pwm_int();      //��ʼ��pwm��� 72000 000 /7199+1=10000 
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	
	Get_Maopi();				//��ëƤ����
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//���»�ȡëƤ����
	
	
   		     
	while(1)
	{
		Get_Weight();

		printf("������ = %d g\r\n",Weight_Shiwu); //��ӡ 
		
		if (Weight_Shiwu > 500)
			{
		moto(0);      //�����ת
			}
		if (Weight_Shiwu < 500)
			{
		moto(2);      //���ֹͣ
			}	
		delay_ms(100);
	
          //moto=0ʱ��ת

	}
}



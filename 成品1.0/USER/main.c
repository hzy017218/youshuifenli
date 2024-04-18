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
	SystemInit(); //配置系统时钟为72M 
	delay_init();
	Gpio_Init();    //初始化gpio口B pin_7/pin_8
//	pwm_int();      //初始化pwm输出 72000 000 /7199+1=10000 
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	
	Get_Maopi();				//称毛皮重量
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//重新获取毛皮重量
	
	
   		     
	while(1)
	{
		Get_Weight();

		printf("净重量 = %d g\r\n",Weight_Shiwu); //打印 
		
		if (Weight_Shiwu > 500)
			{
		moto(0);      //电机正转
			}
		if (Weight_Shiwu < 500)
			{
		moto(2);      //电机停止
			}	
		delay_ms(100);
	
          //moto=0时反转

	}
}



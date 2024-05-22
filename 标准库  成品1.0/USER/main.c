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
	SystemInit(); //配置系统时钟为72M 
	delay_init();
	Gpio_Init();    //初始化gpio口B pin_7/pin_8
//	KEY_GPIO_Config();/*初始化KEY端口*/	
//	pwm_int();      //初始化pwm输出 72000 000 /7199+1=10000 
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	
	Get_Maopi();				//称毛皮重量
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//重新获取毛皮重量

   		     
	while(1)
	{
		
//		if(KEY1_Status_Read()==0||KEY2_Status_Read()==0||KEY3_Status_Read()==0)//判断3个按键是否有按下
//		{
//			Delay_ms(20);
//			if(KEY1_Status_Read()==0)//确认S1键按下
//			{
//				Delay_ms(250);
//					moto(0);;
//			}
//			else if(KEY2_Status_Read()==0)//确认S2键按下
//			{
//				Delay_ms(250);
//	     	moto(1);
//			}
//			else if(KEY3_Status_Read()==0)//确认S3键按下
//			{
//				Delay_ms(250);
//					moto(2);
//			}
		Get_Weight();

	//	printf("净重量 = %d g\r\n",Weight_Shiwu); //打印 
		Delay_ms(300); 
		printf("%d\n",Weight_Shiwu);
//		moto(0);      //down
//		while(Weight_Shiwu > 500)//是否挂球
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
//		while(Weight_Shiwu < 1000) //是否出水
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
//  * @file   Delay_ms（不精确）
//  * @brief  毫秒延时time_ms ms
//  * @param   time_ms 延时时间
//  * @retval 无
//  */
void Delay_ms( uint16_t time_ms )
{
  uint16_t i,j;
  for( i=0;i<time_ms;i++ )
  {
		for( j=0;j<10309;j++ );//大约1ms
  }



}
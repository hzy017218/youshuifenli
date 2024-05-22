#include "HX711.h"
#include "usart.h"
#include "gpio.h"

unsigned long val =0; 
int pulse_mun=0;
long HX711_Buffer = 0,First_Weight=0,Weight=0;


void HAL_Delay_us(uint32_t delay)
{
	delay=delay*5;
	while(delay)
	{
		delay-=1;
	}
}


long Get_Weight(void)
{
	HX711_Buffer=Get_number();
	Weight = HX711_Buffer;
	Weight=(long)((float)Weight/103);
	return Weight;
}

unsigned long Get_number()
{
	val=0;
		  CLK_0;
		  while(!Read_PIN);
		  for(int i=0;i<24;i++)
		  {
			  HAL_Delay_us(100);
			  CLK_1;
			  val=val<<1;
			  HAL_Delay_us(1);
			  CLK_0;
				HAL_Delay_us(1);
			  if(Read_PIN)
			  val++;
			  HAL_Delay_us(1);

		  }
		  CLK_1;
		  val = val^0x800000; 
		  HAL_Delay_us(1);
		  CLK_0;
		  return val;
		  
}
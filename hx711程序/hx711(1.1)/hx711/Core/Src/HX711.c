//#include "HX711.h"
////#include "usart.h"
////#include "gpio.h"
// 
//unsigned long val =0; 
//int pulse_mun=0;
//long HX711_Buffer = 0,First_Weight=0,Weight=0;
// 
// 
//void HAL_Delay_us(uint32_t delay)
//{
//	delay=delay*5;
//	while(delay)
//	{
//		delay-=1;
//	}
//}
// 
// 
//long Get_Weight(void)
//{
//	HX711_Buffer=Get_number();
//	Weight = HX711_Buffer;
//	Weight=(long)((float)Weight/103);
//	return Weight;
//}
// 
//unsigned long Get_number()
//{
//	val=0;
//		  CLK_0;
//		  while(!Read_PIN);
//		  for(int i=0;i<24;i++)
//		  {
//			  HAL_Delay_us(100);
//			  CLK_1;
//			  val=val<<1;
//			  HAL_Delay_us(1);
//			  CLK_0;
//				HAL_Delay_us(1);
//			  if(Read_PIN)
//			  val++;
//			  HAL_Delay_us(1);
// 
//		  }
//		  CLK_1;
//		  val = val^0x800000; 
//		  HAL_Delay_us(1);
//		  CLK_0;
//		  return val;
//		  
//}




#include "HX711.h"

//???IIC  ? PC0->DOUT PC1->PD_SCK
void Hx711_Init(void)// ???GPIO???
{                         
    GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_APB2PeriphClockCmd(    RCC_APB2Periph_GPIOA, ENABLE );    //??GPIOA??
    __HAL_RCC_GPIOB_CLK_ENABLE (  );//?????????
    /*????SDA*/
    GPIO_InitStructure.Pin  = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT ;          //????
    GPIO_InitStructure.Speed =  GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull  = GPIO_NOPULL;            
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);     // ???
    /*????SCK*/
    GPIO_InitStructure.Pin  = GPIO_PIN_1;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP ;   //????
    GPIO_InitStructure.Speed =  GPIO_SPEED_FREQ_HIGH;    
    GPIO_InitStructure.Pull  = GPIO_PULLDOWN;         //  HX711?PD_SCK ?????,?????????
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET); // ???
}
// ?????
 uint32_t get_hx711_ad(void)// ?????
{ 
     unsigned long Count; 
     unsigned char i; 
     ADSK=0; 
     Count=0; 
     while(ADDO); //  HX711?DOUT ?????,?? A/D ????????????
     for (i=0;i<24;i++){ 
         ADSK=1; // ????
         Count=Count<<1; 
         ADSK=0; // ????
         if(ADDO) Count++; 
     } 
     ADSK=1;   // ????, ???25?????,??AD?????(??)?128
     Count=Count^0x800000; 
     ADSK=0;   // HX711?PD_SCK ?????,?????????
     return(Count); 
}
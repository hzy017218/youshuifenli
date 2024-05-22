//#ifndef _HX711_H_
//#define _HX711_H_
//#include "main.h"

//#define CLK_1 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1)
//#define CLK_0 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0)
//#define Read_PIN HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)
//#define Gain_128 25
//#define Gain_64 27
//#define Gain_32 26
//void HAL_Delay_us(uint32_t delay);
//unsigned long Get_number(void);
//long Get_Weight(void);

//#endif



#ifndef __HX711_H
#define __HX711_H
#include "main.h"

/*??????*/
//??IO?
#define ADDO PBin(0)
#define ADSK PBout(1)

//??AD???
uint32_t get_hx711_ad(void);//??AD???

void Hx711_Init(void);// ???GPIO???
#endif
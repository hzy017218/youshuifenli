#ifndef __HX711_H
#define __HX711_H

#include "main.h"

#define HX711_SCK PBout(1)// PB0
#define HX711_DOUT PBin(0)// PB1


extern void MX_GPIO_Init(void);
//extern uint32_t Read_HX711(void);
//extern void Read_HX711(void);
//void HAL_Delay_us(uint32_t delay);
extern void Get_Maopi(void);
extern void Get_Weight(void);
uint32_t Read_HX711(void);

extern uint32_t HX711_Buffer;
extern uint32_t Weight_Maopi;
extern uint32_t Weight_Shiwu;
extern uint8_t Flag_Error;


#endif


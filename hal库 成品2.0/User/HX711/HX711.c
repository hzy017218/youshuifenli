
#include "stm32f1xx_hal.h"
#include "HX711.h"


uint32_t HX711_Buffer;
uint32_t Weight_Maopi;
uint32_t Weight_Shiwu;
// ????PA0??DOUT,PA1??SCK
#define HX711_DOUT_PIN GPIO_PIN_0
#define HX711_SCK_PIN GPIO_PIN_1

#define HX711_DOUT_PORT GPIOB
#define HX711_SCK_PORT GPIOB


//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 1000

// ???GPIO??
void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // ??GPIO??
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // ??PA0?????
    GPIO_InitStruct.Pin = HX711_DOUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(HX711_DOUT_PORT, &GPIO_InitStruct);

    // ??PA1?????
    GPIO_InitStruct.Pin = HX711_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX711_SCK_PORT, &GPIO_InitStruct);
}

// ??HX711???
uint32_t Read_HX711(void)
{
    uint32_t data = 0;
    uint8_t i;

    // ??SCK?????
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);

    // ??24?????????
    for (i = 0; i < 24; i++)
    {
        // ??SCK????
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);

        // ??DOUT??
        data = (data << 1) | HAL_GPIO_ReadPin(HX711_DOUT_PORT, HX711_DOUT_PIN);

        // ??SCK????
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    }

    return data;
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = Read_HX711();	
} 

//****************************************************
//称重
//****************************************************
void Get_Weight(void)
{
	HX711_Buffer = Read_HX711();
//	if(HX711_Buffer > Weight_Maopi)			
//	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	
//		Weight_Shiwu = (uint32_t)((float)Weight_Shiwu/GapValue) + 0; 	//计算实物的实际重量
																		//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
																		//当发现测试出来的重量偏大时，增加该数值。
																		//如果测试出来的重量偏小时，减小改数值。
//	}

	
}

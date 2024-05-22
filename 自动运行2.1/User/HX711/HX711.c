
#include "stm32f1xx_hal.h"
#include "HX711.h"


uint32_t HX711_Buffer;
uint32_t Weight_Maopi;
int32_t Weight_Shiwu;

/**
 * @brief 定义HX711引脚
 * @attention
 * DOUT--->PB0
 * SCK---->PB1
 */
//定义HX711芯片的数据输出引脚和时钟引脚
 #define HX711_DOUT_PIN GPIO_PIN_0 // 定义HX711的数据输出引脚为GPIO_PIN_0
#define HX711_SCK_PIN GPIO_PIN_1 // 定义HX711的时钟引脚为GPIO_PIN_1
//定义HX711芯片的数据输出端口和时钟端口
#define HX711_DOUT_PORT GPIOB // 定义HX711的数据输出端口为GPIOB
#define HX711_SCK_PORT GPIOB // 定义HX711的时钟端口为GPIOB


//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 450

// GPIO初始化
void HX711_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置HX711的SCK引脚
    GPIO_InitStruct.Pin = HX711_DOUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(HX711_DOUT_PORT, &GPIO_InitStruct);

    // 配置HX711的DOUT引脚
    GPIO_InitStruct.Pin = HX711_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX711_SCK_PORT, &GPIO_InitStruct);
}

/**
 * 读取HX711数据
 * 
 * 本函数用于从HX711芯片中读取数据。HX711是一款专用的A/D转换器，用于称重传感器接口。
 * 
 * @return 返回从HX711读取的32位数据。
 */
uint32_t Read_HX711(void)
{
    uint32_t data = 0; // 初始化用于存储从HX711读取数据的变量
    uint8_t i;

    // 初始化SCK引脚为低电平，准备开始数据读取
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    Delay_us(1); // 延时1us
    // 依次读取HX711数据的24位
    for (i = 0; i < 24; i++)
    {
        // 将SCK引脚设置为高电平，准备采样
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
        Delay_us(1); // 延时1us 
        // 读取DOUT引脚的数据，并将其加入到data中
        data = (data << 1) | HAL_GPIO_ReadPin(HX711_DOUT_PORT, HX711_DOUT_PIN);
        Delay_us(1); // 延时1us 
        // 将SCK引脚设置为低电平，完成一次采样
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
        Delay_us(1); // 延时1us 
    }

    // 第25个脉冲下降沿来时，转换数据
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
    Delay_us(1); // 延时1us 
    data = data ^ 0x800000; // 第25个脉冲数据位翻转
    Delay_us(1); // 延时1us
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    Delay_us(1); // 延时1us
    return data; // 返回读取到的完整数据
}
void Delay_us(uint32_t delay)
{
    uint32_t Delay = delay * 72;
    do
    {
        __NOP();
    }
    while (Delay --);
}
//****************************************************
// 获取毛皮重量
//****************************************************
int Get_Maopi(void)
{
	Weight_Maopi = Read_HX711();
    return	1;
} 

//****************************************************
//称重
//****************************************************
void Get_Weight(void)
{
HX711_Buffer = Read_HX711();

    Weight_Shiwu = HX711_Buffer;
    Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。

    Weight_Shiwu = (int32_t)((float)Weight_Shiwu/GapValue) + 520; 	//计算实物的实际重量
//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
}

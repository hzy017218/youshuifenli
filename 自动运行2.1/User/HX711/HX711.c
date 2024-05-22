
#include "stm32f1xx_hal.h"
#include "HX711.h"


uint32_t HX711_Buffer;
uint32_t Weight_Maopi;
int32_t Weight_Shiwu;

/**
 * @brief ����HX711����
 * @attention
 * DOUT--->PB0
 * SCK---->PB1
 */
//����HX711оƬ������������ź�ʱ������
 #define HX711_DOUT_PIN GPIO_PIN_0 // ����HX711�������������ΪGPIO_PIN_0
#define HX711_SCK_PIN GPIO_PIN_1 // ����HX711��ʱ������ΪGPIO_PIN_1
//����HX711оƬ����������˿ں�ʱ�Ӷ˿�
#define HX711_DOUT_PORT GPIOB // ����HX711����������˿�ΪGPIOB
#define HX711_SCK_PORT GPIOB // ����HX711��ʱ�Ӷ˿�ΪGPIOB


//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 450

// GPIO��ʼ��
void HX711_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // ʹ��ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // ����HX711��SCK����
    GPIO_InitStruct.Pin = HX711_DOUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(HX711_DOUT_PORT, &GPIO_InitStruct);

    // ����HX711��DOUT����
    GPIO_InitStruct.Pin = HX711_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX711_SCK_PORT, &GPIO_InitStruct);
}

/**
 * ��ȡHX711����
 * 
 * ���������ڴ�HX711оƬ�ж�ȡ���ݡ�HX711��һ��ר�õ�A/Dת���������ڳ��ش������ӿڡ�
 * 
 * @return ���ش�HX711��ȡ��32λ���ݡ�
 */
uint32_t Read_HX711(void)
{
    uint32_t data = 0; // ��ʼ�����ڴ洢��HX711��ȡ���ݵı���
    uint8_t i;

    // ��ʼ��SCK����Ϊ�͵�ƽ��׼����ʼ���ݶ�ȡ
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    Delay_us(1); // ��ʱ1us
    // ���ζ�ȡHX711���ݵ�24λ
    for (i = 0; i < 24; i++)
    {
        // ��SCK��������Ϊ�ߵ�ƽ��׼������
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
        Delay_us(1); // ��ʱ1us 
        // ��ȡDOUT���ŵ����ݣ���������뵽data��
        data = (data << 1) | HAL_GPIO_ReadPin(HX711_DOUT_PORT, HX711_DOUT_PIN);
        Delay_us(1); // ��ʱ1us 
        // ��SCK��������Ϊ�͵�ƽ�����һ�β���
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
        Delay_us(1); // ��ʱ1us 
    }

    // ��25�������½�����ʱ��ת������
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
    Delay_us(1); // ��ʱ1us 
    data = data ^ 0x800000; // ��25����������λ��ת
    Delay_us(1); // ��ʱ1us
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    Delay_us(1); // ��ʱ1us
    return data; // ���ض�ȡ������������
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
// ��ȡëƤ����
//****************************************************
int Get_Maopi(void)
{
	Weight_Maopi = Read_HX711();
    return	1;
} 

//****************************************************
//����
//****************************************************
void Get_Weight(void)
{
HX711_Buffer = Read_HX711();

    Weight_Shiwu = HX711_Buffer;
    Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��

    Weight_Shiwu = (int32_t)((float)Weight_Shiwu/GapValue) + 520; 	//����ʵ���ʵ������
//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
}

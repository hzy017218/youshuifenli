/**
 ******************************************************************************
 * @file    main.c
 * @author  FYS
 * @version V0.5
 * @date    2024-05-14
 * @brief   �����������+���������+��������
 ******************************************************************************
 * @attention
 * ʵ��ƽ̨:Ұ�� F103-���� STM32 ������
 ******************************************************************************
 */

/* -------------------------------- Includes -------------------------------- */
#include "main.h"
#include <stdlib.h>
#include ".\motor_control\bsp_motor_control.h"
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"
#include "./usart/bsp_debug_usart.h"
#include "./Encoder/bsp_encoder.h"
#include "HX711.h"


/* -------------------------------- ����ȫ�ֱ��� ------------------------------ */
/* �����ת���� */
__IO int8_t Motor_Direction = 0;
/* ��ǰʱ���ܼ���ֵ */
__IO int32_t Capture_Count = 0;
/* ��һʱ���ܼ���ֵ */
__IO int32_t Last_Count = 0;
/* ���ת��ת�� */
__IO float Shaft_Speed = 0.0f;
/* �����ǣ������ */
__IO float TotalDistance = 1.0f;


/*******************************************************************************
 * @brief  ������
 * @param  ��  
 * @retval ��
 */
int main(void)
{
  /* -------------------------------- ��ʼ�� ------------------------------ */
  uint8_t enable = 0;//���ʹ�ܱ�־λ
  uint8_t i = 1;//��������־λ
  uint8_t Auto = 1;//�Զ����б�־λ

  /*���õ��ռ�ձ�--->���õ���ٶ�Ϊ 60% */
  __IO uint16_t ChannelPulse = PWM_MAX_PERIOD_COUNT * 0.6;
  /* HAL���ʼ��*/
  HAL_Init();
  /* ��ʼ��GPIO */
  HX711_GPIO_Init();
  /* ��ʼ��ϵͳʱ��Ϊ72MHz */
  SystemClock_Config();
  /* �������üĴ���ʱ�� */
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  /* ����1msʱ����SysTick */
  HAL_InitTick(5);
  /* ��ʼ������GPIO */
  Key_GPIO_Config();
  /* ��ʼ��USART */
  DEBUG_USART_Config();
  
  /* �����ʼ�� */
  motor_init();

  /* �����ٶ� */
  set_motor_disable();
  set_motor_speed(ChannelPulse);

  /* �������ӿڳ�ʼ�� */
  Encoder_Init();

  printf("\r\n���������������������Լ���ɡ�������������������\r\n");
  printf("\r\n����������������������V1.3 ������������������������\r\n");


/* ����ɨ�� */
  while (1)
  {
    /* -------------------------------- �Զ����� ------------------------------ */
    if (Auto == 1 && enable == 1)
    {
      if (TotalDistance < 0.09 && Motor_Direction == 1)//1=fwd
      {
			HAL_Delay(5);
      set_motor_direction(MOTOR_REV);
      }
      if (Weight_Shiwu > 480 && TotalDistance > 0.97 && Motor_Direction == 0)
      {
        set_motor_disable();
        enable = 0;
      }
    }


    /* -------------------------------- ����ɨ�� ------------------------------ */
    /* ɨ��KEY1 */
    if (Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      if (enable == 0)
      {
        /* ʹ�ܵ�� */
        set_motor_enable();
				HAL_Delay(200);
        enable = 1;
      }
      else
      {
        /* ���õ�� */
        set_motor_disable();
        enable = 0;
      }
    }

    /* ɨ��KEY2 */
    if (Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
    {
      /* ����ռ�ձ� */
      ChannelPulse += PWM_MAX_PERIOD_COUNT / 10;//�ٶ�����10%
      if (ChannelPulse > PWM_MAX_PERIOD_COUNT)//�ٶ����ֵ
        ChannelPulse = PWM_MAX_PERIOD_COUNT;
      
      set_motor_speed(ChannelPulse);
    }

    /* ɨ��KEY3 */
    if (Key_Scan(KEY3_GPIO_PORT, KEY3_PIN) == KEY_ON)
    {
      /* ��Сռ�ձ� */
      if (ChannelPulse < PWM_MAX_PERIOD_COUNT / 10)//�ٶ���Сֵ
        ChannelPulse = 0;
      else
        ChannelPulse -= PWM_MAX_PERIOD_COUNT / 10;//�ٶȼ�С10%
      
      set_motor_speed(ChannelPulse);
    }

    /* ɨ��KEY4 */
    if (Key_Scan(KEY4_GPIO_PORT, KEY4_PIN) == KEY_ON)
    {
      /* ת������ */
      set_motor_direction((++i % 2) ? MOTOR_FWD : MOTOR_REV);
      Auto = 0;
    }
  }
}


/*********************************  �ж�  *****************************************
 * @brief  SysTick�жϻص�����
 * @param  ��
 * @retval ��
 */
void HAL_SYSTICK_Callback(void)
{
  static uint16_t i = 0;//������
  static uint16_t t = 200;//����ʱ��
  static uint16_t flag = 0;//ë�ر�־λ
  if (flag == 0)//��ëƤ
  {
    int OK = 0;
    while (OK != 1)//ȷ����ëƤ�ɹ�
    {
      OK = Get_Maopi();
    }
    OK = 0;
    flag = 1;
  }
  i++;
  if (i == t) /* t ms����һ�� */
  {
    if (flag == 1)//�ٴγ�ëƤ
    {
      int OK = 0;
      while (OK != 1)//�����ëƤʧ�ܣ�һֱ�ظ���ëƤ
      {
        OK = Get_Maopi();
      }
      flag = 2;
    }
    /* �����ת���� = �������������� */
    Motor_Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&TIM_EncoderHandle);

    /* ��ǰʱ���ܼ���ֵ = ������ֵ + ����������� * ENCODER_TIM_PERIOD  */
    Capture_Count = __HAL_TIM_GET_COUNTER(&TIM_EncoderHandle) + (Encoder_Overflow_Count * ENCODER_TIM_PERIOD);

    /* ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ��� * ʱ��ϵ��  */
    Shaft_Speed = (float)(Capture_Count - Last_Count) / ENCODER_TOTAL_RESOLUTION * (1000 / (float)t);

    /* ���ǣ������ */
    float DistancePerUnitTime = (Shaft_Speed / REDUCTION_RATIO) * D * pi * ((float)t / 1000); /* ��λʱ��ǣ������ = (�����ת�� * ���ǣ����ֱ�� * ��) * ��λʱ�� */
    TotalDistance += DistancePerUnitTime; // ���ǣ������
    Get_Weight(); //��ȡ����

//    printf("�������%d\r\n", Motor_Direction);
//    printf("��λʱ������Ч����ֵ��%d\r\n", Capture_Count - Last_Count); /* ��λʱ�����ֵ = ��ǰʱ���ܼ���ֵ - ��һʱ���ܼ���ֵ */
//    printf("���ת�ᴦת�٣�%.2f ת/�� \r\n", Shaft_Speed);
//    printf("��������ת�٣�%.2f ת/�� \r\n", Shaft_Speed / REDUCTION_RATIO); /* �����ת�� = ת��ת�� / ���ٱ� */
//    printf("���ǣ�����ٶȣ�%.2f ��/�� \r\n", (Shaft_Speed / REDUCTION_RATIO) * D * pi); /* �����ת�� * ���ǣ����ֱ�� * pi */
//    printf("���ǣ�����룺%.2f ��\r\n", TotalDistance);//���ǣ������ = ��λʱ��ǣ������ * ��λʱ�� t��s��
//    printf("ëƤ������%d\r\n",Weight_Maopi);
//    printf("������������ֵ: %d \n", Weight_Shiwu);
    printf("%d\n", Weight_Shiwu);
    /* ��¼��ǰ�ܼ���ֵ������һʱ�̼���ʹ�� */
    Last_Count = Capture_Count;
    i = 0;
  }
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 72000000
 *            HCLK(Hz)                       = 72000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 8000000
 *            HSE PREDIV1                    = 2
 *            PLLMUL                         = 9
 *            Flash Latency(WS)              = 0
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
  {
    /* Initialization Error */
    while (1)
      ;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2) != HAL_OK)
  {
    /* Initialization Error */
    while (1)
      ;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

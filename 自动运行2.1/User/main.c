/**
 ******************************************************************************
 * @file    main.c
 * @author  FYS
 * @version V0.5
 * @date    2024-05-14
 * @brief   电机按键控制+编码器测距+拉力测量
 ******************************************************************************
 * @attention
 * 实验平台:野火 F103-拂晓 STM32 开发板
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


/* -------------------------------- 定义全局变量 ------------------------------ */
/* 电机旋转方向 */
__IO int8_t Motor_Direction = 0;
/* 当前时刻总计数值 */
__IO int32_t Capture_Count = 0;
/* 上一时刻总计数值 */
__IO int32_t Last_Count = 0;
/* 电机转轴转速 */
__IO float Shaft_Speed = 0.0f;
/* 电机总牵引距离 */
__IO float TotalDistance = 1.0f;


/*******************************************************************************
 * @brief  主函数
 * @param  无  
 * @retval 无
 */
int main(void)
{
  /* -------------------------------- 初始化 ------------------------------ */
  uint8_t enable = 0;//电机使能标志位
  uint8_t i = 1;//电机方向标志位
  uint8_t Auto = 1;//自动运行标志位

  /*设置电机占空比--->设置电机速度为 60% */
  __IO uint16_t ChannelPulse = PWM_MAX_PERIOD_COUNT * 0.6;
  /* HAL库初始化*/
  HAL_Init();
  /* 初始化GPIO */
  HX711_GPIO_Init();
  /* 初始化系统时钟为72MHz */
  SystemClock_Config();
  /* 开启复用寄存器时钟 */
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  /* 配置1ms时基的SysTick */
  HAL_InitTick(5);
  /* 初始化按键GPIO */
  Key_GPIO_Config();
  /* 初始化USART */
  DEBUG_USART_Config();
  
  /* 电机初始化 */
  motor_init();

  /* 设置速度 */
  set_motor_disable();
  set_motor_speed(ChannelPulse);

  /* 编码器接口初始化 */
  Encoder_Init();

  printf("\r\n——————————自检完成——————————\r\n");
  printf("\r\n———————————V1.3 ————————————\r\n");


/* 按键扫描 */
  while (1)
  {
    /* -------------------------------- 自动运行 ------------------------------ */
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


    /* -------------------------------- 按键扫描 ------------------------------ */
    /* 扫描KEY1 */
    if (Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      if (enable == 0)
      {
        /* 使能电机 */
        set_motor_enable();
				HAL_Delay(200);
        enable = 1;
      }
      else
      {
        /* 禁用电机 */
        set_motor_disable();
        enable = 0;
      }
    }

    /* 扫描KEY2 */
    if (Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
    {
      /* 增大占空比 */
      ChannelPulse += PWM_MAX_PERIOD_COUNT / 10;//速度增加10%
      if (ChannelPulse > PWM_MAX_PERIOD_COUNT)//速度最大值
        ChannelPulse = PWM_MAX_PERIOD_COUNT;
      
      set_motor_speed(ChannelPulse);
    }

    /* 扫描KEY3 */
    if (Key_Scan(KEY3_GPIO_PORT, KEY3_PIN) == KEY_ON)
    {
      /* 减小占空比 */
      if (ChannelPulse < PWM_MAX_PERIOD_COUNT / 10)//速度最小值
        ChannelPulse = 0;
      else
        ChannelPulse -= PWM_MAX_PERIOD_COUNT / 10;//速度减小10%
      
      set_motor_speed(ChannelPulse);
    }

    /* 扫描KEY4 */
    if (Key_Scan(KEY4_GPIO_PORT, KEY4_PIN) == KEY_ON)
    {
      /* 转换方向 */
      set_motor_direction((++i % 2) ? MOTOR_FWD : MOTOR_REV);
      Auto = 0;
    }
  }
}


/*********************************  中断  *****************************************
 * @brief  SysTick中断回调函数
 * @param  无
 * @retval 无
 */
void HAL_SYSTICK_Callback(void)
{
  static uint16_t i = 0;//计数器
  static uint16_t t = 200;//计数时间
  static uint16_t flag = 0;//毛重标志位
  if (flag == 0)//称毛皮
  {
    int OK = 0;
    while (OK != 1)//确保称毛皮成功
    {
      OK = Get_Maopi();
    }
    OK = 0;
    flag = 1;
  }
  i++;
  if (i == t) /* t ms计算一次 */
  {
    if (flag == 1)//再次称毛皮
    {
      int OK = 0;
      while (OK != 1)//如果称毛皮失败，一直重复称毛皮
      {
        OK = Get_Maopi();
      }
      flag = 2;
    }
    /* 电机旋转方向 = 计数器计数方向 */
    Motor_Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&TIM_EncoderHandle);

    /* 当前时刻总计数值 = 计数器值 + 计数溢出次数 * ENCODER_TIM_PERIOD  */
    Capture_Count = __HAL_TIM_GET_COUNTER(&TIM_EncoderHandle) + (Encoder_Overflow_Count * ENCODER_TIM_PERIOD);

    /* 转轴转速 = 单位时间内的计数值 / 编码器总分辨率 * 时间系数  */
    Shaft_Speed = (float)(Capture_Count - Last_Count) / ENCODER_TOTAL_RESOLUTION * (1000 / (float)t);

    /* 电机牵引距离 */
    float DistancePerUnitTime = (Shaft_Speed / REDUCTION_RATIO) * D * pi * ((float)t / 1000); /* 单位时间牵引距离 = (输出轴转速 * 电机牵引轮直径 * π) * 单位时间 */
    TotalDistance += DistancePerUnitTime; // 电机牵引距离
    Get_Weight(); //获取重量

//    printf("电机方向：%d\r\n", Motor_Direction);
//    printf("单位时间内有效计数值：%d\r\n", Capture_Count - Last_Count); /* 单位时间计数值 = 当前时刻总计数值 - 上一时刻总计数值 */
//    printf("电机转轴处转速：%.2f 转/秒 \r\n", Shaft_Speed);
//    printf("电机输出轴转速：%.2f 转/秒 \r\n", Shaft_Speed / REDUCTION_RATIO); /* 输出轴转速 = 转轴转速 / 减速比 */
//    printf("电机牵引线速度：%.2f 米/秒 \r\n", (Shaft_Speed / REDUCTION_RATIO) * D * pi); /* 输出轴转速 * 电机牵引轮直径 * pi */
//    printf("电机牵引距离：%.2f 米\r\n", TotalDistance);//电机牵引距离 = 单位时间牵引距离 * 单位时间 t（s）
//    printf("毛皮重量：%d\r\n",Weight_Maopi);
//    printf("拉力传感器数值: %d \n", Weight_Shiwu);
    printf("%d\n", Weight_Shiwu);
    /* 记录当前总计数值，供下一时刻计算使用 */
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

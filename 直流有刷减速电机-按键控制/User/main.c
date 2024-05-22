///**
//  ******************************************************************************
//  * @file    main.c
//  * @author  fire
//  * @version V1.0
//  * @date    2020-xx-xx
//  * @brief   直流有刷减速电机-按键控制
//  ******************************************************************************
//  * @attention
//  *
//  * 实验平台:野火 F103-拂晓 STM32 开发板 
//  * 论坛    :http://www.firebbs.cn
//  * 淘宝    :https://fire-stm32.taobao.com
//  *
//  ******************************************************************************
//  */ 

///* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include <stdlib.h>
//#include ".\motor_control\bsp_motor_control.h"
//#include "./led/bsp_led.h"
//#include "./key/bsp_key.h" 
//#include "./usart/bsp_debug_usart.h"

///**
//  * @brief  主函数
//  * @param  无
//  * @retval 无
//  */
//int main(void) 
//{
//  __IO uint16_t ChannelPulse = PWM_MAX_PERIOD_COUNT/2;
//  uint8_t i = 0;
//  uint8_t enable = 0;
//  
//	/* 初始化系统时钟为72MHz */
//	SystemClock_Config();

//	/* 开启复用寄存器时钟 */
//	__HAL_RCC_SYSCFG_CLK_ENABLE();
//	
//	/* 初始化按键GPIO */
//	Key_GPIO_Config();

//  /* 电机初始化 */
//  motor_init();
//  
//  set_motor_disable();
//	set_motor_speed(ChannelPulse);
//	
//	while(1)
//	{
//    /* 扫描KEY1 */
//    if( Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
//    {
//      if (enable == 0)
//      {
//        /* 使能电机 */
//        set_motor_enable();
//      }
//      else
//      {
//        /* 禁用电机 */
//        set_motor_disable();
//      }
//      
//      enable = !enable;
//    }
//    
//    /* 扫描KEY2 */
//    if( Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
//    {
//      /* 增大占空比 */
//      ChannelPulse += PWM_MAX_PERIOD_COUNT/10;
//      
//      if(ChannelPulse > PWM_MAX_PERIOD_COUNT)
//        ChannelPulse = PWM_MAX_PERIOD_COUNT;
//      
//      set_motor_speed(ChannelPulse);
//    }
//    
//    /* 扫描KEY3 */
//    if( Key_Scan(KEY3_GPIO_PORT, KEY3_PIN) == KEY_ON)
//    {
//      if(ChannelPulse < PWM_MAX_PERIOD_COUNT/10)
//        ChannelPulse = 0;
//      else
//        ChannelPulse -= PWM_MAX_PERIOD_COUNT/10;
//      
//      set_motor_speed(ChannelPulse);
//    }
//    
//    /* 扫描KEY4 */
//    if( Key_Scan(KEY4_GPIO_PORT, KEY4_PIN) == KEY_ON)
//    {
//      /* 转换方向 */
//      set_motor_direction( (++i % 2) ? MOTOR_FWD : MOTOR_REV);
//    }
//	}
//}


///**
//  * @brief  System Clock Configuration
//  *         The system Clock is configured as follow : 
//  *            System Clock source            = PLL (HSE)
//  *            SYSCLK(Hz)                     = 72000000
//  *            HCLK(Hz)                       = 72000000
//  *            AHB Prescaler                  = 1
//  *            APB1 Prescaler                 = 2
//  *            APB2 Prescaler                 = 1
//  *            HSE Frequency(Hz)              = 8000000
//  *            HSE PREDIV1                    = 2
//  *            PLLMUL                         = 9
//  *            Flash Latency(WS)              = 0
//  * @param  None
//  * @retval None
//  */
//void SystemClock_Config(void)
//{
//  RCC_ClkInitTypeDef clkinitstruct = {0};
//  RCC_OscInitTypeDef oscinitstruct = {0};
//  
//  /* Enable HSE Oscillator and activate PLL with HSE as source */
//  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
//  oscinitstruct.HSEState        = RCC_HSE_ON;
//  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
//  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
//  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
//  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
//  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
//  {
//    /* Initialization Error */
//    while(1); 
//  }

//  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
//     clocks dividers */
//  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
//  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
//  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
//  {
//    /* Initialization Error */
//    while(1); 
//  }
//}

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "tim.h"
#include "bsp_debug_usart.h"
//#include "gpio.h"
 
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "hx711.h"
/* USER CODE END Includes */
 
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
 
long First_weight=0,weight=0;
 
/* USER CODE END PTD */
 
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int fputc(int ch,FILE *f)
{
HAL_UART_Transmit(&UartHandle,(uint8_t *)&ch,1,0xff);
return (ch);
}
 
 
/* USER CODE END PD */
 
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
 
/* USER CODE END PM */
 
/* Private variables ---------------------------------------------------------*/
 
/* USER CODE BEGIN PV */
 
/* USER CODE END PV */
 
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
 
/* USER CODE END PFP */
 
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
 
/* USER CODE END 0 */
 
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 
  /* USER CODE END 1 */
 
  /* MCU Configuration--------------------------------------------------------*/
 
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
 
  /* USER CODE BEGIN Init */
 
  /* USER CODE END Init */
 
  /* Configure the system clock */
  SystemClock_Config();
 
  /* USER CODE BEGIN SysInit */
 
  /* USER CODE END SysInit */
 
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(1000);
//	for(int i=0;i<10;i++)
//	{
		First_weight=Get_Weight();
//	}
    
 
  /* USER CODE END 2 */
 
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  weight=-Get_Weight()+First_weight;
	  printf("%ld\n",weight);
	  HAL_Delay(120);
	
    /* USER CODE END WHILE */
 
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
 
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
 
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
 
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
 
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
 
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	if (htim == (&htim2))//100us//????3.125us
//	{
 
//		
//			
 
 
//	}
 
}
/* USER CODE END 4 */
 
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
 
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
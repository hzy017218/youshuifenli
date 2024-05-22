///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file    usart.h
//  * @brief   This file contains all the function prototypes for
//  *          the usart.c file
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2024 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
///* USER CODE END Header */
///* Define to prevent recursive inclusion -------------------------------------*/
//#ifndef __USART_H__
//#define __USART_H__

//#ifdef __cplusplus
//extern "C" {
//#endif

///* Includes ------------------------------------------------------------------*/
//#include "main.h"

///* USER CODE BEGIN Includes */

///* USER CODE END Includes */

//extern UART_HandleTypeDef huart1;

///* USER CODE BEGIN Private defines */

///* USER CODE END Private defines */

//void MX_USART1_UART_Init(void);

///* USER CODE BEGIN Prototypes */

///* USER CODE END Prototypes */

//#ifdef __cplusplus
//}
//#endif

//#endif /* __USART_H__ */




#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f1xx.h"
#include <stdio.h>

//串口接收缓冲数组大小
#define UART_RX_BUFFER_SIZE 256 

extern unsigned char UART_RxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t receive_cmd;

//串口波特率
#define DEBUG_USART_BAUDRATE                    115200

//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __HAL_RCC_USART1_CLK_ENABLE();

#define DEBUG_USART_RX_GPIO_PORT                GPIOB
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_7

#define DEBUG_USART_TX_GPIO_PORT                GPIOB
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_6

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn

//将串口1复用到PB6\PB7上
#define DEBUG_USART_AF_ENABLE()					      	__HAL_AFIO_REMAP_USART1_ENABLE()

/************************************************************/

void uart_FlushRxBuffer(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
//int fputc(int ch, FILE *f);
int fgetc(FILE *f);
extern UART_HandleTypeDef UartHandle;
#endif /* __USART1_H */


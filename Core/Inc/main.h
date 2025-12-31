/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "rs485.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IO_IN_Pin GPIO_PIN_0
#define IO_IN_GPIO_Port GPIOA
#define IO_OUT_Pin GPIO_PIN_1
#define IO_OUT_GPIO_Port GPIOA
#define m485A_TE_Pin GPIO_PIN_3
#define m485A_TE_GPIO_Port GPIOA
#define m485A_TX_Pin GPIO_PIN_4
#define m485A_TX_GPIO_Port GPIOA
#define m485A_RX_Pin GPIO_PIN_5
#define m485A_RX_GPIO_Port GPIOA
#define DTS6012M_EN_Pin GPIO_PIN_6
#define DTS6012M_EN_GPIO_Port GPIOA
#define mLED_Pin GPIO_PIN_1
#define mLED_GPIO_Port GPIOB
#define DTS6012M_INT_Pin GPIO_PIN_10
#define DTS6012M_INT_GPIO_Port GPIOA
#define DTS6012M_TX_Pin GPIO_PIN_6
#define DTS6012M_TX_GPIO_Port GPIOB
#define DTS6012M_RX_Pin GPIO_PIN_7
#define DTS6012M_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RX_BUF_SIZE 30

extern uint8_t rxBuf[RX_BUF_SIZE];
extern uint8_t dataReceived; 
extern uint16_t receivedLength;
extern uint32_t detectTime;
extern uint8_t u100msTaskFlag;

extern uint8_t isToolingTest;
extern uint8_t getIN_IO;
extern uint8_t resultUp[15];
int8_t checkSequenceMatch(const unsigned char received[36]);
void ToolingTest();
void objectDetect();
void normalWork();
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

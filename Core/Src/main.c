/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "dts6012m.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
DTS6012_DATA dts6012_data[] = {0, 0, 0, 0};

uint8_t rxBuffDTS[30];
uint16_t rxIndex = 0;
uint8_t rxData[23];
uint8_t headerIsFoundFlag = 0;
uint8_t dataIsReceived = 0;
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
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_DeInit(&huart1);
	MX_GPIO_DTS6012_Init();
	HAL_GPIO_WritePin(DTS6012M_EN_GPIO_Port, DTS6012M_EN_Pin, 0); 
	HAL_Delay(1500);
	HAL_GPIO_WritePin(DTS6012M_EN_GPIO_Port, DTS6012M_EN_Pin, 1); 
	HAL_Delay(1500);                                            

	MX_USART1_UART_Init();
	HAL_Delay(100);
	
	dts6012_start();
	HAL_Delay(50);
	dts6012_start();
	HAL_Delay(50);
	dts6012_start();
		
		
	HAL_UART_Receive_IT(&huart1, &rxBuffDTS[rxIndex], 1);				
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void ProcessData(uint8_t *data, uint16_t length)
{
	if((data[0]==0xA5)&&(data[1]==0x03)&&(data[2]==0x20))
	{
		dts6012_data->firstPeakDistance	= (data[14]<<8) | (data[13]);
		dts6012_data->firstPeakAmp		= (data[18]<<8) | (data[17]);
		dts6012_data->secondPeakDistance	= (data[8]<<8) | (data[7]);
		dts6012_data->secondPeakAmp		= (data[12]<<8) | (data[11]);		
		
//		printf("dts6012_data.firstPeakDistance = %d\t",dts6012_data->firstPeakDistance);
//		printf("dts6012_data.firstPeakAmp = %d\r\n",dts6012_data->firstPeakAmp);
//		printf("dts6012_data.secondPeakDistance = %d\t",dts6012_data->secondPeakDistance);
//		printf("dts6012_data.secondPeakAmp = %d\r\n",dts6012_data->secondPeakAmp);
	}
	
}

uint8_t RxTest[100] ;
uint8_t index = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{   
    /* 判断是哪个串口触发的中断 */
    if (huart->Instance == USART1) 
		{
			if(index <= 99)
			{
				RxTest[index++] = rxBuffDTS[rxIndex];
			}
			else
			{
				index = 0;
			}
			
			if (rxIndex == 0 && rxBuffDTS[0] == 0XA5) 
			{
					headerIsFoundFlag = 1;
					rxIndex++;       
			}
			
			else if (headerIsFoundFlag == 1) 
			{
				rxIndex++;

				if (rxIndex >= 23) 
				{
						dataIsReceived = 1;
						ProcessData(rxBuffDTS, 23);
						dataIsReceived  = 0;
						rxIndex = 0;
						headerIsFoundFlag = 0;
				}
			}			
			else if (rxBuffDTS[rxIndex] == 0xA5)
			{
					headerIsFoundFlag = 1;
					rxIndex = 1;
			}	
			else
			{
					rxIndex = 0;
			}

			HAL_UART_Receive_IT(&huart1, &rxBuffDTS[rxIndex], 1);

		}
		if (huart->Instance == USART6) 
		{
			
		}
			
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

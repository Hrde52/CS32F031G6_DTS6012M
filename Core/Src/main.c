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
#include "dma.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rs485.h"
#include "sensorParaTable.h"
#include "sensorAppLogic.h"
#include "toolingtest.h"
#include <string.h>
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
volatile uint8_t u1msTaskFlag = 0;  // 1ms
volatile uint16_t uw10msCounter = 0; // 10ms
volatile uint8_t u10msTaskFlag = 0;  // 10ms
volatile uint16_t uw300msCounter = 0; // 300ms
volatile uint8_t u300msTaskFlag = 0;  // 300ms
volatile uint16_t uw1sCounter = 0; // 10ms
volatile uint8_t u1sTaskFlag = 0;    // 1s

uint8_t rxBuf[RX_BUF_SIZE];
uint8_t dataReceived = 0; 
uint16_t receivedLength = 0;

uint32_t detectTime = 0;
uint8_t lastDetectStatus = 0;

uint8_t cargoLift_IN = 0;
uint8_t isToolingTest = 1;
uint8_t toolingHandShake[36];
ToolingBits toolingBits;
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
	MX_DMA_Init();
  MX_RTC_Init();
	MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim14);
	
	paraTable_Init();
	
	HAL_UART_DeInit(&huart1);
	MX_GPIO_DTS6012_Init();
	HAL_GPIO_WritePin(DTS6012M_EN_GPIO_Port, DTS6012M_EN_Pin, 0); 
	HAL_Delay(500);
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(1000);
	HAL_IWDG_Refresh(&hiwdg);
	HAL_GPIO_WritePin(DTS6012M_EN_GPIO_Port, DTS6012M_EN_Pin, 1); 
	HAL_Delay(500);
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(1000);
	HAL_IWDG_Refresh(&hiwdg);                                          
	MX_USART1_UART_Init();
	HAL_Delay(100);
	dts6012_start();
	HAL_Delay(50);
	dts6012_start();
	HAL_Delay(50);
	dts6012_start();
	HAL_Delay(50);
	dts6012_start();
	
	memset(rxBuf, 0, RX_BUF_SIZE);
	HAL_UART_Receive_DMA(&huart1, rxBuf, RX_BUF_SIZE);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	
	
	MX_USART6_UART_Init();   // 115200
	   
	HAL_GPIO_WritePin(m485A_TE_GPIO_Port, m485A_TE_Pin, GPIO_PIN_RESET);
  HAL_StatusTypeDef handShankeResult = HAL_UART_Receive(&huart6, toolingHandShake, 36, 400);

  if (handShankeResult == HAL_OK)
  {
    if (checkSequenceMatch(toolingHandShake) == 0)
    {
      isToolingTest = 0;
    }
		else
		{
			toolingBits.bits.bit0RS485 = 0;
		}
  }
  else
  {
    isToolingTest = 0;
  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		
		if (isToolingTest == 1)
    {
      ToolingTest();
    }
    else
    {
      normalWork();
    }

		/* 1s */
		if (u1sTaskFlag == 1)
		{
			u1sTaskFlag = 0;    
			HAL_IWDG_Refresh(&hiwdg);
		}
		
		
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM14) 
  {
    u1msTaskFlag = 1;          // 1ms
    uw10msCounter++;           // 1ms
		//uw300msCounter++;
		uw1sCounter++; 
		
		if (uw10msCounter >= 10)  // 10ms
    {
      u10msTaskFlag = 1;         // 10ms
			uw10msCounter = 0;
    }
		
    if (uw1sCounter >= 1000)  // 100*10ms=1s
    {
      u1sTaskFlag = 1;         // 1s
      uw1sCounter = 0;       // 10ms
    }
  }
}


int8_t checkSequenceMatch(const uint8_t received[36])
{
  const uint8_t target[12] = {0XA5, 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB};

  for (int8_t start_recv = 0; start_recv <= 24; start_recv++)
  {
    int8_t match_length = 0;
    int8_t i = 0;

    while (start_recv + i < 36 && i < 12 && received[start_recv + i] == target[i])
    {
      match_length++;
      i++;
    }

    if (match_length >= 12)
    {
      return 1;
    }
  }

  return 0;
}


uint8_t getIN_IO = 0;
uint8_t resultUp[15] =
    {0x5A, 0X1F, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0};
uint8_t resDTS = 0;
		 
/*
	0:485
	1:DTS6012
	2:OUT
	3: IN
	4-7: space
*/
uint8_t toolingStatusR = 0;
void ToolingTest()
{
  HAL_IWDG_Refresh(&hiwdg);
  LEDON;
  testIO();
	testInX();
	resDTS = testDTS();
	toolingBits.bits.bit1UART = (resDTS == 1) ? 0 : 1;
	toolingBits.bits.bit3IN = (getIN_IO == 1) ? 0 : 1;
	
	resultUp[2] = toolingBits.toolingRaw;
	resultUp[3] = toolingBits.toolingRaw;
	resultUp[8] = (PARA_TABLE_USE.data.programVerison >> 0) & 0xFF;
	resultUp[9] = (PARA_TABLE_USE.data.programVerison >> 8) & 0xFF;

	resultUp[14] = xor_checkSum(resultUp, 12);
	
	HAL_Delay(50);
	
	HAL_GPIO_WritePin(m485A_TE_GPIO_Port, m485A_TE_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart6, resultUp, 15, 5);
	HAL_GPIO_WritePin(m485A_TE_GPIO_Port, m485A_TE_Pin, GPIO_PIN_RESET);
  // Ensure output and LED remain asserted long enough for operator to notice
  // Keep the output asserted and LED on for >1s (1.5s here)
  HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_RESET); // assert output (active low)
  LEDON;
  HAL_Delay(1500);
	isToolingTest = 0;
	//LEDOFF;
	toolingStatusR = PARA_TABLE_USE.data.passToolingStatus;
	paraTable11_Reset();

	if (toolingStatusR == 0x00)
	{
			PARA_TABLE_USE.data.passToolingStatus = 0x01;
			paraTable_Write();
	}
	else if (toolingStatusR == 0x01)
	{
			PARA_TABLE_USE.data.passToolingStatus = 0x02;
			paraTable_Write();
	}

	HAL_NVIC_SystemReset();
    
	
  while (1)
  {
    if (u10msTaskFlag == 1)
    {
      u10msTaskFlag = 0;
			
      testInX();
    }

    if (u1sTaskFlag == 1)
		{
			u1sTaskFlag = 0;    
			
			dts6012_start();
			HAL_IWDG_Refresh(&hiwdg);
		}
  }
	
  LEDOFF;
}

void normalWork()
{
	LEDOFF;
	HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(m485A_TE_GPIO_Port, m485A_TE_Pin, 1);
	HAL_GPIO_WritePin(m485A_TE_GPIO_Port, m485A_TE_Pin, 0);
	HAL_Delay(20);  
	HAL_UART_Receive_IT(&huart6, &rxBuffPDA[rxPDAIndex], 1);
	
	//paraTable_Init();
	
	uint8_t reverse = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		
		if (u10msTaskFlag == 1)
		{
			u10msTaskFlag = 0;  
		}

		/* 1s */
		if (u1sTaskFlag == 1)
		{
			u1sTaskFlag = 0;    
			
			dts6012_start();
			HAL_IWDG_Refresh(&hiwdg);
		}
		
		objectDetect();
	
		lastDetectStatus = dts6012_data.objDetectFlag;
    /* USER CODE BEGIN 3 */
  }
}

void objectDetect()
{
	uint16_t detectValue = PARA_TABLE_USE.data.dts6012StudyDistance - PARA_TABLE_USE.data.dts6012DistanceChkThreshold;
	if(dts6012_data.firstPeakDistance > 0 || dts6012_data.secondPeakDistance > 0)
	{
		if ((dts6012_data.firstPeakDistance < detectValue ) || (dts6012_data.secondPeakDistance < detectValue))
		{
			dts6012_data.objDetectFlag = 1;
			ObjectIsDetectedFlag = 1;
			IO_dts6012 = 1;
			LEDON;
			HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_RESET); // IO
		}
		else 
		{
			dts6012_data.objDetectFlag = 0;
			ObjectIsDetectedFlag = 0;
			IO_dts6012 = 0;
			LEDOFF;
			HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_SET); // IO
		}
	}
		return;
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

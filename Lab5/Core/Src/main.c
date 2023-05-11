/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"

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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t RxBuffer[2];
uint8_t TxBuffer[400];
uint velocity =0;
int state = 4;
int status = 0;
int press =0;
int last =2;
int current = 2;
int f=4;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void ControlLED();
void UARTInterruptConfig();
void ON();
void OFF();
void Button();

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(1000);
  UARTInterruptConfig();
  uint8_t text[] = "\r\nPlease Select Menu :\n \r\n 0 for  LED Control\n \r\n 1 for Button Status\n";
  HAL_UART_Transmit(&huart2, text, 69, 10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //Task();
	  velocity = 1000/(f*2);

	  if(state ==0 && press == 0 && RxBuffer[0]=='x'){

		  sprintf((char*)TxBuffer,"\r\n Back : %s\r\n\n Please Select Menu :\r\n 0 for  LED Control \r\n 1 for Button Status\n",RxBuffer);
		  HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
		  press = 1;
	  }
	  else if(state ==1 && press == 0 && RxBuffer[0]=='0'){
		  sprintf((char*)TxBuffer,"\r\n LED Control : %s\r\n\n Select Functions \r\n a : SpeedUp+1Hz \r\n s : SpeedDown-1Hz \r\n d : On/Off \r\n x : Back\n",RxBuffer);
		  HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
		  press = 1;


	  }
	  else if(state ==2 && press == 0 && RxBuffer[0]=='1'){
		   sprintf((char*)TxBuffer,"\r\n Button Status : %s\r\n\n Select Functions\r\n StatusButton \r\n x:Back\n",RxBuffer);
		  HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
		  press = 1;
	  }
	  else if(state ==3 && press == 0){
		  sprintf((char*)TxBuffer,"\r\n No Menu : %s\r\n \n Please Select Menu :\r\n 0 for  LED Control \r\n 1 for Button Status\n",RxBuffer);
		  HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
		  press = 1;
	  }
	  if(status == 1){
		  ON();
	  }
	  else if (status == 0){
		  OFF();
	  }
	  Button();




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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 921600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void UARTInterruptConfig()
{
	HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		RxBuffer[1] = '\0';
		press = 0;
		if (RxBuffer[0] == '0')
		{
			state =1;

		}
		else if (RxBuffer[0] == '1')
		{
			state =2;
		}
		else if (state == 4 && RxBuffer[0] != '0' && RxBuffer[0] != '1')
		{
			state = 3;
		}


		switch (state)
		{
		case 0 :
			if (state==0 && RxBuffer[0] == '0')
			{
				state =1;
			}
			else if (state==0 &&RxBuffer[0] == '1')
			{
				state =2;
			}
			else if (state == 0 && RxBuffer[0]!='0'&& RxBuffer[0]!='1'&& RxBuffer[0]!='a'&& RxBuffer[0]!='d'&& RxBuffer[0]!='s'&& RxBuffer[0]!='x')
			{
				state = 3;
			}
			//HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
			break;

			// LED Control
		case 1 :

			if(state==1 && RxBuffer[0]=='a')
			{
				sprintf((char*)TxBuffer,"\r\n SpeedUp 1Hz : %s\r\n",RxBuffer);
				HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
				f+=1;
				status = 1;
			}
			else if(state==1 && RxBuffer[0]=='s')
			{
				sprintf((char*)TxBuffer,"\r\n SpeedDown 1Hz : %s\r\n",RxBuffer);
				HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
				f-=1;

				status = 1;
			}
			else if(state==1 && RxBuffer[0]=='d')
			{
				status += 1;
				if(status == 1){
					sprintf((char*)TxBuffer,"\r\n On : %s\r\n",RxBuffer);
					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));

				}
				else if(status == 2){
					sprintf((char*)TxBuffer,"\r\n Off : %s\r\n",RxBuffer);
					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
					status = 0;
				}


			}
			else if(state==1 && RxBuffer[0]=='x')
			{

				state = 0;
			}
			else if (state == 1 && RxBuffer[0]!='0'&& RxBuffer[0]!='1'&& RxBuffer[0]!='a'&& RxBuffer[0]!='d'&& RxBuffer[0]!='s'&& RxBuffer[0]!='x')
			{
				state = 3;
			}
			break;

			//Button Status
		case 2:

			if(RxBuffer[0]=='x')
			{

				state = 0;
			}
			else if (state == 2 && RxBuffer[0]!='0'&& RxBuffer[0]!='1'&& RxBuffer[0]!='a'&& RxBuffer[0]!='d'&& RxBuffer[0]!='s'&& RxBuffer[0]!='x')
			{
				state = 3;
			}
			break;

		case 3 :
			//state = 0;

			if (state==0 && RxBuffer[0] == '0')
			{
				state =1;
			}
			else if (state==0 &&RxBuffer[0] == '1')
			{
				state =2;
			}
			else if (state == 0 &&  RxBuffer[0]!='x')
			{
				state = 3;
			}

			//HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
		break;


		}

		HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
	}
}

void ON()
{
	static uint32_t timestamp = 0;
	if(HAL_GetTick()>= timestamp)
	{
		timestamp = HAL_GetTick()+velocity;
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	}
}
void OFF()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);

}
void Button()
{
	current = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	if(last == 1 && current == 0 && state ==2 ){
		sprintf((char*)TxBuffer,"\r\n Press\r\n",RxBuffer);
		HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
	}
	else if(last == 0 && current == 1 && state == 2){
		sprintf((char*)TxBuffer,"\r\n UnPress\r\n",RxBuffer);
		HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
	}
	last = current;
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

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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint8_t left_pressed = 0;
uint8_t right_pressed = 0;
uint8_t estac_pressed = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == LEFT_BT_Pin){

	  left_pressed = 1;
  }

  if (GPIO_Pin == RIGHT_BT_Pin){

  	  right_pressed = 1;
    }

  if (GPIO_Pin == ESTAC_BT_Pin){

  	  estac_pressed = 1;
    }

}
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
  HAL_UART_Transmit(&huart2, "INICIAR\r\n", 9, HAL_MAX_DELAY);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t left_last_pressed_tick = 0;
  uint32_t left_blinks = 0;
  uint32_t left_tick = 0;

  uint32_t right_last_pressed_tick = 0;
  uint32_t right_blinks = 0;
  uint32_t right_tick = 0;

  uint32_t estac_last_pressed_tick = 0;
  uint32_t estac_blinks = 0;
  uint32_t estac_tick = 0;

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (left_pressed != 0) {
		  left_pressed = 0;
		  HAL_UART_Transmit(&huart2, "IZQUIERDA\r\n", 11, HAL_MAX_DELAY);
		  // 5. Si un botón de giro se presiona 1 vez: la luz del lado correspondiente parpadea 3 veces.
		  left_blinks = 6;
		  if (HAL_GetTick() - left_last_pressed_tick < 300) {
			  // 6. Si un botón de giro se presiona 2 o mas veces durante 300ms: la luz del lado correspondiente parpadea indefinidamente.
			  left_blinks = 0xFFFFFFFF;
			  right_blinks = 0;
			  estac_blinks = 0;
		  }
		  left_last_pressed_tick = HAL_GetTick();
		  // 7. Si un botón de giro se presiona y la luz del otro lado esta activa: se desactiva la luz del otro lado.
		  right_blinks = 0;
		  estac_blinks = 0;
	  }
	  if (right_pressed != 0) {
		  right_pressed = 0;
		  HAL_UART_Transmit(&huart2, "DERECHA\r\n", 9, HAL_MAX_DELAY);
		  // 5. Si un botón de giro se presiona 1 vez: la luz del lado correspondiente parpadea 3 veces.
		  right_blinks = 6;
		  if (HAL_GetTick() - right_last_pressed_tick < 300) {
			  // 6. Si un botón de giro se presiona 2 o mas veces durante 300ms: la luz del lado correspondiente parpadea indefinidamente.
			  right_blinks = 0xFFFFFFFF;
			  left_blinks = 0;
			  estac_blinks = 0;
		  }
		  right_last_pressed_tick = HAL_GetTick();
		  // 7. Si un botón de giro se presiona y la luz del otro lado esta activa: se desactiva la luz del otro lado.
		 left_blinks = 0;
		 estac_blinks = 0;
	  }

	  if (estac_pressed != 0) {
		  estac_pressed = 0;
		  // 4. Debe enviar mensajes de información por la consola cuando hayan eventos en el sistema.
		  HAL_UART_Transmit(&huart2, "ESTACIONARIA\r\n", 12, HAL_MAX_DELAY);
		  // 5. Si un botón de giro se presiona 1 vez: la luz del lado correspondiente parpadea 3 veces.
		  left_blinks = 0xFFFFFFFF;
		  right_blinks = 0xFFFFFFFF;
		  estac_last_pressed_tick = HAL_GetTick();
	  }

	  // 2. Tener 2 luces(LEDs): Luz Izquierda(D3), Luz Derecha(D4).
	  if (left_blinks > 0) {
		  if (left_tick < HAL_GetTick()){
			  HAL_GPIO_TogglePin(LEFT_LED_GPIO_Port, LEFT_LED_Pin);
			  // 8. La frecuencia de parpadeo de las luces debe ser de 2Hz (toggle every 250ms).
			  left_tick = HAL_GetTick() + 250;
			  left_blinks--;
		  }
	  }
	  if (right_blinks > 0) {
		  if (right_tick < HAL_GetTick()){
			  HAL_GPIO_TogglePin(RIGHT_LED_GPIO_Port, RIGHT_LED_Pin);
			  // 8. La frecuencia de parpadeo de las luces debe ser de 2Hz (toggle every 250ms).
			  right_tick = HAL_GetTick() + 250;
			  right_blinks--;
		  }
	  }

	  if (estac_tick < HAL_GetTick()) {
	      HAL_GPIO_TogglePin(ESTAC_LED_GPIO_Port, ESTAC_LED_Pin);
	      // 8. La frecuencia de parpadeo de las luces debe ser de 1Hz (toggle every 500ms).
	      estac_tick = HAL_GetTick() + 500;
	  }

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ESTAC_LED_Pin|LEFT_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RIGHT_LED_GPIO_Port, RIGHT_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LEFT_BT_Pin RIGHT_BT_Pin */
  GPIO_InitStruct.Pin = LEFT_BT_Pin|RIGHT_BT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ESTAC_LED_Pin LEFT_LED_Pin */
  GPIO_InitStruct.Pin = ESTAC_LED_Pin|LEFT_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ESTAC_BT_Pin */
  GPIO_InitStruct.Pin = ESTAC_BT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ESTAC_BT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RIGHT_LED_Pin */
  GPIO_InitStruct.Pin = RIGHT_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RIGHT_LED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

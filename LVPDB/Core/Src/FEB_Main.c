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
#include "FEB_Main.h"
#include "FEB_CAN.h"


#include "stdio.h"
#include "stdbool.h"

extern FEB_CAN_APPS_Message_t FEB_CAN_APPS_Message;


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SLEEP_TIME 10
#define BRAKE_THRE 0.2

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
I2C_HandleTypeDef* hi2c1p;



/*
 * The LVPDB has multiple TPS chips on the bus. These are the addresses of
 * each of the TPS chips. The naming conventions is as follows:
 * 		LV - Low voltage bus (the main one)
 * 		CP - Coolant pump
 * 		AF - Accumulator fans
 * 		EX - extra radiator fans
 */
const uint8_t LV_ADDR = 0b1000000 << 1;
const uint8_t CP_ADDR = 0b1000100 << 1;
const uint8_t AF_ADDR = 0b1000101 << 1;
const uint8_t EX_ADDR = 0b1000001 << 1;

// configuration register value
uint8_t CONFIG[2] = {0b01000001, 0b00100111}; // default settings


/*
 * Values needed to calibrate each of the TPS chips. This value gets passed
 * into the init function called in later
 */

// calibration register values
uint8_t MAIN_CAL[2] = {0b00000110, 0b10001110}; // Imax = 50A
uint8_t CP_CAL[2] = {0b00010111, 0b01101000};
uint8_t AF_CAL[2] = {0b00101000, 0b11110110};
uint8_t EX_CAL[2] = {0b00110110, 0b10011101};

// alert types
uint8_t UNDERV[2] = {0b00010000, 0b00000000};
uint8_t OVERPWR[2] = {0b00001000, 0b00000000};

// limits
uint8_t LV_LIMIT[2] = {0b00000000, 0b00010110}; // = 22
uint8_t CP_LIMIT[2] = {0b00000001, 0b01010000}; // = 336, 14 * 24
uint8_t AF_LIMIT[2] = {0b00000000, 0b11000000}; // = 192, 8 * 24
uint8_t EX_LIMIT[2] = {0b00000000, 0b10010000}; // = 144, 6 * 24

static bool isDriving = false;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CAN1_Init(void);
static void MX_I2C1_Init(void);
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
  MX_USART2_UART_Init();
  MX_CAN1_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */


	FEB_CAN_Init(); //changed

	hi2c1p = &hi2c1;

	// uncomment if we need to pull ENs high to start
	//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);// pull PC11 high to enable coolant pump
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);// pull PB5 high to enable accumulator fans
	//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);// pull PC3 high to enable extra

	FEB_TPS2482_SETUP(hi2c1p, LV_ADDR, CONFIG, MAIN_CAL, UNDERV, LV_LIMIT);
	FEB_TPS2482_SETUP(hi2c1p, CP_ADDR, CONFIG, CP_CAL, OVERPWR, CP_LIMIT);
	FEB_TPS2482_SETUP(hi2c1p, AF_ADDR, CONFIG, AF_CAL, OVERPWR, AF_LIMIT);
	FEB_TPS2482_SETUP(hi2c1p, EX_ADDR, CONFIG, EX_CAL, OVERPWR, EX_LIMIT);

	char buf[128];
	int buf_len;
	float current_reading;
	float ex_current_reading;
	float cp_current_reading;
	float apps_current_reading;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		  // Brake Light
		  if (FEB_CAN_APPS_Message.brake_pedal > BRAKE_THRE) {
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);// PA1 high
		  } else {
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);// PA1 low
		  }

		  // activate peripheral devices if ready to drive
		  if (FEB_SW_Message.ready_to_drive == 1 && !isDriving) {
			  isDriving = true;
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);// pull PC11 high to enable coolant pump
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);// pull PB5 high to enable accumulator fans
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);// pull PC3 high to enable extra

		  // de-activate if not ready to drive
		  } else if (FEB_SW_Message.ready_to_drive == 0 && isDriving) {
			  isDriving = false;
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		  }



		  current_reading = FEB_TPS2482_PollBusCurrent(hi2c1p,LV_ADDR+1);
		  ex_current_reading = FEB_TPS2482_PollBusCurrent(hi2c1p,EX_ADDR+1);
		  cp_current_reading = FEB_TPS2482_PollBusCurrent(hi2c1p,CP_ADDR+1);
		  FEB_CAN_Transmit(&hcan1, LVPDB_LV_CURRENT,&current_reading,sizeof(float));
		  FEB_CAN_Transmit(&hcan1, LVPDB_EX_CURRENT,&ex_current_reading,sizeof(float));
		  FEB_CAN_Transmit(&hcan1, LVPDB_CP_CURRENT,&cp_current_reading,sizeof(float));
		  apps_current_reading = APPS_MESSAGE.current;

//make CAN_current file with these functions, one fnction per unqiue transmit
//file itself should delare hcan1
		  //configure tx header (check struct to see what to configure)
		  //then send message w/



		  buf_len = sprintf((char*)buf, "ready: %d, brake: %.3f\r\n", SW_MESSAGE.ready_to_drive, APPS_MESSAGE.brake_pedal);
		  //HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

		  buf_len = sprintf((char*) buf, "Current Draw (LV, EX, CP, APPS): %.3f, %.3f, %.3f, %.3f\r\n", current_reading, ex_current_reading, cp_current_reading, apps_current_reading);
		  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);


    /* USER CODE END WHILE */

		  HAL_Delay(SLEEP_TIME);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC1 PC2 PC10 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC3 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

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
#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
//#include "FEB_CAN_Library/"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// WSS DEFS
#define TIMER_ELAPSED_HZ 5 // Adjust this based on your timer configuration
#define NUM_SAMPLES 128

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// WSS phase counters
uint32_t ticks0 = 0;
uint32_t ticks1 = 0;

uint8_t wss0 = 0;
uint8_t wss1 = 0;

// TMC DEFS
uint16_t* buf;
uint16_t pkt[2];
uint8_t idx = 0;

// determine which IC to read from
uint8_t tm_state = 0;

uint8_t tm0 = 0;
uint8_t tm1 = 0;

uint16_t ds0 = 0;
uint16_t ds1 = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void transmit_uart(const char *string) {
    HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 1000);
}

void CAN_Transmit()
{
  CAN_TxHeaderTypeDef TxHeader;
  uint8_t TxData[8];
  uint32_t TxMailbox;

  TxHeader.DLC = 8; // Data length
  TxHeader.IDE = CAN_ID_STD; // Standard ID
  TxHeader.RTR = CAN_RTR_DATA; // Data frame
  TxHeader.StdId = 0x545; // CAN ID
  TxHeader.ExtId = 0; // Not used with standard ID

  char send[50];
  sprintf(send, "%d rpm, %f mph \r\n", wss0, (wss0 * 0.06098555871));

  transmit_uart(send);

  // Fill the data
  TxData[0] = wss0;
  TxData[1] = wss1;
  TxData[2] = tm0;
  TxData[3] = tm1;
  TxData[4] = ds0;
  TxData[5] = ds1;
  TxData[6] = 0;
  TxData[7] = 0;

  while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

  if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
  {
    // Transmission request error
	 char msg[50];
	 sprintf(msg, "CAN transmit error");
	 transmit_uart(msg);
	 Error_Handler();
  }

  // Wait until the message is transmitted
   // while (HAL_CAN_IsTxMessagePending(&hcan1, TxMailbox)) {}
}

uint16_t FEB_Read_ADC(uint32_t channel){
	ADC_ChannelConfTypeDef sConfig={0};
	sConfig.Channel = channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
//	   Error_Handler();
	}

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	return (uint16_t)HAL_ADC_GetValue(&hadc1);

}

// WSS transmit data
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)  // verify if the interrupt comes from TIM1
    {
		wss0 = (uint8_t) ((float) ticks0 * TIMER_ELAPSED_HZ / 84.0 * 60.0);
		wss1 = (uint8_t) ((float) ticks1 * TIMER_ELAPSED_HZ / 84.0 * 60.0);

		//char msg[50];
		//sprintf(msg, "%d RPM, %d RPM \r\n", wss0, wss1);

		//transmit_uart(msg);
		// add can TX
		ticks0 = 0;
		ticks1 = 0;

//		 if (HAL_ADC_Start(&hadc1) != HAL_OK) {
//			  // Start Error
//			  return 0;
//
//		 }
//		// do displacement reading
//	    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
//	        // Get the converted values
//	        ds0 = HAL_ADC_GetValue(&hadc1);  // First channel (ADC_IN0)
//	        //ds1 = HAL_ADC_GetValue(&hadc1);  // Second channel (ADC_IN4)
//
//	    }
//
//	    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
//		        // Get the converted values
//		        //ds0 = HAL_ADC_GetValue(&hadc1);  // First channel (ADC_IN0)
//		     xds1 = HAL_ADC_GetValue(&hadc1);  // Second channel (ADC_IN4)
//
//		}
//
//	    HAL_ADC_Stop(&hadc1);
		ds0 = FEB_Read_ADC(ADC_CHANNEL_0);
		ds1 = FEB_Read_ADC(ADC_CHANNEL_4);

		char msg[50];
		sprintf(msg, "%d adc, %d adc \r\n", ds0, ds1);

		transmit_uart(msg);

    }

	// CAN sensor report generation
	if (htim->Instance == TIM2) {
//		CAN_Transmit();
		  char send[50];
		  sprintf(send, "%d rpm, %f mph \r\n", wss1, (wss1 * 0.06098555871));

		  transmit_uart(send);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	char msg[50];
	sprintf(msg, "pin: %i", GPIO_Pin);
    //transmit_uart(msg);

	// WSS COS (or sensor 1)
    if(GPIO_Pin == GPIO_PIN_10)
    {
        // Handle GPIO_PIN_10 interrupt
        ticks0++;
    }

    // WSS SIN (or sensor 2)
    if(GPIO_Pin == GPIO_PIN_12)
	{
		// Handle GPIO_PIN_12 interrupt
		ticks1++;
	}

    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}


// Callback function to handle received data
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI2) {
		pkt[idx] = *buf;
		idx = idx + 1;
        char msg[50];

		if (idx == 2) {
			uint32_t val = (((uint32_t) pkt[1]) << 16) | pkt[0];

			uint8_t fault = val & (1 << 16);

		    // Shift right by 18 bits to align to the right, then mask to get the last 14 bits
		    int16_t rawThermoTemp = (int16_t)((val >> 18) & 0x3FFF); // 0x3FFF is the mask for 14 bits

		    // Check if thermocouple temperature data is negative
		    if (rawThermoTemp & 0x2000) { // 0x2000 is the mask for the sign bit in the 14-bit temperature
		        rawThermoTemp |= 0xC000; // Sign extend to 16 bits for negative numbers
		    }

		    // Convert the 14-bit thermocouple data to temperature (assuming 0.25°C resolution)
		    float thermocoupleTemp = rawThermoTemp * 0.25;

		    // Extract the 12-bit internal junction temperature data
		    // Bits D15-D4 are the junction temperature
		    // Shift right by 4 bits to align to the right, then mask to get the last 12 bits
		    int16_t rawJunctionTemp = (int16_t)((val >> 4) & 0x0FFF); // 0x0FFF is the mask for 12 bits

		    // Check if junction temperature data is negative
		    if (rawJunctionTemp & 0x0800) { // 0x0800 is the mask for the sign bit in the 12-bit temperature
		        rawJunctionTemp |= 0xF000; // Sign extend to 16 bits for negative numbers
		    }

		    // Convert the 12-bit junction data to temperature (assuming 0.0625°C resolution)
		    float junctionTemp = rawJunctionTemp * 0.0625;

		    if (fault == 1) {
		    	sprintf(msg, "thermocouple fault detected!");
		    } else {
				sprintf(msg, "(TM %i) thermocouple (internal): %0.2f (%0.2f) \r\n", tm_state, thermocoupleTemp, junctionTemp);

		    }

			//transmit_uart(msg);

			if (tm_state == 0) {
				// disable TC0S (PB6) and enable TC1S (PB8) (active low)
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
				tm0 = (uint8_t) thermocoupleTemp;

				tm_state = 1;
			} else if (tm_state == 1) {
				// enable TC0S (PB6) and disable TC1S (PB8) (active low)
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
				tm1 = (uint8_t) thermocoupleTemp;

				tm_state = 0;
			}

			idx = 0;
		}

        HAL_SPI_Receive_IT(&hspi2, buf, 1);
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
	MX_USART2_UART_Init();

	// clear serial console
	transmit_uart("\033[2J");

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	transmit_uart("FEB Sensor Node peripherals initialized\r\n");
	if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK) {
		// Handling of potential error
		char msg[50];
		sprintf(msg, "TIM1 fault");
		transmit_uart(msg);
	}

	if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK) {
		// Handling of potential error
		char msg[50];
		sprintf(msg, "TIM2 fault");
		transmit_uart(msg);
	}

	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
        // Code Error - Shutdown
	}

	// FEB_CAN_Init(&hcan1, 0);

	buf = (uint16_t*) malloc(sizeof(uint16_t));

	// enable TC0S (PB6) and disable TC1S (PB8)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	HAL_SPI_Receive_IT(&hspi2, buf, 1);


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
  RCC_OscInitStruct.PLL.PLLM = 8;
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

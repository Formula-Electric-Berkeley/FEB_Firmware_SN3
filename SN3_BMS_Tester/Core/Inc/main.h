/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FEB_Main.h"
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
#define A0_Pin GPIO_PIN_0
#define A0_GPIO_Port GPIOA
#define AUX_EN_Pin GPIO_PIN_6
#define AUX_EN_GPIO_Port GPIOA
#define CS_11_Pin GPIO_PIN_4
#define CS_11_GPIO_Port GPIOC
#define CS_10_Pin GPIO_PIN_5
#define CS_10_GPIO_Port GPIOC
#define CS_3_Pin GPIO_PIN_0
#define CS_3_GPIO_Port GPIOB
#define CS_4_Pin GPIO_PIN_1
#define CS_4_GPIO_Port GPIOB
#define CS_5_Pin GPIO_PIN_2
#define CS_5_GPIO_Port GPIOB
#define CS_2_Pin GPIO_PIN_12
#define CS_2_GPIO_Port GPIOB
#define DB_EN_Pin GPIO_PIN_7
#define DB_EN_GPIO_Port GPIOC
#define A2_Pin GPIO_PIN_9
#define A2_GPIO_Port GPIOC
#define A1_Pin GPIO_PIN_8
#define A1_GPIO_Port GPIOA
#define CS_6_Pin GPIO_PIN_9
#define CS_6_GPIO_Port GPIOA
#define CS_7_Pin GPIO_PIN_10
#define CS_7_GPIO_Port GPIOA
#define CS_8_Pin GPIO_PIN_11
#define CS_8_GPIO_Port GPIOA
#define CS_9_Pin GPIO_PIN_12
#define CS_9_GPIO_Port GPIOA
#define Start_Pin GPIO_PIN_11
#define Start_GPIO_Port GPIOC
#define A3_Pin GPIO_PIN_3
#define A3_GPIO_Port GPIOB
#define CS_1_Pin GPIO_PIN_4
#define CS_1_GPIO_Port GPIOB
#define TEMP_SCL_Pin GPIO_PIN_8
#define TEMP_SCL_GPIO_Port GPIOB
#define TEMP_SDA_Pin GPIO_PIN_9
#define TEMP_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
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
#define Out_1_Pin GPIO_PIN_0
#define Out_1_GPIO_Port GPIOA
#define Out_2_Pin GPIO_PIN_1
#define Out_2_GPIO_Port GPIOA
#define Out_3_Pin GPIO_PIN_4
#define Out_3_GPIO_Port GPIOA
#define AUX_EN_Pin GPIO_PIN_6
#define AUX_EN_GPIO_Port GPIOA
#define SPI2_CS3_Pin GPIO_PIN_0
#define SPI2_CS3_GPIO_Port GPIOB
#define SPI2_CS4_Pin GPIO_PIN_1
#define SPI2_CS4_GPIO_Port GPIOB
#define SPI2_CS5_Pin GPIO_PIN_2
#define SPI2_CS5_GPIO_Port GPIOB
#define SPI2_CS2_Pin GPIO_PIN_12
#define SPI2_CS2_GPIO_Port GPIOB
#define DB_EN_Pin GPIO_PIN_7
#define DB_EN_GPIO_Port GPIOC
#define SEL2_Pin GPIO_PIN_9
#define SEL2_GPIO_Port GPIOC
#define SEL1_Pin GPIO_PIN_8
#define SEL1_GPIO_Port GPIOA
#define SPI2_CS6_Pin GPIO_PIN_9
#define SPI2_CS6_GPIO_Port GPIOA
#define SPI2_CS7_Pin GPIO_PIN_10
#define SPI2_CS7_GPIO_Port GPIOA
#define SPI2_CS8_Pin GPIO_PIN_11
#define SPI2_CS8_GPIO_Port GPIOA
#define SPI2_CS9_Pin GPIO_PIN_12
#define SPI2_CS9_GPIO_Port GPIOA
#define SPI2_CS10_Pin GPIO_PIN_13
#define SPI2_CS10_GPIO_Port GPIOA
#define Start_Pin GPIO_PIN_11
#define Start_GPIO_Port GPIOC
#define SPI2_CS1_Pin GPIO_PIN_4
#define SPI2_CS1_GPIO_Port GPIOB
#define SEL3_Pin GPIO_PIN_5
#define SEL3_GPIO_Port GPIOB
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

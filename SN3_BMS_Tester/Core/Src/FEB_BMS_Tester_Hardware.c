/*
 * FEB_BMS_Tester_Hardware.c
 *
 *  Created on: Jan 21, 2024
 *      Author: andyyang
 */

// ********************************** Includes & External **********************************

#include "FEB_BMS_Tester_Hardware.h"

extern SPI_HandleTypeDef hspi2;

// ********************************** Functions **********************************

void FEB_BMS_Tester_Hardware_Configure_MUX(uint8_t cell) {
	uint8_t select0 = (cell & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	uint8_t select1 = ((cell >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	uint8_t select2 = ((cell >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	uint8_t select3 = ((cell >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, select0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, select1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, select2);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, select3);
}

void FEB_BMS_Tester_Hardware_Set_DAC_CS_n(uint8_t cell, uint8_t status) {
	switch (cell) {
		case 0:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //CS1 High
			} else {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); //CS1 Low

			}
			break;
		case 1:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); //CS2 High
			} else {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); //CS2 Low

			}
		case 2:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); //CS3 High
			} else {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //CS3 Low

			}
		case 3:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //CS4 High
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); //CS4 Low

			}
		case 4:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); //CS5 High
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); //CS5 Low

			}
		case 5:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); //CS6 High
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); //CS6 Low

			}
		case 6:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //CS7 High
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //CS7 Low

			}
		case 7:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); //CS8 High
			} else {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); //CS8 Low

			}
		case 8:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); //CS9 High
			} else {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); //CS9 Low

			}
		case 9:
			if (status == 1) {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //CS10 High
			} else {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //CS10 Low

			}
	}
}

bool FEB_BMS_Tester_Hardware_Read_Start() {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET) {
		return true;
	} else {
		return false;
	}
}

bool FEB_BMS_Tester_Hardware_Read_Mode() {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_SET) {
		return true; //daughter
	} else {
		return false; //aux
	}
}

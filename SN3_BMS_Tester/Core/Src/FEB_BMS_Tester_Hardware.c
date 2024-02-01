/*
 * FEB_BMS_Tester_Hardware.c
 *
 *  Created on: Jan 21, 2024
 *      Author: andyyang
 */

// ********************************** Includes & External **********************************

#include "FEB_BMS_Tester_Hardware.h"

extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart2;

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

void FEB_BMS_Tester_Hardware_Set_DAC_CS_n(uint8_t cell, bool status) {

	static cs_pins pins[] = {
			{GPIOA, GPIO_PIN_7}, //CS 1
			{GPIOC, GPIO_PIN_4}, //CS 2
			{GPIOC, GPIO_PIN_5}, //CS 3
			{GPIOB, GPIO_PIN_0}, //CS 4
			{GPIOB, GPIO_PIN_1}, //CS 5
			{GPIOB, GPIO_PIN_2}, //CS 6
			{GPIOB, GPIO_PIN_12}, //CS 7
			{GPIOA, GPIO_PIN_9}, //CS 8
			{GPIOA, GPIO_PIN_10}, //CS 9
			{GPIOA, GPIO_PIN_11}, //CS 10
	};

	if (status) {
		HAL_GPIO_WritePin(pins[cell].pin_type, pins[cell].pin_num, GPIO_PIN_SET); //CS high
	} else {
		HAL_GPIO_WritePin(pins[cell].pin_type, pins[cell].pin_num, GPIO_PIN_RESET); //CS low
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

void FEB_BMS_Tester_Hardware_Transmit_Start_Testing(char *board, char *mode) {
	char msg[256];
	sprintf(msg, "Starting to test cell %s %s", board, mode);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 100);
}

void FEB_BMS_Tester_Hardware_Transmit_Done_Testing(char *board, char *mode) {
	char msg[256];
	sprintf(msg, "Starting to test cell %s %s", board, mode);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 100);
}

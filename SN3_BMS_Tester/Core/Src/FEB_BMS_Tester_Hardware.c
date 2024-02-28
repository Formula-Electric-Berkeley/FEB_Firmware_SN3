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

/**
 * Configure Temperature testing MUX based on specified cell. There are 10 cells to test,
 * and 4 selects, 2^4 = 16, so there are enough combinations.
 */
void FEB_BMS_Tester_Hardware_Configure_MUX(uint8_t cell) {
	GPIO_PinState select0 = (cell & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	GPIO_PinState select1 = ((cell >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	GPIO_PinState select2 = ((cell >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	GPIO_PinState select3 = ((cell >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, select0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, select1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, select2);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, select3);
}

/*
 * Function to set the status of a CS pin corresponding to specified cell.
 */
void FEB_BMS_Tester_Hardware_Set_DAC_CS_n(uint8_t cell, bool status) {

	//Struct representing all pins. pins[cell] returns CS pin corresponding to cell.
	static cs_pins_t pins[] = {
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

/*
 * Function to read if the start button has been pressed.
 */
bool FEB_BMS_Tester_Hardware_Read_Start() {

	static GPIO_PinState previousStatus = GPIO_PIN_RESET;
	GPIO_PinState currentStatus = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
	bool start = false;

	if (currentStatus == GPIO_PIN_SET && previousStatus == GPIO_PIN_RESET) {
		start = FEB_CONSTANT_START; //start
	} else {
		start = FEB_CONSTANT_NOT_START; //not start
	}
	previousStatus = currentStatus;
	return start;
}

/*
 * Function to read which mode (aux testing, daughter testing) has been specified by user
 */
bool FEB_BMS_Tester_Hardware_Read_Mode() {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_SET) {
		return FEB_CONSTANT_DAUGHTER_MODE; //daughter
	} else {
		return FEB_CONSTANT_AUX_MODE; //aux
	}
}

/*
 * Log to user that BMS tester has started testing
 */
void FEB_BMS_Tester_Hardware_Transmit_Start_Testing(char *board, char *mode) {
	char msg[256];
	sprintf(msg, "Starting to test %s %s\n", board, mode);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 100);
}

/*
 * Log to user that BMS tester has finished testing
 */
void FEB_BMS_Tester_Hardware_Transmit_Done_Testing(char *board, char *mode) {
	char msg[256];
	sprintf(msg, "Done testing %s %s\n", board, mode);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 100);
}

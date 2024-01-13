/*
 * FEB_Validate_Readings.c
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */


// ******************************** Includes and External ********************************

#include "FEB_Validate_Readings.h"

extern UART_HandleTypeDef huart2;

// ******************************** Global Variables ********************************

const uint8_t CELL_VOLTAGE_THRESHOLD_mV = 60; //TBD
const uint8_t CELL_TEMP_THRESHOLD_mV = 30; //TBD

// ******************************** Functions ********************************

void FEB_Validate_Readings_Validate_Voltages(uint8_t ic, float expected_voltage) {
	for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
		float polled_voltage = FEB_LTC6811_Get_Voltage(ic, cell);
		if (polled_voltage + CELL_VOLTAGE_THRESHOLD_mV * 0.0001 > expected_voltage || polled_voltage - CELL_VOLTAGE_THRESHOLD_mV * 0.0001 < expected_voltage) {
			char errmsg[128];
			sprintf(errmsg, "Cell %d. Expected voltage: %f, actual voltage: %f", cell, expected_voltage, polled_voltage);
			HAL_UART_Transmit_IT(&huart2, (uint8_t*) errmsg, strlen(errmsg));
		}
	}
}

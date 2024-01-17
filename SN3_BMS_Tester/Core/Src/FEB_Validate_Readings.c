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



// ******************************** Functions ********************************

void FEB_Validate_Readings_Validate_Voltages(uint8_t ic, float expected_voltage, uint8_t curr_cell) {
	for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
		float polled_voltage = FEB_LTC6811_Get_Voltage(ic, cell);
		if (curr_cell == cell) {
			if (polled_voltage + CELL_VOLTAGE_THRESHOLD_mV * 0.0001 > expected_voltage || polled_voltage - CELL_VOLTAGE_THRESHOLD_mV * 0.0001 < expected_voltage) {
				char errmsg[128];
				sprintf(errmsg, "Cell %d. Expected voltage: %f, actual voltage: %f", cell, expected_voltage, polled_voltage);
				HAL_UART_Transmit_IT(&huart2, (uint8_t*) errmsg, strlen(errmsg));
			}
		} else {
			if (polled_voltage + CELL_VOLTAGE_THRESHOLD_mV * 0.0001 > CELL_DEFAULT_VOLTAGE || polled_voltage - CELL_VOLTAGE_THRESHOLD_mV * 0.0001 < CELL_DEFAULT_VOLTAGE) {
				char errmsg[128];
				sprintf(errmsg, "Cell %d. Expected voltage: %f, actual voltage: %f", cell, CELL_DEFAULT_VOLTAGE, polled_voltage);
				HAL_UART_Transmit_IT(&huart2, (uint8_t*) errmsg, strlen(errmsg));
			}
		}

	}
}


void FEB_Validate_Readings_Validate_Temperatures(uint8_t ic, float expected_temperature_voltage) {
	for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
		float polled_temperature_voltage = FEB_LTC6811_Get_Temperature_Voltage(ic, cell);
		if (polled_temperature_voltage + CELL_TEMP_THRESHOLD_mV * 0.0001 > expected_temperature_voltage || polled_temperature_voltage - CELL_TEMP_THRESHOLD_mV * 0.0001 < expected_temperature_voltage) {
			char errmsg[128];
			sprintf(errmsg, "Cell %d. Expected temp voltage: %f, actual temp voltage: %f", cell, expected_temperature_voltage, polled_temperature_voltage);
			HAL_UART_Transmit_IT(&huart2, (uint8_t*) errmsg, strlen(errmsg));
		}
	}
}

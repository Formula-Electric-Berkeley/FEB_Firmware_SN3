/*
 * FEB_Validate_Readings.c
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */


// ******************************** Includes and External ********************************

#include <stdio.h>

#include "FEB_Validate_Readings.h"

extern UART_HandleTypeDef huart2;

// ******************************** Global Variables ********************************



// ******************************** Functions ********************************

void FEB_Validate_Readings_Validate_Voltages(uint8_t ic, float expected_voltage, uint8_t curr_cell) {
	for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
		float polled_voltage = FEB_LTC6811_Get_Voltage(ic, cell);
		if (curr_cell == cell) {
			float voltage_threshold_high = expected_voltage + CELL_VOLTAGE_THRESHOLD_mV * 0.001;
			float voltage_threshold_low = expected_voltage - CELL_VOLTAGE_THRESHOLD_mV * 0.001;
			if (polled_voltage > voltage_threshold_high || polled_voltage < voltage_threshold_low) {
				char errmsg[256];
				sprintf(errmsg, "Cell %d. Expected voltage: %f, actual voltage: %f\n", cell, expected_voltage, polled_voltage);
				HAL_UART_Transmit(&huart2, (uint8_t*) errmsg, strlen(errmsg), 100);
			}
		} else {
			float voltage_threshold_high = CELL_DEFAULT_VOLTAGE + CELL_VOLTAGE_THRESHOLD_mV * 0.001;
			float voltage_threshold_low = CELL_DEFAULT_VOLTAGE - CELL_VOLTAGE_THRESHOLD_mV * 0.001;
			if (polled_voltage > voltage_threshold_high || polled_voltage < voltage_threshold_low) {
				char errmsg[256];
				sprintf(errmsg, "Cell %d. Expected voltage: %f, actual voltage: %f\n", cell, CELL_DEFAULT_VOLTAGE, polled_voltage);
				HAL_UART_Transmit(&huart2, (uint8_t*) errmsg, strlen(errmsg), 100);
			}
		}

	}
}


void FEB_Validate_Readings_Validate_Temperatures(uint8_t ic, float expected_temperature_voltage) {
	for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
		float polled_temperature_voltage = FEB_LTC6811_Get_Temperature_Voltage(ic, cell);
		if (polled_temperature_voltage > expected_temperature_voltage + CELL_TEMP_THRESHOLD_mV * 0.0001 || polled_temperature_voltage < expected_temperature_voltage - CELL_TEMP_THRESHOLD_mV * 0.0001) {
			char errmsg[256];
			sprintf(errmsg, "Cell %d. Expected temp voltage: %f, actual temp voltage: %f\n", cell, expected_temperature_voltage, polled_temperature_voltage);
			HAL_UART_Transmit(&huart2, (uint8_t*) errmsg, strlen(errmsg), 100);
		}
	}
}

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

/*
 * Validate cell voltage readings based on expected voltage
 * Loop through all cells. If current cell is equal to the cell currently being tested, check if polled voltage
 * is within threshold of inputed voltage. Else, check if polled voltage is within threshold of default voltage.
 * If polled voltage is out of range, log error message to user.
 */
void FEB_Validate_Readings_Validate_Voltages(uint8_t ic, float expected_voltage, uint8_t curr_cell) {
	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {
		float polled_voltage = FEB_LTC6811_Get_Voltage(ic, cell);
		if (curr_cell == cell) {
			float voltage_threshold_high = expected_voltage + FEB_CONSTANT_CELL_VOLTAGE_THRESHOLD_V;
			float voltage_threshold_low = expected_voltage - FEB_CONSTANT_CELL_VOLTAGE_THRESHOLD_V;
			FEB_Validate_Readings_Validate_Cell(ic, cell, expected_voltage, polled_voltage, voltage_threshold_high, voltage_threshold_low);
		} else {
			float voltage_threshold_high = FEB_CONSTANT_CELL_DEFAULT_VOLTAGE_V + FEB_CONSTANT_CELL_VOLTAGE_THRESHOLD_V;
			float voltage_threshold_low = FEB_CONSTANT_CELL_DEFAULT_VOLTAGE_V - FEB_CONSTANT_CELL_VOLTAGE_THRESHOLD_V;
			FEB_Validate_Readings_Validate_Cell(ic, cell, expected_voltage, polled_voltage, voltage_threshold_high, voltage_threshold_low);
		}

	}
}

/*
 * Validate cell temperature voltage readings based on expected temperature voltage
 * Loop through all cells. If current cell is equal to the cell currently being tested, check if polled voltage
 * is within threshold of inputed voltage. Else, check if polled voltage is within threshold of default voltage.
 * If polled voltage is out of range, log error message to user.
 */
void FEB_Validate_Readings_Validate_Temperatures(uint8_t ic, float expected_temperature_voltage, uint8_t curr_cell) {
		float polled_temperature_voltage = FEB_LTC6811_Get_Temperature_Voltage(ic, curr_cell);
		float temp_voltage_threshold_high = expected_temperature_voltage + FEB_CONSTANT_CELL_TEMP_THRESHOLD_V;
		float temp_voltage_threshold_low = expected_temperature_voltage - FEB_CONSTANT_CELL_TEMP_THRESHOLD_V;
		FEB_Validate_Readings_Validate_Cell(ic, curr_cell, expected_temperature_voltage, polled_temperature_voltage, temp_voltage_threshold_high, temp_voltage_threshold_low);
}

/*
 * If inputed polled voltage is greater than threshold high or lower than threshold low,
 * log error to serial monitor.
 */
void FEB_Validate_Readings_Validate_Cell(uint8_t ic, uint8_t cell, float expected_voltage, float polled_voltage, float threshold_high, float threshold_low) {
	if (polled_voltage > threshold_high || polled_voltage < threshold_low) {
		char errmsg[256];
		sprintf(errmsg, "Cell: %d. IC: %d. Expected Voltage: %f, Actual Voltage: %f\n", cell, ic, expected_voltage, polled_voltage);
		HAL_UART_Transmit(&huart2, (uint8_t*) errmsg, strlen(errmsg), 100);
	}
}

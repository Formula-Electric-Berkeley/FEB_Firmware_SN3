/*
 * FEB_Input_Voltages.c
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */


// ******************************** Includes and External ********************************

#include "FEB_Input_Voltages.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Global Variables ********************************

int CURR_CELL = 0;
const uint8_t AD5243_ADDRESS = 0x2F << 1; //Address of Potentiometer: 0101 1110. Last bit: W bit.

// ******************************** Functions ********************************

void FEB_Input_Voltages_Input_Cell_Voltage(uint8_t cell, float voltage) {
	uint16_t input = (uint16_t) (voltage * (2 << 13) / 5) << 2;
	char buf[2]; //Buffer for SPI
	buf[0] = ((uint16_t)input >> 0) & 0xFF;
	buf[1] = ((uint16_t)input >> 8) & 0xFF;
	HAL_SPI_Transmit(&hspi2, (uint8_t *)buf, 2, 100); //Send data to DAC
}


// ******************************** Test Temperatures ********************************

void FEB_Input_Voltages_Input_Temp_Voltage(uint8_t voltage) {
	//TODO: Set potentiometer resistance based on voltages corresponding
	FEB_Aux_Tester_Set_Potentiometer_Resistance(voltage);
}

void FEB_Aux_Tester_Set_Potentiometer_Resistance(uint8_t voltage) {
	//TODO: probably want to do the math in here
	uint8_t buf[2];
	buf[0] = 0x00; //MSB Low selects channel 1
	buf[1] = voltage;
	HAL_I2C_Master_Transmit(&hi2c1, AD5243_ADDRESS, buf, 2, 100); //Write value to potentiometer
}



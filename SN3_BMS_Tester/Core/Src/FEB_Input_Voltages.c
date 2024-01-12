/*
 * FEB_Input_Voltages.c
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */


// ******************************** Includes and External ********************************

#include "FEB_Input_Voltages.h";

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Global Variables ********************************

int CURR_CELL = 0;

const uint8_t AD5243_ADDRESS = 0x2F << 1; //Address of Potentiometer: 0101 1110. Last bit: W bit.

// ******************************** Functions ********************************

void FEB_Input_Voltages_Input_Cell_Voltage(uint8_t cell, float voltage) {
	char buf[2]; //Buffer for SPI
	//TODO: Find a way to convert voltage into data to be sent to DAC
	HAL_SPI_Transmit(&hspi2, (uint8_t *)buf, 1, 100); //Send data to DAC
}


// ******************************** Test Temperatures ********************************

void FEB_Input_Voltages_Input_Temp_Voltage(uint8_t voltage) {
	//TODO: Set potentiometer resistance based on Temp LUT
	FEB_Aux_Tester_Set_Potentiometer_Resistance(voltage);
}

void FEB_Aux_Tester_Set_Potentiometer_Resistance(uint8_t resistance) {
	uint8_t buf[2];
	buf[0] = 0x00; //MSB Low selects channel 1
	buf[1] = resistance;
	HAL_I2C_Master_Transmit(&hi2c1, AD5243_ADDRESS, buf, 2, 100); //Write value to potentiometer
}




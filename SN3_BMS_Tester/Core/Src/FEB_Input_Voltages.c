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

#define AD5259_ADDRESS ((uint8_t) 0x30 << 1) //Address of Potentiometer: 0011 0000. Last bit: W bit.
#define AD5259_INSTRUCTION_BYTE ((uint8_t) 0x00); //Instruction byte: 0000 0000

// ******************************** Functions ********************************

void FEB_Input_Voltages_Input_Cell_Voltage(uint8_t cell, float voltage) {
	uint16_t input = (uint16_t) ((voltage + 5) * (2 << 11) / 10) << 2; //Based on equation on datasheet
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
	buf[0] = AD5259_INSTRUCTION_BYTE;
	//buf[1] = TODO: something;
	HAL_I2C_Master_Transmit(&hi2c1, AD5259_ADDRESS, buf, 2, 100); //Write value to potentiometer
}




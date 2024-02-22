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

#define AD5259_ADDRESS ((uint8_t) 0x30) //Address of Potentiometer: 0011 0000. Last bit: W bit.
#define AD5259_INSTRUCTION_BYTE ((uint8_t) 0x00); //Instruction byte: 0000 0000

// ******************************** Functions ********************************

/*
 * We use a DAC to input a voltage to a specified cell.
 * The equation used to get the input is based on datasheet (page 22):
 * https://www.ti.com/lit/ds/symlink/dac7311.pdf?ts=1706682887058
 */
void FEB_Input_Voltages_Input_Cell_Voltage(uint8_t cell, float voltage) {

	//This input is calculated based on equation on datasheet (page 22).
	//It is left shfited by two bits to account for two "don't care bits"
    uint16_t input = (uint16_t) ((voltage) * (2 << 11)) / 5 << 2;

    //Buffer for SPI
	char buf[2];

	//Separate 16 bit input into two bytes
	buf[0] = ((uint16_t)input >> 0) & 0xFF;
	buf[1] = ((uint16_t)input >> 8) & 0xFF;

	FEB_BMS_Tester_Hardware_Set_DAC_CS_n(cell, false); //CS low
	HAL_Delay(5); //Delay 5ms
	HAL_SPI_Transmit(&hspi2, (uint8_t *)buf, 2, 100); //Send data to DAC
	HAL_Delay(5); //Delay 5ms
	FEB_BMS_Tester_Hardware_Set_DAC_CS_n(cell, true); //CS high
}


// ******************************** Test Temperatures ********************************

/*
 * We use a digital potentiometer and voltage divider to input a voltage
 * corresponding to a temperature for a specified cell.
 */
void FEB_Input_Voltages_Input_Temp_Voltage(uint8_t decimal_value) {
	char buf[2];
	buf[0] = AD5259_INSTRUCTION_BYTE;
	buf[1] = decimal_value;
	HAL_I2C_Master_Transmit(&hi2c1, AD5259_ADDRESS, (uint8_t *)buf, 2, 100); //Write value to potentiometer
}





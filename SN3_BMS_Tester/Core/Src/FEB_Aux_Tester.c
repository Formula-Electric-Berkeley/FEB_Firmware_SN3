// ******************************** Includes and External ********************************

#include "FEB_Aux_Tester.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Global Variables ********************************
const uint8_t NUM_CELLS = 10;

const uint8_t AD5243_ADDRESS = 0x2F << 1; //Address of Potentiometer: 0101 1110. Last bit: W bit.

const float CELL_DEFAULT_VOLTAGE = 3.75;
const float TEMP_DEFAULT_VOLTAGE = 1.75;

const float CELL_MIN_VOLTAGE = 2.2;
const float CELL_MAX_VOLTAGE = 4.5;
const float CELL_MIN_TEMP_VOLTAGE = 1;
const float CELL_MAX_TEMP_VOLTAGE = 2.7;

const uint8_t CELL_VOLTAGE_THRESHOLD_mV = 60; //TBD
const uint8_t CELL_TEMP_THRESHOLD_mV = 30; //TBD
// ******************************** Functions ********************************

void FEB_Aux_Tester_Test_Aux(void) {
	FEB_Aux_Tester_Init();
	FEB_Aux_Tester_Test_Cell_Voltages(); //Test cell voltages
	FEB_Aux_Tester_Test_Cell_Temps(); //Test cell temperatures
}

void FEB_Aux_Tester_Init(void) {
	for (int cell = 1; cell <= NUM_CELLS; cell++) {
		FEB_Input_Voltages_Input_Cell_Voltage(cell, CELL_DEFAULT_VOLTAGE);
	}
	FEB_Input_Voltages_Input_Temp_Voltage(TEMP_DEFAULT_VOLTAGE);
}

// ******************************** Test Voltages ********************************

void FEB_Aux_Tester_Test_Cell_Voltages(void) {
	for (int cell = 1; cell <= NUM_CELLS; cell++) {
		for (float voltage = CELL_MIN_VOLTAGE; voltage <= CELL_MAX_VOLTAGE; voltage += 0.1) { //TODO: Figure out how much to increment by
			FEB_Input_Voltages_Input_Cell_Voltage(cell, voltage);
			//READ OUTPUTS
			//VALIDATE OUTPUTS
		}
	}
}


// ******************************** Test Temperatures ********************************

void FEB_Aux_Tester_Test_Cell_Temps(void) {
	for (float temp_voltage = CELL_MIN_TEMP_VOLTAGE; temp_voltage <= CELL_MAX_TEMP_VOLTAGE; temp_voltage += 0.1) { //TODO: Figure out how much to increment by
		FEB_Input_Voltages_Input_Temp_Voltage(temp_voltage);
		//READ OUTPUTS
		//VALIDATE OUTPUTS
	}
}





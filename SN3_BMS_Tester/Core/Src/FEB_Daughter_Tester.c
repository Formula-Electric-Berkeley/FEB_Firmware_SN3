// ******************************** Includes and External ********************************

#include "FEB_Daughter_Tester.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Functions ********************************

void FEB_Daughter_Tester_Test_Daughter(void) {
	FEB_Aux_Tester_Init();
	FEB_Aux_Tester_Test_Cell_Voltages(); //Test cell voltages
	FEB_Aux_Tester_Test_Cell_Temps(); //Test cell temperatures
}

void FEB_Daughter_Tester_Init(void) {
	//set all cells to default values
	for (uint8_t cell = 0; cell < 10; cell++) {
		//voltage
		FEB_BMS_Tester_Hardware_Set_DAC_CS_n(cell, 0); //CS low
		HAL_Delay(5); //Delay 5ms
		FEB_Input_Voltages_Input_Cell_Voltage(cell, CELL_DEFAULT_VOLTAGE);
		HAL_Delay(5); //Delay 5ms
		FEB_BMS_Tester_Hardware_Set_DAC_CS_n(cell, 1); //CS high

		//temperature
		FEB_BMS_Tester_Hardware_Configure_MUX(cell);
		FEB_Input_Voltages_Input_Temp_Voltage(TEMP_DEFAULT_VOLTAGE);
	}
}

// ******************************** Test Voltages ********************************

void FEB_Daughter_Tester_Test_Cell_Voltages(void) {

	for (uint8_t curr_cell = 0; curr_cell < NUM_CELLS; curr_cell++) {
		for (float voltage = CELL_MIN_VOLTAGE; voltage <= CELL_MAX_VOLTAGE; voltage += 0.1) {
				FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, 0); //CS low
				HAL_Delay(5); //Delay 5ms
				FEB_Input_Voltages_Input_Cell_Voltage(curr_cell, voltage);
				HAL_Delay(5); //Delay 5ms
				FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, 1); //CS high
				FEB_LTC6811_Poll_Voltage();
				FEB_Validate_Readings_Validate_Voltages(1, voltage, curr_cell); //IC 1 for daughter board
			}
			FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, 0); //CS low
			HAL_Delay(5); //Delay 5ms
			FEB_Input_Voltages_Input_Cell_Voltage(curr_cell, CELL_DEFAULT_VOLTAGE); //Reset Cell Voltage
			HAL_Delay(5); //Delay 5ms
			FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, 1); //CS high
	}
}


// ******************************** Test Temperatures ********************************

void FEB_Daughter_Tester_Test_Cell_Temps(void) {
	for (uint8_t cell = 0; cell < NUM_CELLS; cell++) {
		FEB_BMS_Tester_Hardware_Configure_MUX(cell);
		for (float temp_voltage = CELL_MIN_TEMP_VOLTAGE; temp_voltage <= CELL_MAX_TEMP_VOLTAGE; temp_voltage += 0.05) { //TODO: Figure out how much to increment by
			FEB_Input_Voltages_Input_Temp_Voltage(temp_voltage);
			FEB_LTC6811_Poll_Temperature();
			FEB_Validate_Readings_Validate_Temperatures(1, temp_voltage); //IC 1 for daughter board
		}
	}
}






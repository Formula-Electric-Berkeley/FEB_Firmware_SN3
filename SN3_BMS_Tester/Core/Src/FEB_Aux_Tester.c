// ******************************** Includes and External ********************************

#include "FEB_Aux_Tester.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Functions ********************************

void FEB_Aux_Tester_Test_Aux(void) {
	FEB_Aux_Tester_Init();
	FEB_Aux_Tester_Test_Cell_Voltages(); //Test cell voltages
	FEB_Aux_Tester_Test_Cell_Temps(); //Test cell temperatures
}

void FEB_Aux_Tester_Init(void) {
	//set all cells to default values
	for (uint8_t cell = 0; cell < 10; cell++) {
		//voltage
		FEB_BMS_Tester_Hardware_Set_DAC_CS_n(cell, false); //CS low
		HAL_Delay(5); //Delay 5ms
		FEB_Input_Voltages_Input_Cell_Voltage(cell, FEB_CONSTANT_CELL_DEFAULT_VOLTAGE);
		HAL_Delay(5); //Delay 5ms
		FEB_BMS_Tester_Hardware_Set_DAC_CS_n(cell, true); //CS high

		//temperature
		FEB_BMS_Tester_Hardware_Configure_MUX(cell);
		FEB_Input_Voltages_Input_Temp_Voltage(FEB_CONSTANT_TEMP_DEFAULT_VOLTAGE);
	}
}

// ******************************** Test Voltages ********************************

void FEB_Aux_Tester_Test_Cell_Voltages(void) {

	for (uint8_t curr_cell = 0; curr_cell < FEB_CONSTANT_NUM_CELLS; curr_cell++) {
		for (float voltage = FEB_CONSTANT_CELL_MIN_VOLTAGE; voltage <= FEB_CONSTANT_CELL_MAX_VOLTAGE; voltage += 0.1) {
				FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, false); //CS low
				HAL_Delay(5); //Delay 5ms
				FEB_Input_Voltages_Input_Cell_Voltage(curr_cell, voltage); //Input voltages
				HAL_Delay(5); //Delay 5ms
				FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, true); //CS high
				FEB_LTC6811_Poll_Voltage(); //Poll voltages
				FEB_LTC6811_Poll_Temperature(); //Poll temperature
				FEB_Validate_Readings_Validate_Voltages(FEB_CONSTANT_AUX_TESTER_IC, voltage, curr_cell); //IC for pseudo-daughter board
			}
			FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, false); //CS low
			HAL_Delay(5); //Delay 5ms
			FEB_Input_Voltages_Input_Cell_Voltage(curr_cell, FEB_CONSTANT_CELL_DEFAULT_VOLTAGE); //Reset Cell Voltage
			HAL_Delay(5); //Delay 5ms
			FEB_BMS_Tester_Hardware_Set_DAC_CS_n(curr_cell, true); //CS high
	}
}


// ******************************** Test Temperatures ********************************

void FEB_Aux_Tester_Test_Cell_Temps(void) {
	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS; cell++) {
		FEB_BMS_Tester_Hardware_Configure_MUX(cell);
		for (float temp_voltage = FEB_CONSTANT_CELL_MIN_TEMP_VOLTAGE; temp_voltage <= FEB_CONSTANT_CELL_MAX_TEMP_VOLTAGE; temp_voltage += 0.05) { //TODO: Figure out how much to increment by
			FEB_Input_Voltages_Input_Temp_Voltage(temp_voltage);
			FEB_LTC6811_Poll_Voltage(); //Poll voltages
			FEB_LTC6811_Poll_Temperature(); //Poll temperatures
			FEB_Validate_Readings_Validate_Temperatures(FEB_CONSTANT_AUX_TESTER_IC, temp_voltage, cell); //IC for pseudo-daughter board
		}
		FEB_Input_Voltages_Input_Temp_Voltage(FEB_CONSTANT_TEMP_DEFAULT_VOLTAGE);
	}
}






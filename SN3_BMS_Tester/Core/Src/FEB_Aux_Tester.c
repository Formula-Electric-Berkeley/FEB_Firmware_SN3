// ******************************** Includes and External ********************************

#include "FEB_Aux_Tester.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Functions ********************************

//Test external aux board.
void FEB_Aux_Tester_Test_Aux(void) {
	FEB_Aux_Tester_Init();
	FEB_Aux_Tester_Test_Cell_Voltages(); //Test cell voltages
	FEB_Aux_Tester_Test_Cell_Temps(); //Test cell temperatures
}

//Set all cell voltages and temperatures to the default values
void FEB_Aux_Tester_Init(void) {

	//loop through all cells
	for (uint8_t cell = 0; cell < 10; cell++) {
		//voltage
		FEB_Input_Voltages_Input_Cell_Voltage(cell, FEB_CONSTANT_CELL_DEFAULT_VOLTAGE);

		//temperature
		FEB_BMS_Tester_Hardware_Configure_MUX(cell);
		FEB_Input_Voltages_Input_Temp_Voltage(cell, FEB_CONSTANT_TEMP_DEFAULT_VOLTAGE);
	}

}

// ******************************** Test Voltages ********************************

/*
 * Test cell voltages:
 * We loop through all cells
 * For each cell, we input a range of voltages, starting from min voltage, ending at max voltage
 * incrementing by 0.1 volts each time
 * After inputting voltage, read voltages and temperatures from all 10 cells via "pseudo daughter board,
 * and make sure that current cell voltage is equal to expected voltage, and all other cells are equal to default value.
 */
void FEB_Aux_Tester_Test_Cell_Voltages(void) {
	FEB_BMS_Tester_Hardware_Transmit_Start_Testing("Aux", "voltages");

	//Loop through all cells
	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS; cell++) {

		//Loop through range of voltages
		for (float voltage = FEB_CONSTANT_CELL_MIN_VOLTAGE; voltage <= FEB_CONSTANT_CELL_MAX_VOLTAGE; voltage += 0.1) {
				FEB_Input_Voltages_Input_Cell_Voltage(cell, voltage); //Input voltages
				FEB_LTC6811_Poll_Voltage(); //Poll voltages
				FEB_LTC6811_Poll_Temperature(); //Poll temperature
				FEB_Validate_Readings_Validate_Voltages(FEB_CONSTANT_AUX_TESTER_IC, voltage, cell); //IC for pseudo-daughter board
			}
			FEB_Input_Voltages_Input_Cell_Voltage(cell, FEB_CONSTANT_CELL_DEFAULT_VOLTAGE); //Reset Cell Voltage
	}
	FEB_BMS_Tester_Hardware_Transmit_Done_Testing("Aux", "voltages");
}

// ******************************** Test Temperatures ********************************

/*
 * Test cell temperatures:
 * We loop through all cells
 * For each cell, we input a range of voltages corresponding to temperatures, starting from min voltage, ending at max voltage
 * incrementing by 0.05 volts each time
 * After inputting voltage, read voltages and temperatures from all 10 cells, and make sure that
 * current cell voltage is equal to expected voltage, and all other cells are equal to default value.
 */
void FEB_Aux_Tester_Test_Cell_Temps(void) {
	FEB_BMS_Tester_Hardware_Transmit_Start_Testing("Aux", "temperatures");

	//Loop through all cells
	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS; cell++) {
		FEB_BMS_Tester_Hardware_Configure_MUX(cell); // Configure MUX based on cell
		//Loop through all temperature voltages.
		for (float temp_voltage = FEB_CONSTANT_CELL_MIN_TEMP_VOLTAGE; temp_voltage <= FEB_CONSTANT_CELL_MAX_TEMP_VOLTAGE; temp_voltage += 0.05) { //TODO: Figure out how much to increment by
			FEB_Input_Voltages_Input_Temp_Voltage(cell, temp_voltage); //Input temp voltage
			FEB_LTC6811_Poll_Voltage(); //Poll voltages
			FEB_LTC6811_Poll_Temperature(); //Poll temperatures
			FEB_Validate_Readings_Validate_Temperatures(FEB_CONSTANT_AUX_TESTER_IC, temp_voltage, cell); //IC for pseudo-daughter board
		}
		FEB_Input_Voltages_Input_Temp_Voltage(cell, FEB_CONSTANT_TEMP_DEFAULT_VOLTAGE); //Reset temp voltage
	}
	FEB_BMS_Tester_Hardware_Transmit_Done_Testing("Aux", "temperatures");
}






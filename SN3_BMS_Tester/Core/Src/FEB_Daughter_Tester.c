// ******************************** Includes and External ********************************

#include "FEB_Daughter_Tester.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Functions ********************************

//Test external daughter board
void FEB_Daughter_Tester_Test_Daughter(void) {
	FEB_Daughter_Tester_Init();
	FEB_Daughter_Tester_Test_Cell_Voltages(); //Test cell voltages
	FEB_Daughter_Tester_Test_Cell_Temps(); //Test cell temperatures
}

//Set all cell voltages to the default values
void FEB_Daughter_Tester_Init(void) {
	FEB_LTC6811_Init(); //Init LTC

	//set all cells to default values
	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {
		//voltage
		FEB_Input_Voltages_Input_Cell_Voltage(cell, FEB_CONSTANT_CELL_DEFAULT_VOLTAGE_V);
	}
}

// ******************************** Test Voltages ********************************

/*
 * Test cell voltages:
 * We loop through all cells
 * For each cell, we input a range of voltages, starting from min voltage, ending at max voltage
 * incrementing by 0.1 volts each time
 * After inputting voltage, read voltages and temperatures from all 10 cells via external daughter board
 * that is currently being tested, and make sure that current cell voltage is equal to expected voltage,
 * and all other cells are equal to default value.
 */
void FEB_Daughter_Tester_Test_Cell_Voltages(void) {
	FEB_BMS_Tester_Hardware_Transmit_Start_Testing("Daughter", "voltages");

	//Loop through all cells
	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {

		//Loop through range of voltages
		for (float voltage_V = FEB_CONSTANT_CELL_MIN_VOLTAGE_V; voltage_V <= FEB_CONSTANT_CELL_MAX_VOLTAGE_V + 0.05; voltage_V += 0.1) {
				FEB_Input_Voltages_Input_Cell_Voltage(cell, voltage_V); //Input voltage
				HAL_Delay(10); //delay so voltage can stabalize
				FEB_LTC6811_Poll_Voltage(); //Poll voltage
				FEB_LTC6811_Poll_Temperature(); //Poll temperature
				FEB_Validate_Readings_Validate_Voltages(FEB_CONSTANT_DAUGHTER_TESTER_IC, voltage_V, cell); //IC for daughter board
				FEB_LTC6811_Clear_Voltage();
				FEB_LTC6811_Clear_Temperature();
			}
			FEB_Input_Voltages_Input_Cell_Voltage(cell, FEB_CONSTANT_CELL_DEFAULT_VOLTAGE_V); //Reset Cell Voltage
	}
	FEB_BMS_Tester_Hardware_Transmit_Done_Testing("Daughter", "voltages");
}


// ******************************** Test Temperatures ********************************

/*
 * Test cell temperatures:
 * We loop through all cells
 * For each cell, we input a range of voltages corresponding to temperatures, starting from min voltage, ending at max voltage
 * incrementing by 0.05 volts each time
 * After inputting voltage, read voltages and temperatures from all 10 cells via external daughter board
 * that is currently being tested, and make sure that current cell voltage is equal to expected voltage,
 * and all other cells are equal to default value.
 */
void FEB_Daughter_Tester_Test_Cell_Temps(void) {
	FEB_BMS_Tester_Hardware_Transmit_Start_Testing("Daughter", "temperatures");

	//Loop through all cells
	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {
		FEB_BMS_Tester_Hardware_Configure_MUX(cell); //Configure MUX based on cell

		//Loop through range of potentiometer decimal values
		for (uint8_t d_value = 17; d_value <= 70; d_value++) {
			FEB_Input_Voltages_Input_Temp_Voltage(d_value);
			HAL_Delay(10);
			float resistance = d_value * 10000 / 256;
			float expected_temp_voltage_V = 5 * 680 / (resistance + 680);
			FEB_LTC6811_Poll_Temperature(); //Poll voltage
			FEB_LTC6811_Poll_Temperature(); //Poll temperature
			FEB_Validate_Readings_Validate_Temperatures(FEB_CONSTANT_DAUGHTER_TESTER_IC, expected_temp_voltage_V, cell); //IC for daughter board
			FEB_LTC6811_Clear_Voltage();
			FEB_LTC6811_Clear_Temperature();
		}
	}
	FEB_BMS_Tester_Hardware_Transmit_Done_Testing("Daughter", "temperatures");
}





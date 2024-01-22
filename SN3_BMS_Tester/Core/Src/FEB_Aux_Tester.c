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
	//all cs low
	FEB_Input_Voltages_Input_Cell_Voltage(cell, CELL_DEFAULT_VOLTAGE);
	FEB_Input_Voltages_Input_Temp_Voltage(TEMP_DEFAULT_VOLTAGE);
}

// ******************************** Test Voltages ********************************

void FEB_Aux_Tester_Test_Cell_Voltages(void) {

	uint8_t curr_cell = 1;
	for (float voltage = CELL_MIN_VOLTAGE; voltage <= CELL_MAX_VOLTAGE; voltage += 0.1) { //TODO: Figure out how much to increment by
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //CS1 Low
		//add a delay 5 ms
		FEB_Input_Voltages_Input_Cell_Voltage(curr_cell, voltage);
		//add a delay 5 ms
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); //CS1 High
		FEB_LTC6811_Poll_Voltage();
		FEB_Validate_Readings_Validate_Voltages(0, voltage, curr_cell); //TODO: Figure out which IC to pass in
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //CS1 Low
	FEB_Input_Voltages_Input_Cell_Voltage(curr_cell, CELL_DEFAULT_VOLTAGE); //Reset Cell Voltage
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); //CS1 High

}


// ******************************** Test Temperatures ********************************

void FEB_Aux_Tester_Test_Cell_Temps(void) {
	for (uint8_t cell = 0; cell < NUM_CELLS; cell++) {
		FEB_Aux_Tester_Configure_MUX(cell);
		for (float temp_voltage = CELL_MIN_TEMP_VOLTAGE; temp_voltage <= CELL_MAX_TEMP_VOLTAGE; temp_voltage += 0.05) { //TODO: Figure out how much to increment by
			FEB_Input_Voltages_Input_Temp_Voltage(temp_voltage);
			FEB_LTC6811_Poll_Temperature();
			FEB_Validate_Readings_Validate_Temperatures(0, temp_voltage); //TODO: Figure out which IC to pass in
		}
	}
}

void FEB_Aux_Tester_Configure_MUX(uint8_t cell) {
	uint8_t channel = cell;
	uint8_t select0 = (channel & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	uint8_t select1 = ((channel >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	uint8_t select2 = ((channel >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	uint8_t select3 = ((channel >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, select0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, select1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, select2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, select3);
}





/*
 * FEB_LTC6811.c
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */


// ******************************** Includes and External ********************************

#include "FEB_LTC6811.h"
#include <stdio.h>

// ********************************** Structs **********************************

typedef struct {
	float temp; //voltage corresponding to temp. //temperature_volt. TODO: add units
	float voltage; //voltage //cell_volt
} Cell;

typedef struct {
	Cell cells[FEB_CONSTANT_NUM_ACC_CELLS_PER_IC];
} Chip;

typedef struct {
	cell_asic IC_config[TOTAL_IC];
	Chip chips[TOTAL_IC];
} TestAccumulator_t;

// ******************************** Static Function Declaration ********************************

static uint8_t get_LTC6811_cell(uint8_t accumulator_cell);
static int8_t get_accumulator_cell(uint8_t IC, uint8_t LTC6811_cell);

// ********************************** LTC6811 Config **********************************

bool REFON = true; //!< Reference Powered Up Bit
bool ADCOPT = false; //!< ADC Mode option bit
bool gpioBits_a[5] = {true, true,false,false,false}; //!< GPIO Pin Control // Gpio 1,2,3,4,5 (1, 2) ADC, (3, 4, 5) MUX Select
uint16_t UV=UV_THRESHOLD; //!< Under-voltage Comparison Voltage
uint16_t OV=OV_THRESHOLD; //!< Over-voltage Comparison Voltage
bool dccBits_a[12] = {false,false,false,false,false,false,false,false,false,false,false,false}; //!< Discharge cell switch //Dcc 1,2,3,4,5,6,7,8,9,10,11,12
bool dctoBits[4] = {true, false, true, false}; //!< Discharge time value // Dcto 0,1,2,3 // Programed for 4 min

TestAccumulator_t accumulator;

// ******************************** Static Functions ********************************

static uint8_t get_LTC6811_cell(uint8_t accumulator_cell);
static int8_t get_accumulator_cell(uint8_t IC, uint8_t LTC6811_cell);
static uint8_t get_channel(uint8_t accumulator_cell);
static uint8_t get_mux(uint8_t accumulator_cell);


// ******************************** Accumulator Mapping ********************************

/*
Accumulator Cell (- terminal)			00	01	02	03	04	05	06	07	08	09	10	11	12	13	14	15	16	17	18	19
IC [0 (- terminal), 1 (+ terminal)]		0	0	0	0	0	0	0	0	0	0	1	1	1	1	1	1	1	1	1	1
LTC6811 Cell							0	1	2	3	4	6	7	8	9	10	0	1	2	3	4	6	7	8	9	10
Multiplexer								B	B	B	B	B	A	A	A	A	A	B	B	B	B	B	A	A	A	A	A
Channel									0	1	2	3	4	0	1	2	3	4	0	1	2	3	4	0	1	2	3	4
*/

// ******************************** Helper Functions ********************************

// Function maps accumulator cell -> LTC6811 cell
static uint8_t get_LTC6811_cell(uint8_t accumulator_cell) {
	// Map for each IC
	static uint8_t LTC6811_cell_map[FEB_CONSTANT_NUM_ACC_CELLS_PER_IC] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10};
	return LTC6811_cell_map[accumulator_cell % FEB_CONSTANT_NUM_ACC_CELLS_PER_IC];
}

static int8_t get_accumulator_cell(uint8_t IC, uint8_t LTC6811_cell) {
	static uint8_t acc_cell_map[FEB_CONST_NUM_LTC_CELLS_PER_IC] = {0, 1, 2, 3, 4, -1, 5, 6, 7, 8, 9, -1};
	return acc_cell_map[LTC6811_cell] + FEB_CONSTANT_NUM_ACC_CELLS_PER_IC * (IC % 2);
}

// Return multiplexer channel 0-4
static uint8_t get_channel(uint8_t accumulator_cell) {
	return accumulator_cell % 5;
}

// Return multiplexer 0 (A), 1 (B)
static uint8_t get_mux(uint8_t accumulator_cell) {
	if ((0 <= accumulator_cell && accumulator_cell <= 4) ||
		(10 <= accumulator_cell && accumulator_cell <= 14)) {
		// Multiplexer B: accumulator cell 0-4, 10-14
		return 1;
	} else {
		// Multiplexer A: accumulator cell 5-9, 15-19
		return 0;
	}
}

// ******************************** Functions ********************************

void FEB_LTC6811_Init(void) {
	LTC6811_init_cfg(TOTAL_IC, accumulator.IC_config);

	for(uint8_t current_ic = 0; current_ic < TOTAL_IC; current_ic++) {
		LTC6811_set_cfgr(current_ic, accumulator.IC_config, REFON, ADCOPT, gpioBits_a, dccBits_a, dctoBits, UV, OV);
	}

	LTC6811_reset_crc_count(TOTAL_IC, accumulator.IC_config);
	LTC6811_init_reg_limits(TOTAL_IC, accumulator.IC_config);
}

// ******************************** Read Voltage ********************************

void FEB_LTC6811_Poll_Voltage(void) {
	FEB_LTC6811_Start_Cell_ADC_Measurement();
	FEB_LTC6811_Read_Cell_Voltage_Registers();
	FEB_LTC6811_Store_Voltage();
}

void FEB_LTC6811_Start_Cell_ADC_Measurement(void) {
	wakeup_sleep(TOTAL_IC);
	LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();
}

void FEB_LTC6811_Read_Cell_Voltage_Registers(void) {
	wakeup_sleep(TOTAL_IC);
	LTC6811_rdcv(NO_OF_REG, TOTAL_IC, accumulator.IC_config);
}

void FEB_LTC6811_Store_Voltage(void) {
    for (uint8_t ic = 0; ic < TOTAL_IC; ic++) {
    	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {
    		accumulator.chips[ic].cells[cell].voltage = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].cells.c_codes[get_LTC6811_cell(cell)]);
    	}
    }
}

float FEB_LTC6811_Convert_Voltage(uint16_t value) {
	// Check for error: value = 2^16 - 1
	if (value == 65535) {
		return -42;
	}
	return value * 0.0001;
}

float FEB_LTC6811_Get_Voltage(uint8_t ic, uint8_t cell) {
	return accumulator.chips[ic].cells[cell].voltage;
}

void FEB_LTC6811_Clear_Voltage(void) {
	for (uint8_t ic = 0; ic < TOTAL_IC; ic++) {
		for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {
			accumulator.chips[ic].cells[cell].voltage = 0;
		}
	}
}


// ******************************** Read Temp ********************************

void FEB_LTC6811_Poll_Temperature(void) {
	for (uint8_t channel = 0; channel <= 4; channel++) {
		FEB_LTC6811_Update_GPIO(channel);
		FEB_LTC6811_Start_GPIO_ADC_Measurement();
		FEB_LTC6811_Read_Aux_Voltages();
		FEB_LTC6811_Store_Temperature(channel);
	}
}

void FEB_LTC6811_Update_GPIO(uint8_t channel) {
	gpioBits_a[0] = 0b1;					// ADC
	gpioBits_a[1] = 0b1;					// ADC
	gpioBits_a[2] = (channel >> 0) & 0b1;	// MUX Select
	gpioBits_a[3] = (channel >> 1) & 0b1;	// MUX Select
	gpioBits_a[4] = (channel >> 2) & 0b1;	// MUX Select
	for (uint8_t ic = 0; ic < TOTAL_IC; ic++) {
		LTC6811_set_cfgr(ic, accumulator.IC_config, REFON, ADCOPT, gpioBits_a, dccBits_a, dctoBits, UV, OV);
	}
    wakeup_idle(TOTAL_IC);
    LTC6811_wrcfg(TOTAL_IC, accumulator.IC_config);
}

void FEB_LTC6811_Start_GPIO_ADC_Measurement(void) {
	wakeup_sleep(TOTAL_IC);
	LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
	LTC6811_pollAdc();
}

void FEB_LTC6811_Read_Aux_Voltages() {
	wakeup_sleep(TOTAL_IC);
	LTC6811_rdaux(NO_OF_REG, TOTAL_IC, accumulator.IC_config);
}

void FEB_LTC6811_Store_Temperature(uint8_t channel) {
	//TODO: Figure out actual cell configuration. Below is just a guess
	for (uint8_t ic = 0; ic < TOTAL_IC; ic++) {
		for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {
			if (get_channel(cell) == channel) {
				uint8_t mux = get_mux(cell);
				accumulator.chips[ic].cells[cell].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[mux]);
			}
		}
	}
}


float FEB_LTC6811_Get_Temperature_Voltage(uint8_t ic, uint8_t cell) {
	return accumulator.chips[ic].cells[cell].temp;
}

void FEB_LTC6811_Clear_Temperature(void) {
	for (uint8_t ic = 0; ic < TOTAL_IC; ic++) {
		for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_ACC_CELLS_PER_IC; cell++) {
			accumulator.chips[ic].cells[cell].temp = 0;
		}
	}
}

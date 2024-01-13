/*
 * FEB_LTC6811.c
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */


// ******************************** Includes and External ********************************

#include "FEB_LTC6811.h"
#include <stdio.h>

// ********************************** LTC6811 Config **********************************

bool REFON = true; //!< Reference Powered Up Bit
bool ADCOPT = false; //!< ADC Mode option bit
bool gpioBits_a[5] = {true, true,false,false,false}; //!< GPIO Pin Control // Gpio 1,2,3,4,5 (1, 2) ADC, (3, 4, 5) MUX Select
uint16_t UV=UV_THRESHOLD; //!< Under-voltage Comparison Voltage
uint16_t OV=OV_THRESHOLD; //!< Over-voltage Comparison Voltage
bool dccBits_a[12] = {false,false,false,false,false,false,false,false,false,false,false,false}; //!< Discharge cell switch //Dcc 1,2,3,4,5,6,7,8,9,10,11,12
bool dctoBits[4] = {true, false, true, false}; //!< Discharge time value // Dcto 0,1,2,3 // Programed for 4 min

TestAccumulator accumulator;

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
    	for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
    		accumulator.chips[ic].cells[cell].voltage = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].cells.c_codes[cell]);
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


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

float FEB_LTC6811_Get_Voltage(uint8_t ic, uint8_t cell) {
	return accumulator.chips[ic].cells[cell].voltage;
}

void FEB_LTC6811_Clear_Voltage(void) {
	for (uint8_t ic = 0; ic < TOTAL_IC; ic++) {
		for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
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
    wakeup_sleep(TOTAL_IC);
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
		switch (channel) {
			case 0:
				accumulator.chips[ic].cells[4].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[0]);
				accumulator.chips[ic].cells[9].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[1]);
				break;
			case 1:
				accumulator.chips[ic].cells[3].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[0]);
				accumulator.chips[ic].cells[8].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[1]);;
				break;
			case 2:
				accumulator.chips[ic].cells[2].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[0]);
				accumulator.chips[ic].cells[7].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[1]);
				break;
			case 3:
				accumulator.chips[ic].cells[1].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[0]);
				accumulator.chips[ic].cells[6].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[1]);
				break;
			case 4:
				accumulator.chips[ic].cells[0].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[0]);
				accumulator.chips[ic].cells[5].temp = FEB_LTC6811_Convert_Voltage(accumulator.IC_config[ic].aux.a_codes[1]);
				break;
		}
	}
}


float FEB_LTC6811_Get_Temperature_Voltage(uint8_t ic, uint8_t cell) {
	return accumulator.chips[ic].cells[cell].temp;
}

void FEB_LTC6811_Clear_Temperature(void) {
	for (uint8_t ic = 0; ic < TOTAL_IC; ic++) {
		for (uint8_t cell = 0; cell < CELLS_TO_TEST; cell++) {
			accumulator.chips[ic].cells[cell].temp = 0;
		}
	}
}

// ******************************** Seet Discharge ********************************

//Change dccBits_a[cell] to 1, set rest to zero
void FEB_LTC6811_Configure_dccBits_a(uint8_t cell) {
	for (uint8_t i = 0; i < CELLS_TO_TEST; i++) {
		if (i == cell) {
			dccBits_a[i] = true;
		} else {
			dccBits_a[i] = false;
		}
	}
}

void FEB_LTC6811_Set_Discharge(uint8_t cell) {
	FEB_LTC6811_Configure_dccBits_a(cell); //set dccBits
	LTC6811_set_cfgr(FEB_CONSTANT_DAUGHTER_TESTER_IC, accumulator.IC_config, REFON, ADCOPT, gpioBits_a, dccBits_a, dctoBits, UV, OV); //set all configuration bits w/ updated dccBits
	wakeup_idle(TOTAL_IC); //wakeup ics
	LTC6811_wrcfg(TOTAL_IC, accumulator.IC_config); //write configuration bits to ics.
}

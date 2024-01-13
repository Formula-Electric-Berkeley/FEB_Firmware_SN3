/*
 * FEB_LTC6811.h
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */

#ifndef INC_FEB_LTC6811_H_
#define INC_FEB_LTC6811_H_

// ********************************** Includes **********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "LTC6811.h"

// ********************************** Test Accumulator Config **********************************

#define CELLS_TO_TEST 10

// ********************************** LTC6811 Config **********************************

#define TOTAL_IC 2//!< Number of ICs in the daisy chain. One on

#define ENABLED 1
#define DISABLED 0

//ADC Command Configurations. See LTC681x.h for options.
#define ADC_OPT ADC_OPT_DISABLED; //!< ADC Mode option bit
#define ADC_CONVERSION_MODE MD_7KHZ_3KHZ //!< ADC Mode
#define ADC_DCP DCP_ENABLED //!< Discharge Permitted
#define CELL_CH_TO_CONVERT CELL_CH_ALL //!< Channel Selection for ADC conversion
#define AUX_CH_TO_CONVERT AUX_CH_ALL //!< Channel Selection for ADC conversion
#define STAT_CH_TO_CONVERT STAT_CH_ALL //!< Channel Selection for ADC conversion
#define NO_OF_REG REG_ALL //!< Register Selection
#define MEASUREMENT_LOOP_TIME 500 //!< Loop Time in milliseconds(ms)

//Under Voltage and Over Voltage Thresholds
#define OV_THRESHOLD 41000 //!< Over voltage threshold ADC Code. LSB = 0.0001 ---(4.1V)
#define UV_THRESHOLD 30000 //!< Under voltage threshold ADC Code. LSB = 0.0001 ---(3V)

//Loop Measurement Setup. These Variables are ENABLED or DISABLED. Remember ALL CAPS
#define WRITE_CONFIG DISABLED //!< Loop Measurement Setup
#define READ_CONFIG DISABLED //!< Loop Measurement Setup
#define MEASURE_CELL ENABLED //!< Loop Measurement Setup
#define MEASURE_AUX DISABLED //!< Loop Measurement Setup
#define MEASURE_STAT DISABLED //!< Loop Measurement Setup
#define PRINT_PEC DISABLED //!< Loop Measurement Setup

// ********************************** Structs **********************************

typedef struct {
	float temp;
	float voltage;
} Cell;

typedef struct {
	Cell cells[CELLS_TO_TEST];
} Chip;

typedef struct {
	cell_asic IC_config[TOTAL_IC];
	Chip chips[TOTAL_IC];
} TestAccumulator;

// ********************************** Functions **********************************

void FEB_LTC6811_Init(void);

// Read voltage
void FEB_LTC6811_Poll_Voltage(void);
void FEB_LTC6811_Start_Cell_ADC_Measurement(void);
void FEB_LTC6811_Read_Cell_Voltage_Registers(void);
void FEB_LTC6811_Store_Voltage(void);
float FEB_LTC6811_Convert_Voltage(uint16_t value);

#endif /* INC_FEB_LTC6811_H_ */



#ifndef INC_FEB_LTC6811_H_
#define INC_FEB_LTC6811_H_

// ********************************** Includes **********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "LTC6811.h"

#include "FEB_CAN.h"
#include "FEB_Constant.h"
#include "FEB_LTC6811_Temp_LUT.h"
#include "FEB_State.h"
#include "FEB_UART.h"

// ********************************** LTC6811 Configuration **********************************

#define NUM_IC	 				FEB_CONSTANT_NUM_BANKS * 2

#define ENABLED 				1
#define DISABLED 				0
#define DATALOG_ENABLED 		1
#define DATALOG_DISABLED 		0

#define ADC_OPT 				ADC_OPT_DISABLED 	//!< ADC Mode option bit
#define ADC_CONVERSION_MODE 	MD_7KHZ_3KHZ 		//!< ADC Mode
#define ADC_DCP 				DCP_ENABLED 		//!< Discharge Permitted
#define CELL_CH_TO_CONVERT 		CELL_CH_ALL 		//!< Channel Selection for ADC conversion
#define AUX_CH_TO_CONVERT 		AUX_CH_ALL 			//!< Channel Selection for ADC conversion
#define STAT_CH_TO_CONVERT 		STAT_CH_ALL 		//!< Channel Selection for ADC conversion
#define SEL_ALL_REG 			REG_ALL 			//!< Register Selection
#define SEL_REG_A 				REG_1 				//!< Register Selection
#define SEL_REG_B 				REG_2				//!< Register Selection

#define MEASUREMENT_LOOP_TIME 	500					//!< Loop Time in milliseconds(ms)

// Voltage thresholds (0.1 mV)
#define OV_THRESHOLD 			FEB_CONSTANT_CELL_MAX_VOLT * 10000 	//!< Over voltage threshold ADC Code. LSB = 0.0001 ---(4.2V)
#define UV_THRESHOLD 			FEB_CONSTANT_CELL_MIN_VOLT * 10000	//!< Under voltage threshold ADC Code. LSB = 0.0001 ---(2.5V)

// Loop measurement setup
#define WRITE_CONFIG 			DISABLED	//!< This is to ENABLED or DISABLED writing into to configuration registers in a continuous loop
#define READ_CONFIG 			DISABLED	//!< This is to ENABLED or DISABLED reading the configuration registers in a continuous loop
#define MEASURE_CELL			ENABLED 	//!< This is to ENABLED or DISABLED measuring the cell voltages in a continuous loop
#define MEASURE_AUX 			DISABLED 	//!< This is to ENABLED or DISABLED reading the auxiliary registers in a continuous loop
#define MEASURE_STAT 			DISABLED 	//!< This is to ENABLED or DISABLED reading the status registers in a continuous loop
#define PRINT_PEC 				DISABLED 	//!< This is to ENABLED or DISABLED printing the PEC Error Count in a continuous loop

// ********************************** Structs **********************************

typedef struct {
	float temperature;
	uint16_t voltage_100uV;
} FEB_LTC6811_Cell_t;

typedef struct {
	FEB_LTC6811_Cell_t cells[FEB_CONSTANT_NUM_CELLS_PER_BANK];
} FEB_LTC6811_Bank_t;

typedef struct {
	cell_asic IC_config[NUM_IC];
	FEB_LTC6811_Bank_t banks[FEB_CONSTANT_NUM_BANKS];
	float total_voltage;
} FEB_LTC6811_Accumulator_t;

// ********************************** Functions **********************************

void FEB_LTC6811_Init(void);
uint8_t FEB_LTC6811_Get_IC(uint8_t bank, uint8_t cell);
uint8_t FEB_LTC6811_Get_Cell_Id(uint8_t cell);

// Read voltage
void FEB_LTC6811_Poll_Voltage(void);
void FEB_LTC6811_Start_Cell_ADC_Measurement(void);
void FEB_LTC6811_Read_Cell_Voltage_Registers(void);
void FEB_LTC6811_Store_Voltage(void);

// Voltage interface
void FEB_LTC6811_Validate_Voltage(void);
void FEB_LTC6811_UART_Transmit_Voltage();
float FEB_LTC6811_Total_Voltage(void);

// Read temperature
void FEB_LTC6811_Poll_Temperature(void);
void FEB_LTC6811_Update_GPIO(uint8_t channel);
void FEB_LTC6811_Start_GPIO_ADC_Measurement(void);
void FEB_LTC6811_Read_Aux_Voltages(void);
void FEB_LTC6811_Store_Temperature(uint8_t channel);
float FEB_LTC6811_Convert_Temperature(uint16_t value);

// Temperature interface
void FEB_LTC6811_Validate_Temperature(void);
void FEB_LTC6811_UART_Transmit_Temperature(void);
void FEB_LTC6811_CAN_Transmit_Temperature(void);

#endif /* INC_FEB_LTC6811_H_ */

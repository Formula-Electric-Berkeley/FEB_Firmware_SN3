#ifndef INC_FEB_LTC6811_H_
#define INC_FEB_LTC6811_H_

// ******************************** Includes ********************************

#include "FEB_CAN_ID.h"
#include "FEB_Const.h"
#include "FEB_SM.h"
#include "FEB_Temp_LUT.h"

#include "cmsis_os.h"
#include "LTC6811.h"
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

// ********************************** LTC6811 Configuration **********************************
// TODO: Look through configuration

#define NUM_IC	 				FEB_CONST_NUM_BANKS * 2

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
#define OV_THRESHOLD 			FEB_CONST_CELL_MIN_VOLT_V * 10000 	//!< Over voltage threshold ADC Code. LSB = 0.0001 ---(4.2V)
#define UV_THRESHOLD 			FEB_CONST_CELL_MAX_VOLT_V * 10000		//!< Under voltage threshold ADC Code. LSB = 0.0001 ---(2.5V)

// Loop measurement setup
#define WRITE_CONFIG 			DISABLED	//!< This is to ENABLED or DISABLED writing into to configuration registers in a continuous loop
#define READ_CONFIG 			DISABLED	//!< This is to ENABLED or DISABLED reading the configuration registers in a continuous loop
#define MEASURE_CELL			ENABLED 	//!< This is to ENABLED or DISABLED measuring the cell voltages in a continuous loop
#define MEASURE_AUX 			DISABLED 	//!< This is to ENABLED or DISABLED reading the auxiliary registers in a continuous loop
#define MEASURE_STAT 			DISABLED 	//!< This is to ENABLED or DISABLED reading the status registers in a continuous loop
#define PRINT_PEC 				DISABLED 	//!< This is to ENABLED or DISABLED printing the PEC Error Count in a continuous loop

// ******************************** Functions ********************************

// Voltage functions
void FEB_LTC6811_Poll_Voltage(void);
void FEB_LTC6811_Validate_Voltage(void);
float FEB_LTC6811_Get_Total_Voltage(void);
void FEB_LTC6811_CAN_Transmit_Voltage(void);

// Balance
void FEB_LTC6811_Init_Balance(void);
void FEB_LTC6811_Balance_Process(void);
bool FEB_LTC6811_Balance_Complete(void);
void FEB_LTC6811_Stop_Balance(void);

// Temperature functions
void FEB_LTC6811_Poll_Temperature(void);
void FEB_LTC6811_Validate_Temperature(void);
void FEB_LTC6811_CAN_Transmit_Temperature(void);

#endif /* INC_FEB_LTC6811_H_ */

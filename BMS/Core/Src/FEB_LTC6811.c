// ********************************** Includes & External **********************************

#include "FEB_LTC6811.h"

extern CAN_HandleTypeDef hcan1;
extern UART_HandleTypeDef huart2;

extern uint8_t FEB_CAN_Tx_Data[8];
extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
extern uint32_t FEB_CAN_Tx_Mailbox;
extern bool FEB_State_Debug;
extern bool FEB_State_Shutdown;

// TODO: REMOVE TEST CODE
#include "stm32f4xx_hal.h"
#include <string.h>
extern UART_HandleTypeDef huart2;
// TODO: REMOVE TEST CODE

// ********************************** LTC6811 Configuration **********************************

// Cell -> Cell Id
static uint8_t FEB_LTC6811_Cell_Id_Map[17] = {9, 8, 7, 6, 4, 3, 2, 1, 0, 9, 8, 7, 6, 3, 2, 1, 0};

// Set configuration bits
static bool REFON = 1; 												//!< Reference Powered Up Bit
static bool ADCOPT = 0; 											//!< ADC Mode option bit
static bool GPIOBITS_A[5] = {1, 1, 0, 0, 0}; 						//!< GPIO Pin Control - (1, 2) ADC - (3, 4, 5) MUX Select
static uint16_t UV = UV_THRESHOLD; 									//!< Under-voltage Comparison Voltage
static uint16_t OV = OV_THRESHOLD; 									//!< Over-voltage Comparison Voltage
static bool DCCBITS_A[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 	//!< Discharge cell switch 	//Dcc 1,2,3,4,5,6,7,8,9,10,11,12
static bool DCTOBITS[4] = {1, 0, 1, 0}; 							//!< Discharge time value 	// Dcto 0,1,2,3 // Programmed for 4 min

// Set accumulator
static FEB_LTC6811_Accumulator_t FEB_LTC6811_Accumulator;

// ********************************** Functions **********************************

void FEB_LTC6811_Init(void) {
	LTC6811_init_cfg(NUM_IC, FEB_LTC6811_Accumulator.IC_config);
	for (uint8_t ic = 0; ic < NUM_IC; ic++) {
		LTC6811_set_cfgr(ic, FEB_LTC6811_Accumulator.IC_config, REFON, ADCOPT, GPIOBITS_A, DCCBITS_A, DCTOBITS, UV, OV);
	}
	LTC6811_reset_crc_count(NUM_IC, FEB_LTC6811_Accumulator.IC_config);
	LTC6811_init_reg_limits(NUM_IC, FEB_LTC6811_Accumulator.IC_config);
}

uint8_t FEB_LTC6811_Get_IC(uint8_t bank, uint8_t cell) {
	if (cell < 9) {
		return bank * 2 + 1;
	} else {
		return bank * 2;
	}
}

uint8_t FEB_LTC6811_Get_Cell_Id(uint8_t cell) {
	return FEB_LTC6811_Cell_Id_Map[cell];
}

// ******************** Read Voltage ********************

void FEB_LTC6811_Poll_Voltage(void) {
	FEB_LTC6811_Start_Cell_ADC_Measurement();
	FEB_LTC6811_Read_Cell_Voltage_Registers();
	FEB_LTC6811_Store_Voltage();
}

void FEB_LTC6811_Start_Cell_ADC_Measurement(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();
}

void FEB_LTC6811_Read_Cell_Voltage_Registers(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_rdcv(SEL_ALL_REG, NUM_IC, FEB_LTC6811_Accumulator.IC_config);
}

void FEB_LTC6811_Store_Voltage(void) {
	FEB_LTC6811_Accumulator.total_voltage = 0;
    for (uint16_t bank = 0; bank < FEB_CONSTANT_NUM_BANKS; bank++) {
    	for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS_PER_BANK; cell++) {
    		uint8_t ic = FEB_LTC6811_Get_IC(bank, cell);
    		uint8_t cell_id = FEB_LTC6811_Get_Cell_Id(cell);
    		FEB_LTC6811_Accumulator.banks[bank].cells[cell].voltage_100uV = FEB_LTC6811_Accumulator.IC_config[ic].cells.c_codes[cell_id];
    		FEB_LTC6811_Accumulator.total_voltage += FEB_LTC6811_Accumulator.IC_config[ic].cells.c_codes[cell_id] * 0.0001;
    	}
    }
}

// ******************** Voltage Interface ********************

void FEB_LTC6811_Validate_Voltage(void) {
	if (FEB_State_Shutdown) {
		return;
	}
	uint16_t min_voltage_100uV = FEB_CONSTANT_CELL_MIN_VOLT * 10000;
	uint16_t max_voltage_100uV = FEB_CONSTANT_CELL_MAX_VOLT * 10000;
	for (uint8_t bank = 0; bank < FEB_CONSTANT_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS_PER_BANK; cell++) {
			uint16_t voltage_100uV = FEB_LTC6811_Accumulator.banks[bank].cells[cell].voltage_100uV;
			if (voltage_100uV < min_voltage_100uV) {
				FEB_State_Set_Shutdown("[FEB_LTC6811_Validate_Voltage] Cell under voltage.");
			} else if (voltage_100uV > max_voltage_100uV) {
				FEB_State_Set_Shutdown("[FEB_LTC6811_Validate_Voltage] Cell over voltage.");
			}
		}
	}
}

void FEB_LTC6811_UART_Transmit_Voltage() {
	if (!FEB_State_Debug) {
		return;
	}
	char UART_str[1024];
	char temp_str[256];
	for (uint8_t bank = 0; bank < FEB_CONSTANT_NUM_BANKS; bank++) {
		sprintf(UART_str, "%" PRIu8 " %" PRIu8, FEB_UART_Cell_Voltage_ID, bank);
		for (uint16_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS_PER_BANK; cell++) {
			sprintf(temp_str, " %" PRIu16, FEB_LTC6811_Accumulator.banks[bank].cells[cell].voltage_100uV);
			strncat(UART_str, temp_str, strlen(temp_str));
		}
		sprintf(temp_str, "\n");
		strncat(UART_str, temp_str, strlen(temp_str));
		HAL_UART_Transmit_IT(&huart2, (uint8_t*) UART_str, strlen(UART_str));
	}
}

float FEB_LTC6811_Total_Voltage(void) {
	return FEB_LTC6811_Accumulator.total_voltage;
}

// ******************** Read Temperature ********************

void FEB_LTC6811_Poll_Temperature(void) {
	for (uint8_t channel = 0; channel <= 4; channel++) {
		FEB_LTC6811_Update_GPIO(channel);
		FEB_LTC6811_Start_GPIO_ADC_Measurement();
		FEB_LTC6811_Read_Aux_Voltages();
//		FEB_LTC6811_Store_Temperature(channel);
	}
}

void FEB_LTC6811_Update_GPIO(uint8_t channel) {
	GPIOBITS_A[0] = 0b1;					// ADC
	GPIOBITS_A[1] = 0b1;					// ADC
	GPIOBITS_A[2] = (channel >> 0) & 0b1;	// MUX Select
	GPIOBITS_A[3] = (channel >> 1) & 0b1;	// MUX Select
	GPIOBITS_A[4] = (channel >> 2) & 0b1;	// MUX Select

    wakeup_sleep(NUM_IC);
    for (uint8_t ic = 0; ic < NUM_IC; ic++) {
		LTC6811_set_cfgr(ic, FEB_LTC6811_Accumulator.IC_config, REFON, ADCOPT, GPIOBITS_A, DCCBITS_A, DCTOBITS, UV, OV);
    }
    wakeup_idle(NUM_IC);
    LTC6811_wrcfg(NUM_IC, FEB_LTC6811_Accumulator.IC_config);
}

void FEB_LTC6811_Start_GPIO_ADC_Measurement(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
	LTC6811_pollAdc();
}

void FEB_LTC6811_Read_Aux_Voltages(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_rdaux(SEL_ALL_REG, NUM_IC, FEB_LTC6811_Accumulator.IC_config);
}

void FEB_LTC6811_Store_Temperature(uint8_t channel) {
    for (uint16_t bank = 0; bank < FEB_CONSTANT_NUM_BANKS; bank++) {
    	switch (channel) {
    		case 0:
    			FEB_LTC6811_Accumulator.banks[bank].cells[12].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[16].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[1]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[4].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[8].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[1]);
    			break;
    		case 1:
    			FEB_LTC6811_Accumulator.banks[bank].cells[11].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[15].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[1]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[3].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[7].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[1]);
    			break;
    		case 2:
    			FEB_LTC6811_Accumulator.banks[bank].cells[10].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[14].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[1]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[2].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[6].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[1]);
    			break;
    		case 3:
    			FEB_LTC6811_Accumulator.banks[bank].cells[9].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[13].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2].aux.a_codes[1]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[1].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[0]);
    			FEB_LTC6811_Accumulator.banks[bank].cells[5].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[1]);
    			break;
    		case 4:
    			FEB_LTC6811_Accumulator.banks[bank].cells[0].temperature = FEB_LTC6811_Convert_Temperature(FEB_LTC6811_Accumulator.IC_config[bank * 2 + 1].aux.a_codes[0]);
    			break;
    	}
    }
}


//float FEB_LTC6811_Convert_Temperature(uint16_t voltage_100uV) {
float FEB_LTC6811_Convert_Temperature(uint16_t value) {
	if (value == 65535) {
		return -42;
	}
	return FEB_LTC6811_Temp_LUT_Get_Temperature(value * 0.0001);
}

// ******************** Temperature Interface ********************

void FEB_LTC6811_Validate_Temperature(void) {
	if (FEB_State_Shutdown) {
		return;
	}
	for (uint8_t bank = 0; bank < FEB_CONSTANT_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS_PER_BANK; cell++) {
			float temperature = FEB_LTC6811_Accumulator.banks[bank].cells[cell].temperature;
			if (fabs(temperature - -42.0f) < 0.1f || fabs(temperature  - -41.0f) < 0.1f) {
			  // TK: override error code, might want to disable this after fixing the segments
			  continue;
			}
			if (temperature < FEB_CONSTANT_CELL_MIN_TEMP) {
				FEB_State_Set_Shutdown("[FEB_LTC6811_Validate_Temperature] Cell under temperature.");
			} else if (temperature > FEB_CONSTANT_CELL_MAX_TEMP) {
				FEB_State_Set_Shutdown("[FEB_LTC6811_Validate_Temperature] Cell over temperature.");
			}
		}
	}
}

void FEB_LTC6811_UART_Transmit_Temperature(void) {
	if (!FEB_State_Debug) {
		return;
	}
	char UART_Str[1024];
	char temp_str[256];
	for (uint8_t bank = 0; bank < FEB_CONSTANT_NUM_BANKS; bank++) {
		sprintf(UART_Str, "%" PRIu8 " %" PRIu8, FEB_UART_Cell_Temperature_ID, bank);
		for (uint16_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS_PER_BANK; cell++) {
			float temperature = FEB_LTC6811_Accumulator.banks[bank].cells[cell].temperature;
			sprintf(temp_str, " %f", temperature);
			strncat(UART_Str, temp_str, strlen(temp_str));
		}
		sprintf(temp_str, "\n");
		strncat(UART_Str, temp_str, strlen(temp_str));
		HAL_UART_Transmit_IT(&huart2, (uint8_t*) UART_Str, strlen(UART_Str));
	}
}

void FEB_LTC6811_CAN_Transmit_Temperature(void) {
	for (uint8_t bank = 0; bank < FEB_CONSTANT_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONSTANT_NUM_CELLS_PER_BANK; cell++) {
			uint16_t temperature_100mC = (uint16_t) FEB_LTC6811_Accumulator.banks[bank].cells[cell].temperature * 10;

			// Initialize transmission header
			FEB_CAN_Tx_Header.DLC = 4;
			FEB_CAN_Tx_Header.StdId = FEB_CAN_ID_BMS_TEMPERATURE;
			FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
			FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
			FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

			// Configure data
			FEB_CAN_Tx_Data[0] = bank;
			FEB_CAN_Tx_Data[0] = cell;
			FEB_CAN_Tx_Data[0] = temperature_100mC >> 8;
			FEB_CAN_Tx_Data[0] = temperature_100mC && 0xFF;

			// Delay until mailbox available
			while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

			// Add Tx data to mailbox
			if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK && !FEB_State_Shutdown) {
				FEB_State_Set_Shutdown("[FEB_LTC6811_CAN_Transmit_Temperature] Unable to transmit CAN message.");
			}
		}
	}
}

#include "FEB_LTC6811.h"
#include "FEB_Config.h"
#include "FEB_Temp_LUT.h"
#include "FEB_SM.h"
#include "LTC6811.h"
#include "LTC681x.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "cmsis_os2.h"

extern osMutexId_t FEB_LTC6811_LockHandle;	// Handles synchronization
extern UART_HandleTypeDef huart2;

/* ******** Accumulator Mapping ******** */

/*
Accumulator Cell (- terminal)			00	01	02	03	04	05	06	07	08	09	10	11	12	13	14	15	16	17	18	19
IC [0 (- terminal), 1 (+ terminal)]		0	0	0	0	0	0	0	0	0	0	1	1	1	1	1	1	1	1	1	1
LTC6811 Cell							0	1	2	3	4	6	7	8	9	10	0	1	2	3	4	6	7	8	9	10
Multiplexer								B	B	B	B	B	A	A	A	A	A	B	B	B	B	B	A	A	A	A	A
Channel									0	1	2	3	4	0	1	2	3	4	0	1	2	3	4	0	1	2	3	4
*/

/* ******** LTC6811 Configuration ******** */

#define NUM_IC	 				(FEB_CONFIG_NUM_BANKS * 2)

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
#define OV_THRESHOLD 			FEB_CONFIG_CELL_MIN_VOLTAGE_100uV 	//!< Over voltage threshold ADC Code. LSB = 0.0001 ---(4.2V)
#define UV_THRESHOLD 			FEB_CONFIG_CELL_MAX_VOLTAGE_100uV	//!< Under voltage threshold ADC Code. LSB = 0.0001 ---(2.5V)

// Loop measurement setup
#define WRITE_CONFIG 			DISABLED	//!< This is to ENABLED or DISABLED writing into to configuration registers in a continuous loop
#define READ_CONFIG 			DISABLED	//!< This is to ENABLED or DISABLED reading the configuration registers in a continuous loop
#define MEASURE_CELL			ENABLED 	//!< This is to ENABLED or DISABLED measuring the cell voltages in a continuous loop
#define MEASURE_AUX 			DISABLED 	//!< This is to ENABLED or DISABLED reading the auxiliary registers in a continuous loop
#define MEASURE_STAT 			DISABLED 	//!< This is to ENABLED or DISABLED reading the status registers in a continuous loop
#define PRINT_PEC 				DISABLED 	//!< This is to ENABLED or DISABLED printing the PEC Error Count in a continuous loop

static bool REFON = 1; 												//!< Reference Powered Up Bit
static bool ADCOPT = 0; 											//!< ADC Mode option bit
static bool GPIOBITS_A[5] = {1, 1, 0, 0, 0}; 						//!< GPIO Pin Control // (1, 2) ADC // (3, 4, 5) MUX Select
static uint16_t UV = UV_THRESHOLD; 									//!< Under-voltage Comparison Voltage
static uint16_t OV = OV_THRESHOLD; 									//!< Over-voltage Comparison Voltage
static bool DCCBITS_A[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 	//!< Discharge cell switch 	// DCC 1,2,3,4,5,6,7,8,9,10,11,12
static bool DCTOBITS[4] = {1, 0, 0, 0}; 							//!< Discharge time value // DCTO 0, 1, 2, 3 // Programmed for 30s

typedef struct {
	uint16_t voltage_mV;		// Mili-volts
	int16_t temperature_dC;		// Deci-celsius
	bool voltage_fault;			// Under/over voltage
	bool temperature_fault;		// Under/over temperature
	bool balance;				// Cell balance
} cell_t;
typedef struct {
	cell_t cells[FEB_CONFIG_NUM_BANKS][FEB_CONFIG_NUM_CELLS_PER_BANK];
	uint32_t total_voltage_mV;				// Mili-volts
	uint16_t balance_target_voltage_mV;		// Mili-volts
	bool balance_active;					// Used to toggle balance while in balance state
} accumulator_t;
static accumulator_t accumulator;
static cell_asic IC_config[NUM_IC];

/* ******** Static Function Declaration ******** */

static void init_all_IC(void);
static uint8_t get_IC(uint8_t bank, uint8_t accumulator_cell);
static uint8_t get_IC_cell(uint8_t accumulator_cell);
static uint8_t get_mux(uint8_t accumulator_cell);

// Voltage
static void start_voltage_ADC_measurement(void);
static void read_voltage_registers(void);
static void store_voltage_values(void);

// Balance
static void set_target_voltage(void);
static void balance_all_cells(void);
static bool check_balance_complete(void);

// Temperature
static void set_GPIO_bits(uint8_t channel);
static void start_GPIO_ADC_measurement(void);
static void read_ADC_voltage_registers(void);
static void store_temperature_values(uint8_t channel);

/* ******** Startup and Helper Functions ******** */

void FEB_LTC6811_Init(void) {
	init_all_IC();
	accumulator.total_voltage_mV = FEB_CONFIG_NUM_BANKS * FEB_CONFIG_NUM_CELLS_PER_BANK
		* FEB_CONFIG_CELL_MAX_VOLTAGE_100uV * 1e-1;
}

static void init_all_IC(void) {
	LTC6811_init_cfg(NUM_IC, IC_config);
	for (uint8_t current_ic = 0; current_ic < NUM_IC; current_ic++) {
		LTC6811_set_cfgr(current_ic, IC_config, REFON, ADCOPT, GPIOBITS_A, DCCBITS_A, DCTOBITS, UV, OV);
	}
	LTC6811_reset_crc_count(NUM_IC, IC_config);
	LTC6811_init_reg_limits(NUM_IC, IC_config);
}

/* Function returns the IC number that belongs to the pair (bank, accumulator cell). */
static uint8_t get_IC(uint8_t bank, uint8_t accumulator_cell) {
	if (accumulator_cell < FEB_CONFIG_NUM_ACC_CELLS_PER_IC) {
		// Accumulator cell 0-9
		return bank * 2;
	} else {
		// Accumulator cell 10-19
		return bank * 2 + 1;
	}
}

/* Function returns bank associated with IC. */
static uint8_t get_bank(uint8_t IC) {
	return IC >> 1;
}

/* Function returns the IC cell that corresponds to an accumulator cell. */
static uint8_t get_IC_cell(uint8_t accumulator_cell) {
	static uint8_t IC_cell_map[FEB_CONFIG_NUM_ACC_CELLS_PER_IC] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10};
	return IC_cell_map[accumulator_cell % FEB_CONFIG_NUM_ACC_CELLS_PER_IC];
}

/* Function maps IC pin [0, 11] + IC -> accumulator cell [0, 19], -1 if no mapping exists. */
static int8_t get_accumulator_cell(uint8_t IC, uint8_t LTC6811_cell) {
	static uint8_t acc_cell_map[FEB_CONFIG_NUM_PINS_PER_IC] = {0, 1, 2, 3, 4, -1, 5, 6, 7, 8, 9, -1};
	return acc_cell_map[LTC6811_cell] + FEB_CONFIG_NUM_ACC_CELLS_PER_IC * (IC % 2);
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

// Return multiplexer channel 0-4
static uint8_t get_channel(uint8_t accumulator_cell) {
	return accumulator_cell % 5;
}

uint32_t FEB_LTC6811_Get_Total_Voltage(void) {
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	uint32_t total_voltage_mV = accumulator.total_voltage_mV;
	osMutexRelease(FEB_LTC6811_LockHandle);
	return total_voltage_mV;
}

void FEB_LTC6811_UART_Transmit(void) {
	static char str[64];
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			// Get cell data
			while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
			uint16_t cell_voltage_mV = accumulator.cells[bank][cell].voltage_mV;
			int16_t cell_temperature_dC = accumulator.cells[bank][cell].temperature_dC;
			bool cell_voltage_fault = accumulator.cells[bank][cell].voltage_fault;
			bool cell_temperature_fault = accumulator.cells[bank][cell].temperature_fault;
			bool balance = accumulator.cells[bank][cell].balance && accumulator.balance_active;
			osMutexRelease(FEB_LTC6811_LockHandle);

			// Transmit cell data
			sprintf(str, "cell %d %d %d %d %d %d %d\n",
					bank + 1, cell + 1,
					cell_voltage_mV, cell_temperature_dC,
					cell_voltage_fault, cell_temperature_fault,
					balance);
			HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
		}
	}

	// Transmit total pack voltage
	sprintf(str, "total-voltage %ld\n", FEB_LTC6811_Get_Total_Voltage());
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
}

/* ******** Voltage Functions ******** */

void FEB_LTC6811_Poll_Voltage(void) {
	start_voltage_ADC_measurement();
	read_voltage_registers();
	store_voltage_values();
}

static void start_voltage_ADC_measurement(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();
}

static void read_voltage_registers(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_rdcv(SEL_ALL_REG, NUM_IC, IC_config);
}

static void store_voltage_values(void) {
//	FEB_SM_ST_t state = FEB_SM_Get_Current_State();

	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	uint32_t total_voltage_mV = 0;
//	bool balance_complete = true;

	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			// Store voltage
			uint16_t voltage_mV = IC_config[get_IC(bank, cell)].cells.c_codes[get_IC_cell(cell)] * 1e-1;
			accumulator.cells[bank][cell].voltage_mV = voltage_mV;
			total_voltage_mV += voltage_mV;

			// Check under/over voltage
			uint16_t min_voltage_mV = FEB_Config_Get_Cell_Min_Voltage_mV();
			uint16_t max_voltage_mV = FEB_Config_Get_Cell_Max_Voltage_mV();
			accumulator.cells[bank][cell].voltage_fault =
				voltage_mV < min_voltage_mV || voltage_mV > max_voltage_mV;

			// Check balance voltage
//			if (state == FEB_SM_ST_BALANCE && accumulator.cells[bank][cell].balance &&
//				!accumulator.balance_active && voltage_mV < accumulator.balance_target_voltage_mV) {
//				accumulator.cells[bank][cell].balance = false;
//			}
//			if (accumulator.cells[bank][cell].balance) {
//				balance_complete = false;
//			}
		}
	}
	accumulator.total_voltage_mV = total_voltage_mV;
	osMutexRelease(FEB_LTC6811_LockHandle);

//	// Stop balance if complete
//	if (balance_complete) {
//		FEB_SM_Transition(FEB_SM_ST_STANDBY);
//	}
}

/* ******** Balance Functions ******** */

void FEB_LTC6811_Init_Balance(void) {
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	set_target_voltage();
	balance_all_cells();
	osMutexRelease(FEB_LTC6811_LockHandle);
}

static void set_target_voltage(void) {
	uint16_t min_voltage = FEB_CONFIG_CELL_MAX_VOLTAGE_mV;
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			uint16_t cell_voltage = accumulator.cells[bank][cell].voltage_mV;
			if (cell_voltage < min_voltage) {
				min_voltage = cell_voltage;
			}
		}
	}
	accumulator.balance_target_voltage_mV = 3300;
}

static void balance_all_cells(void) {
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			accumulator.cells[bank][cell].balance = true;
		}
	}
}

void FEB_LTC6811_Balance_Process(void) {
	if (FEB_SM_Get_Current_State() == FEB_SM_ST_BALANCE) {
		while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
		accumulator.balance_active = true;
		osMutexRelease(FEB_LTC6811_LockHandle);

		osDelay(30000);

		while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
		accumulator.balance_active = false;
		osMutexRelease(FEB_LTC6811_LockHandle);

		osDelay(4000);

		if (check_balance_complete())
			FEB_SM_Transition(FEB_SM_ST_STANDBY);
	}
}

static bool check_balance_complete(void) {
	bool complete = true;
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			if (accumulator.cells[bank][cell].balance)
				complete = false;
			if (accumulator.cells[bank][cell].balance &&
				accumulator.cells[bank][cell].voltage_mV < accumulator.balance_target_voltage_mV) {
				accumulator.cells[bank][cell].balance = false;
			}
		}
	}
	osMutexRelease(FEB_LTC6811_LockHandle);
	return complete;
}

//
//
//	uint32_t total_voltage_mV = 0;
////	bool balance_complete = true;
//
//	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
//			// Store voltage
//			uint16_t voltage_mV = IC_config[get_IC(bank, cell)].cells.c_codes[get_IC_cell(cell)] * 1e-1;
//			accumulator.cells[bank][cell].voltage_mV = voltage_mV;
//			total_voltage_mV += voltage_mV;
//
//			// Check under/over voltage
//			uint16_t min_voltage_mV = FEB_Config_Get_Cell_Min_Voltage_mV();
//			uint16_t max_voltage_mV = FEB_Config_Get_Cell_Max_Voltage_mV();
//			accumulator.cells[bank][cell].voltage_fault =
//				voltage_mV < min_voltage_mV || voltage_mV > max_voltage_mV;
//
//			// Check balance voltage
////			if (state == FEB_SM_ST_BALANCE && accumulator.cells[bank][cell].balance &&
////				!accumulator.balance_active && voltage_mV < accumulator.balance_target_voltage_mV) {
////				accumulator.cells[bank][cell].balance = false;
////			}
////			if (accumulator.cells[bank][cell].balance) {
////				balance_complete = false;
////			}
//		}
//	}

//}

static void configure_DCCBITS_A(uint8_t IC) {
	FEB_SM_ST_t state = FEB_SM_Get_Current_State();
	bool accumulator_balance = accumulator.balance_active;

	uint8_t bank = get_bank(IC);
	for (uint8_t IC_pin = 0; IC_pin < FEB_CONFIG_NUM_PINS_PER_IC; IC_pin++) {
		int8_t accumulator_cell = get_accumulator_cell(IC, IC_pin);
		if (state != FEB_SM_ST_BALANCE || !accumulator_balance || accumulator_cell == -1) {
			DCCBITS_A[IC_pin] = false;
		} else {
			DCCBITS_A[IC_pin] = accumulator.cells[bank][accumulator_cell].balance;
		}
	}
}

void FEB_LTC6811_Stop_Balance(void) {
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	accumulator.balance_active = false;
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			accumulator.cells[bank][cell].balance = false;
		}
	}
	osMutexRelease(FEB_LTC6811_LockHandle);
}

//static void configure_DCCBITS_A(uint8_t IC) {
//	uint8_t bank = get_bank(IC);
//	for (uint8_t LTC6811_cell = 0; LTC6811_cell < FEB_CONST_NUM_LTC_CELLS_PER_IC; LTC6811_cell++) {
//		int8_t acc_cell = get_accumulator_cell(IC, LTC6811_cell);
//		if (acc_cell == -1 ||
//			accumulator.balance == false ||
//			FEB_SM_Get_Current_State() != FEB_SM_ST_BALANCE) {
//			DCCBITS_A[LTC6811_cell] = false;
//		} else {
//			DCCBITS_A[LTC6811_cell] = accumulator.banks[bank].cells[acc_cell].balance;
//		}
//	}



//void FEB_LTC6811_Init_Balance(void) {
//	if (FEB_SM_Get_Current_State() == FEB_SM_ST_BALANCE) {
//		set_target_voltage();
//		balance_all_cells();
//	}
//}
//
//void FEB_LTC6811_Balance_Process(void) {
//	if (FEB_SM_Get_Current_State() == FEB_SM_ST_BALANCE) {
//		balance();
//		osDelay(30000);
//		FEB_LTC6811_Stop_Balance();
//		osDelay(1000);
//		if (balance_complete()) {
//			FEB_SM_Set_Current_State(FEB_SM_ST_PRECHARGE);
//		}
//	}
//}
//
//static bool balance_complete(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			if (accumulator.banks[bank].cells[cell].balance) {
//				return false;
//			}
//		}
//	}
//	return true;
//}
//
//static void balance(void) {
//	accumulator.balance = true;
//}
//
//void FEB_LTC6811_Stop_Balance(void) {
//	accumulator.balance = false;
//}
//
//static void balance_all_cells(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			accumulator.banks[bank].cells[cell].balance = true;
//		}
//	}
//}
//
//static void set_target_voltage(void) {
//	accumulator.target_voltage_100uV = accumulator.banks[0].cells[0].volt_100uV;
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t temp_voltage_100uV = accumulator.banks[bank].cells[cell].volt_100uV;
//			if (temp_voltage_100uV < accumulator.target_voltage_100uV) {
//				accumulator.target_voltage_100uV = temp_voltage_100uV;
//			}
//		}
//	}
//}
//
//static void configure_DCCBITS_A(uint8_t IC) {
//	uint8_t bank = get_bank(IC);
//	for (uint8_t LTC6811_cell = 0; LTC6811_cell < FEB_CONST_NUM_LTC_CELLS_PER_IC; LTC6811_cell++) {
//		int8_t acc_cell = get_accumulator_cell(IC, LTC6811_cell);
//		if (acc_cell == -1 ||
//			accumulator.balance == false ||
//			FEB_SM_Get_Current_State() != FEB_SM_ST_BALANCE) {
//			DCCBITS_A[LTC6811_cell] = false;
//		} else {
//			DCCBITS_A[LTC6811_cell] = accumulator.banks[bank].cells[acc_cell].balance;
//		}
//	}
//}

/* ******** Temperature Functions ******** */

void FEB_LTC6811_Poll_Temperature(void) {
	for (uint8_t channel = 0; channel < FEB_CONFIG_NUM_TEMP_MUX_CH; channel++) {
		set_GPIO_bits(channel);
		start_GPIO_ADC_measurement();
		read_ADC_voltage_registers();
		store_temperature_values(channel);
	}
}

static void set_GPIO_bits(uint8_t channel) {
	GPIOBITS_A[0] = 0b1;					// ADC - Multiplexer A
	GPIOBITS_A[1] = 0b1;					// ADC - Multiplexer B
	GPIOBITS_A[2] = (channel >> 0) & 0b1;	// MUX Select
	GPIOBITS_A[3] = (channel >> 1) & 0b1;	// MUX Select
	GPIOBITS_A[4] = (channel >> 2) & 0b1;	// MUX Select

    for (uint8_t IC = 0; IC < NUM_IC; IC++) {
    	configure_DCCBITS_A(IC);	// Set balance register bits
		LTC6811_set_cfgr(IC, IC_config, REFON, ADCOPT, GPIOBITS_A, DCCBITS_A, DCTOBITS, UV, OV);
    }
    wakeup_idle(NUM_IC);
    LTC6811_wrcfg(NUM_IC, IC_config);
}

static void start_GPIO_ADC_measurement(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
	LTC6811_pollAdc();
}

static void read_ADC_voltage_registers(void) {
	wakeup_sleep(NUM_IC);
	LTC6811_rdaux(SEL_ALL_REG, NUM_IC, IC_config);
}

static void store_temperature_values(uint8_t channel) {
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			if (get_channel(cell) == channel) {
				uint8_t IC = get_IC(bank, cell);
				uint8_t mux = get_mux(cell);
				uint16_t voltage_100uV = IC_config[IC].aux.a_codes[mux];

				// Store temperature
				while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
				int16_t temperature_dC = FEB_Temp_LUT_Get_Temp_100mC(voltage_100uV * 1e-1);
				accumulator.cells[bank][cell].temperature_dC = temperature_dC;

				// Check under/over temperature
				int16_t min_temperature_dC = FEB_Config_Get_Cell_Min_Temperature_dC();
				int16_t max_temperature_dC = FEB_Config_Get_Cell_Max_Temperature_dC();
				accumulator.cells[bank][cell].temperature_fault =
					temperature_dC < min_temperature_dC || temperature_dC > max_temperature_dC;
				osMutexRelease(FEB_LTC6811_LockHandle);
			}
		}
	}
}



//static void store_voltage_values(void) {
//	total_voltage
//	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t voltage_mV = IC_config[get_IC(bank, cell)].cells.c_codes[get_IC_cell(cell)];
//			accumulator.cells[bank][cell].voltage_mV = voltage_mV;
//
//			char UART_Str[64];
//			sprintf(UART_Str, "cell %d %d %d 0\n", bank + 1, cell + 1, (uint16_t) (volt_100uV * 1e-1));
//			HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//
//			total_voltate
////			accumulator.total_volt_V += (float) volt_100uV * 0.0001;
//		}
//	}
//}




//void FEB_LTC6811_Poll_Voltage(void) {
//	start_voltage_ADC_measurement();
//	read_voltage_registers();
//	store_voltage_values();
//}
//
//static void start_voltage_ADC_measurement(void) {
//	wakeup_sleep(NUM_IC);
//	LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
//	LTC6811_pollAdc();
//}
//
//static void read_voltage_registers(void) {
//	wakeup_sleep(NUM_IC);
//	LTC6811_rdcv(SEL_ALL_REG, NUM_IC, IC_config);
//}

//static void store_voltage_values(void) {
////	uint32_t total_voltage_mV = 0;
//	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t voltage_mV = IC_config[get_IC(bank, cell)].cells.c_codes[get_IC_cell(cell)];
//			accumulator.cells[bank][cell].voltage_mV = voltage_mV;
//
//			static char str[32];
//			sprintf(str, "cell %d %d %d\n", bank, cell, voltage_mV);
//			HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
//		}
//	}
//}





//static void store_voltage_values(void) {
//	accumulator.total_volt_V = 0;
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t volt_100uV = IC_config[get_IC(bank, cell)].cells.c_codes[get_LTC6811_cell(cell)];
//			accumulator.banks[bank].cells[cell].volt_100uV = volt_100uV;
//
//			char UART_Str[64];
//			sprintf(UART_Str, "Bank %d, Cell %d: %d\n", bank, cell, volt_100uV);
////			HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//
//			accumulator.total_volt_V += (float) volt_100uV * 0.0001;
//		}
//	}
//
//	for (uint8_t i = 0 ; i < 12; i++) {
//		uint16_t volt_100uV = IC_config[0].cells.c_codes[i];
//		char UART_Str[64];
//		sprintf(UART_Str, "IC 0, Pin: %d: %d\n", i, volt_100uV);
////		HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//	}
//
//	for (uint8_t i = 0 ; i < 12; i++) {
//		uint16_t volt_100uV = IC_config[1].cells.c_codes[i];
//		char UART_Str[64];
//		sprintf(UART_Str, "IC 1, Pin: %d: %d\n", i, volt_100uV);
////		HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//	}







// ******************************** Static Function Declaration ********************************
//
//// Helper
//static void init_all_IC(void);
//static uint8_t get_IC(uint8_t bank, uint8_t accumulator_cell);
//static uint8_t get_LTC6811_cell(uint8_t accumulator_cell);
//static uint8_t get_bank(uint8_t IC);
//static int8_t get_accumulator_cell(uint8_t IC, uint8_t LTC6811_cell);
//static uint8_t get_channel(uint8_t accumulator_cell);
//static uint8_t get_mux(uint8_t accumulator_cell);
//static bool valid_temp_sensor(uint8_t bank, uint8_t accumulator_cell);
//
//// Voltage
//static void start_voltage_ADC_measurement(void);
//static void read_voltage_registers(void);
//static void store_voltage_values(void);
//
//// Temperature
//static void set_GPIO_bits(uint8_t channel);
//static void start_GPIO_ADC_measurement(void);
//static void read_ADC_voltage_registers(void);
//static void store_temperature_values(uint8_t channel);
//
//// Balance
//static bool balance_complete(void);
//static void balance(void);
//static void balance_all_cells(void);
//static void set_target_voltage(void);
//static void configure_DCCBITS_A(uint8_t IC);

// ******************************** LTC6811 Configuration ********************************
// TODO: Look through configuration bits

// Set configuration bits




// ******************************** Helper Functions ********************************
//
//void FEB_LTC6811_Init(void) {
//	init_all_IC();
////	accumulator.total_volt_V = 600;
//}
//
//static void init_all_IC(void) {
//	// TODO: Validate functions
//	LTC6811_init_cfg(NUM_IC, IC_config);
//	for (uint8_t ic = 0; ic < NUM_IC; ic++) {
//		LTC6811_set_cfgr(ic, IC_config, REFON, ADCOPT, GPIOBITS_A, DCCBITS_A, DCTOBITS, UV, OV);
//	}
//	LTC6811_reset_crc_count(NUM_IC, IC_config);
//	LTC6811_init_reg_limits(NUM_IC, IC_config);
//}
//
//static uint8_t get_IC(uint8_t bank, uint8_t accumulator_cell) {
//	if (accumulator_cell < FEB_CONFIG_NUM_ACC_CELLS_PER_IC) {
//		// Accumulator cell 0-9
//		return bank * 2;
//	} else {
//		// Accumulator cell 10-19
//		return bank * 2 + 1;
//	}
//}
////
////// Function maps accumulator cell -> LTC6811 cell
//static uint8_t get_LTC6811_cell(uint8_t accumulator_cell) {
//	// Map for each IC
//	static uint8_t LTC6811_cell_map[FEB_CONFIG_NUM_ACC_CELLS_PER_IC] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10};
//	return LTC6811_cell_map[accumulator_cell % FEB_CONFIG_NUM_ACC_CELLS_PER_IC];
//}
//
//static uint8_t get_bank(uint8_t IC) {
//	return IC >> 1;
//}
//
//// Function maps LTC6811 cell [0, 11] + IC -> accumulator cell [0, 19], -1 if no mapping
//static int8_t get_accumulator_cell(uint8_t IC, uint8_t LTC6811_cell) {
//	static uint8_t acc_cell_map[FEB_CONST_NUM_LTC_CELLS_PER_IC] = {0, 1, 2, 3, 4, -1, 5, 6, 7, 8, 9, -1};
//	return acc_cell_map[LTC6811_cell] + FEB_CONST_NUM_ACC_CELLS_PER_IC * (IC % 2);
//}
//
//// Return multiplexer channel 0-4
//static uint8_t get_channel(uint8_t accumulator_cell) {
//	return accumulator_cell % 5;
//}
//
//// Return multiplexer 0 (A), 1 (B)
//static uint8_t get_mux(uint8_t accumulator_cell) {
//	if ((0 <= accumulator_cell && accumulator_cell <= 4) ||
//		(10 <= accumulator_cell && accumulator_cell <= 14)) {
//		// Multiplexer B: accumulator cell 0-4, 10-14
//		return 1;
//	} else {
//		// Multiplexer A: accumulator cell 5-9, 15-19
//		return 0;
//	}
//}
//
//static bool valid_temp_sensor(uint8_t bank, uint8_t accumulator_cell) {
//	// TODO: Test temperature sensors, determine invalid sensors
//	return true;
//}
//
//// ******************************** Voltage Functions ********************************
//
//void FEB_LTC6811_Poll_Voltage(void) {
//	// TODO: Validate functions
//	start_voltage_ADC_measurement();
//	read_voltage_registers();
//	store_voltage_values();
//}
//
//static void start_voltage_ADC_measurement(void) {
//	// TODO: Validate functions
//	wakeup_sleep(NUM_IC);
//	LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
//	LTC6811_pollAdc();
//}
//
//static void read_voltage_registers(void) {
//	// TODO: Validate functions
//	wakeup_sleep(NUM_IC);
//	LTC6811_rdcv(SEL_ALL_REG, NUM_IC, IC_config);
//}

//static void store_voltage_values(void) {
////	accumulator.total_volt_V = 0;
//	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t volt_100uV = IC_config[get_IC(bank, cell)].cells.c_codes[get_LTC6811_cell(cell)];
////			accumulator.banks[bank].cells[cell].volt_100uV = volt_100uV;
//
//			char UART_Str[64];
//			sprintf(UART_Str, "cell %d %d %d 0\n", bank + 1, cell + 1, (uint16_t) (volt_100uV * 1e-1));
//			HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//
////			accumulator.total_volt_V += (float) volt_100uV * 0.0001;
//		}
//	}
//}
//
//	for (uint8_t i = 0 ; i < 12; i++) {
//		uint16_t volt_100uV = IC_config[0].cells.c_codes[i];
//		char UART_Str[64];
//		sprintf(UART_Str, "IC 0, Pin: %d: %d\n", i, volt_100uV);
////		HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//	}
//
//	for (uint8_t i = 0 ; i < 12; i++) {
//		uint16_t volt_100uV = IC_config[1].cells.c_codes[i];
//		char UART_Str[64];
//		sprintf(UART_Str, "IC 1, Pin: %d: %d\n", i, volt_100uV);
////		HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//	}
//}
//
//// Check cell voltage range and balance status
//void FEB_LTC6811_Validate_Voltage(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t volt_100uV = accumulator.banks[bank].cells[cell].volt_100uV;
//			if (volt_100uV < FEB_CONST_CELL_MIN_VOLT_100uV ||
//				volt_100uV > FEB_CONST_CELL_MAX_VOLT_100uV) {
//				FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//			}
//			if (FEB_SM_Get_Current_State() == FEB_SM_ST_BALANCE) {
//				if (accumulator.banks[bank].cells[cell].balance &&
//					volt_100uV <= accumulator.target_voltage_100uV) {
//					accumulator.banks[bank].cells[cell].balance = false;
//				}
//			}
//		}
//	}
//}
//
//float FEB_LTC6811_Get_Total_Voltage(void) {
//	return accumulator.total_volt_V;
//}
//
//void FEB_LTC6811_CAN_Transmit_Voltage(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t volt_100uV = accumulator.banks[bank].cells[cell].volt_100uV;
//
//			// Initialize transmission header
//			FEB_CAN_Tx_Header.DLC = 4;
//			FEB_CAN_Tx_Header.StdId = FEB_CAN_ID_BMS_VOLTAGE;
//			FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
//			FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
//			FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;
//
//			// Configure data
//			FEB_CAN_Tx_Data[0] = bank + 1;
//			FEB_CAN_Tx_Data[1] = cell + 1;
//			FEB_CAN_Tx_Data[2] = volt_100uV >> 8;
//			FEB_CAN_Tx_Data[3] = volt_100uV && 0xFF;
//
//			// Delay until mailbox available
//			while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}
//
//			// Add Tx data to mailbox
//			if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
//				FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//			}
//		}
//	}
//}
//
//// ******************************** Balance Functions ********************************
//
//void FEB_LTC6811_Init_Balance(void) {
//	if (FEB_SM_Get_Current_State() == FEB_SM_ST_BALANCE) {
//		set_target_voltage();
//		balance_all_cells();
//	}
//}
//
//void FEB_LTC6811_Balance_Process(void) {
//	if (FEB_SM_Get_Current_State() == FEB_SM_ST_BALANCE) {
//		balance();
//		osDelay(30000);
//		FEB_LTC6811_Stop_Balance();
//		osDelay(1000);
//		if (balance_complete()) {
//			FEB_SM_Set_Current_State(FEB_SM_ST_PRECHARGE);
//		}
//	}
//}
//
//static bool balance_complete(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			if (accumulator.banks[bank].cells[cell].balance) {
//				return false;
//			}
//		}
//	}
//	return true;
//}
//
//static void balance(void) {
//	accumulator.balance = true;
//}
//
//void FEB_LTC6811_Stop_Balance(void) {
//	accumulator.balance = false;
//}
//
//static void balance_all_cells(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			accumulator.banks[bank].cells[cell].balance = true;
//		}
//	}
//}
//
//static void set_target_voltage(void) {
//	accumulator.target_voltage_100uV = accumulator.banks[0].cells[0].volt_100uV;
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			uint16_t temp_voltage_100uV = accumulator.banks[bank].cells[cell].volt_100uV;
//			if (temp_voltage_100uV < accumulator.target_voltage_100uV) {
//				accumulator.target_voltage_100uV = temp_voltage_100uV;
//			}
//		}
//	}
//}
//
//static void configure_DCCBITS_A(uint8_t IC) {
//	uint8_t bank = get_bank(IC);
//	for (uint8_t LTC6811_cell = 0; LTC6811_cell < FEB_CONST_NUM_LTC_CELLS_PER_IC; LTC6811_cell++) {
//		int8_t acc_cell = get_accumulator_cell(IC, LTC6811_cell);
//		if (acc_cell == -1 ||
//			accumulator.balance == false ||
//			FEB_SM_Get_Current_State() != FEB_SM_ST_BALANCE) {
//			DCCBITS_A[LTC6811_cell] = false;
//		} else {
//			DCCBITS_A[LTC6811_cell] = accumulator.banks[bank].cells[acc_cell].balance;
//		}
//	}
//}
//
//// ******************************** Temperature Functions ********************************
//
//void FEB_LTC6811_Poll_Temperature(void) {
//	for (uint8_t channel = 0; channel < FEB_CONST_NUM_TEMP_MUX_CH; channel++) {
//		set_GPIO_bits(channel);
//		start_GPIO_ADC_measurement();
//		read_ADC_voltage_registers();
//		store_temperature_values(channel);
//	}
//}
//
//static void set_GPIO_bits(uint8_t channel) {
//	GPIOBITS_A[0] = 0b1;					// ADC - Multiplexer A
//	GPIOBITS_A[1] = 0b1;					// ADC - Multiplexer B
//	GPIOBITS_A[2] = (channel >> 0) & 0b1;	// MUX Select
//	GPIOBITS_A[3] = (channel >> 1) & 0b1;	// MUX Select
//	GPIOBITS_A[4] = (channel >> 2) & 0b1;	// MUX Select
//
//    for (uint8_t IC = 0; IC < NUM_IC; IC++) {
//    	configure_DCCBITS_A(IC);	// Set balance register bits
//		LTC6811_set_cfgr(IC, IC_config, REFON, ADCOPT, GPIOBITS_A, DCCBITS_A, DCTOBITS, UV, OV);
//    }
//    wakeup_idle(NUM_IC);
//    LTC6811_wrcfg(NUM_IC, IC_config);
//}
//
//static void start_GPIO_ADC_measurement(void) {
//	wakeup_sleep(NUM_IC);
//	LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
//	LTC6811_pollAdc();
//}
//
//static void read_ADC_voltage_registers(void) {
//	wakeup_sleep(NUM_IC);
//	LTC6811_rdaux(SEL_ALL_REG, NUM_IC, IC_config);
//}
//
//static void store_temperature_values(uint8_t channel) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			if (get_channel(cell) == channel) {
//				uint8_t IC = get_IC(bank, cell);
//				uint8_t mux = get_mux(cell);
//				uint16_t voltage_100uV = IC_config[IC].aux.a_codes[mux];
//				accumulator.banks[bank].cells[cell].temp_100mC = FEB_Temp_LUT_Get_Temp_100mC(voltage_100uV * 1e-1);
//
//				char UART_Str[128];
//				sprintf(UART_Str, "Bank: %d, Cell: %d, value: %f, channel: %d, mux: %d, ic: %d\n", bank, cell, accumulator.banks[bank].cells[cell].temp_100mC * 1e-1, channel, mux, IC);
//				HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//			}
//		}
//	}
//}
//
//void FEB_LTC6811_Validate_Temperature(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			if (!valid_temp_sensor(bank, cell)) {
//				// Only  check valid temperature sensors
//				continue;
//			}
//			int16_t temp_100mC = accumulator.banks[bank].cells[cell].temp_100mC;
//			if (FEB_SM_Get_Current_State() == FEB_SM_ST_CHARGE) {
//				if (temp_100mC < FEB_CONST_CELL_MIN_TEMP_CH_100mC ||
//					temp_100mC > FEB_CONST_CELL_MAX_TEMP_CH_100mC) {
//					FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//				}
//			} else {
//				if (temp_100mC < FEB_CONST_CELL_MIN_TEMP_DC_100mC ||
//					temp_100mC > FEB_CONST_CELL_MAX_TEMP_DC_100mC) {
//					FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//				}
//			}
//		}
//	}
//}
//
//void FEB_LTC6811_CAN_Transmit_Temperature(void) {
//	for (uint8_t bank = 0; bank < FEB_CONST_NUM_BANKS; bank++) {
//		for (uint8_t cell = 0; cell < FEB_CONST_NUM_CELLS_PER_BANK; cell++) {
//			int16_t temp_100mC = accumulator.banks[bank].cells[cell].temp_100mC;
//
//			// Initialize transmission header
//			FEB_CAN_Tx_Header.DLC = 4;
//			FEB_CAN_Tx_Header.StdId = FEB_CAN_ID_BMS_TEMPERATURE;
//			FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
//			FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
//			FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;
//
//			// Configure data
//			FEB_CAN_Tx_Data[0] = bank + 1;
//			FEB_CAN_Tx_Data[1] = cell + 1;
//			FEB_CAN_Tx_Data[2] = temp_100mC >> 8;
//			FEB_CAN_Tx_Data[3] = temp_100mC && 0xFF;
//
//			// Delay until mailbox available
//			while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}
//
//			// Add Tx data to mailbox
//			if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
//				FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//			}
//		}
//	}
//}

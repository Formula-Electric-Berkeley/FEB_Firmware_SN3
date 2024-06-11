#include "FEB_LTC6811.h"
#include "FEB_Config.h"
#include "FEB_Temp_LUT.h"
#include "FEB_SM.h"
#include "FEB_CAN_Library/FEB_CAN_ID.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "LTC6811.h"
#include "LTC681x.h"
#include "stdio.h"
#include "string.h"

extern osMutexId_t FEB_LTC6811_LockHandle;
extern osMutexId_t FEB_UART_LockHandle;
extern UART_HandleTypeDef huart2;
extern CAN_HandleTypeDef hcan1;
extern uint32_t FEB_CAN_Tx_Mailbox;

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

#define FEB_LTC6811_VOLTAGE_ERROR_VALUE ((uint16_t) 0xFFFF)
#define FEB_LTC6811_VOLTAGE_MAX_ERROR_COUNT 3

#define FEB_LTC6811_TEMPERATURE_ERROR_VALUE ((int16_t) 0x7FFF)
#define FEB_LTC6811_TEMPERATURE_MAX_ERROR_COUNT 3

typedef struct {
	uint16_t voltage_mV;				// Mili-volts
	int16_t temperature_dC;				// Deci-celsius
	bool voltage_fault;					// Under/over voltage
	bool temperature_fault;				// Under/over temperature
	bool temperature_disabled;			// Disabled temperature sensors
	bool balance;						// Cell balance
	uint32_t voltage_error_count;		// Reset on valid voltage readings
	uint32_t temperature_error_count;	// Reset on valid temperature readings
} cell_t;
typedef struct {
	cell_t cells[FEB_CONFIG_NUM_BANKS][FEB_CONFIG_NUM_CELLS_PER_BANK];

	// Voltage summary
	uint32_t total_voltage_mV;
	uint16_t cell_min_voltage_mV;
	uint16_t cell_max_voltage_mV;

	// Temperature summary
	int16_t cell_min_temperature_dC;
	int16_t cell_max_temperature_dC;
	int16_t cell_avg_temperature_dC;

	uint16_t balance_target_voltage_mV;
	bool balance_active;					// Used to toggle balance while in balance state
} accumulator_t;
static accumulator_t accumulator;
static cell_asic IC_config[NUM_IC];

/* ******** Static Function Declaration ******** */

static void init_accumulator_data(void);
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
static void store_temperature_values(uint8_t channel, int16_t *min_temperature_dC,
	int16_t *max_temperature_dC, int32_t *total_temperature_dC, uint8_t *count);

/* ******** Startup and Helper Functions ******** */

void FEB_LTC6811_Init(void) {
	init_accumulator_data();
	init_all_IC();
}

static void init_accumulator_data(void) {
	accumulator.total_voltage_mV = FEB_CONFIG_NUM_BANKS * FEB_CONFIG_NUM_CELLS_PER_BANK
		* FEB_CONFIG_CELL_MAX_VOLTAGE_100uV * 1e-1;
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			cell_t *cell_data = &accumulator.cells[bank][cell];
			cell_data->voltage_error_count = 0;
			cell_data->temperature_error_count = 0;
			cell_data->temperature_disabled = false;
		}
	}
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

uint16_t FEB_LTC6811_Get_Min_Voltage(void) {
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	uint16_t min_voltage_mV = FEB_CONFIG_CELL_MAX_VOLTAGE_mV;
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			uint16_t cell_voltage_mV = accumulator.cells[bank][cell].voltage_mV;
			if (cell_voltage_mV < min_voltage_mV)
				min_voltage_mV = cell_voltage_mV;
		}
	}
	osMutexRelease(FEB_LTC6811_LockHandle);
	return min_voltage_mV;
}

void FEB_LTC6811_UART_Transmit(void) {
	if (!FEB_SM_ST_DEBUG)
		return;

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

			// Format cell data string
			// Data: bank, cell, voltage, temperature, voltage fault, temperature fault, balance state
			sprintf(str, "cell %d %d %d %d %d %d %d\n",
					bank + 1, cell + 1,
					cell_voltage_mV, cell_temperature_dC,
					cell_voltage_fault, cell_temperature_fault,
					balance);

			// Transmit cell data
			while (osMutexAcquire(FEB_UART_LockHandle, UINT32_MAX) != osOK);
			HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
			osMutexRelease(FEB_UART_LockHandle);
		}
	}

	// Get accumulator voltage data
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	uint32_t total_voltage_mV = accumulator.total_voltage_mV;
	uint16_t cell_min_voltage_mV = accumulator.cell_min_voltage_mV;
	uint16_t cell_max_voltage_mV = accumulator.cell_max_voltage_mV;
	osMutexRelease(FEB_LTC6811_LockHandle);

	// Transmit accumulator voltage data
	// Data: total voltage, min cell voltage, max cell voltage
	sprintf(str, "accumulator-voltage %ld %d %d\n",
			total_voltage_mV, cell_min_voltage_mV, cell_max_voltage_mV);
	while (osMutexAcquire(FEB_UART_LockHandle, UINT32_MAX) != osOK);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
	osMutexRelease(FEB_UART_LockHandle);
}

void FEB_LTC6811_Cell_Data_CAN_Transmit(void) {
	static CAN_TxHeaderTypeDef tx_header;
	static uint8_t tx_data[8];

	// Initialize transmission header
	tx_header.DLC = 6;
	tx_header.StdId = FEB_CAN_ID_BMS_CELL_DATA;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.TransmitGlobalTime = DISABLE;

	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			// Get data
			while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
			uint16_t voltage_mV = accumulator.cells[bank][cell].voltage_mV;
			int16_t temperature_dC = accumulator.cells[bank][cell].temperature_dC;
			bool voltage_fault = accumulator.cells[bank][cell].voltage_fault;
			bool temperature_fault = accumulator.cells[bank][cell].temperature_fault;
			bool balance = accumulator.cells[bank][cell].balance && accumulator.balance_active;
			osMutexRelease(FEB_LTC6811_LockHandle);

			tx_data[0] = (((bank + 1) & 0x7) << 5) + (cell & 0x1F);
			tx_data[1] = voltage_mV & 0xFF;
			tx_data[2] = (voltage_mV >> 8) & 0xFF;
			tx_data[3] = temperature_dC & 0xFF;
			tx_data[4] = (temperature_dC >> 8) & 0xFF;
			tx_data[5] = voltage_fault + (temperature_fault >> 1) + (balance >> 2);

			// Delay until mailbox available
			while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

			// Add Tx data to mailbox
			if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
				// FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
			}
		}
	}
}

void FEB_LTC6811_Accumulator_Voltage_CAN_Transmit(void) {
	static CAN_TxHeaderTypeDef tx_header;
	static uint8_t tx_data[8];

	// Initialize transmission header
	tx_header.DLC = 6;
	tx_header.StdId = FEB_CAN_ID_BMS_ACCUMULATOR_VOLTAGE;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.TransmitGlobalTime = DISABLE;

	// Get data
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	tx_data[0] = (accumulator.total_voltage_mV >> 8) && 0xFF;
	tx_data[1] = accumulator.total_voltage_mV && 0xFF;
	tx_data[2] = (accumulator.cell_min_voltage_mV >> 8) && 0xFF;
	tx_data[3] = accumulator.cell_max_voltage_mV && 0xFF;
	tx_data[4] = (accumulator.cell_max_voltage_mV >> 8) && 0xFF;
	tx_data[5] = accumulator.cell_max_voltage_mV && 0xFF;
	osMutexRelease(FEB_LTC6811_LockHandle);

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		// Shutdown ...
	}
}

void FEB_LTC6811_Accumulator_Temperature_CAN_Transmit(void) {
	static CAN_TxHeaderTypeDef tx_header;
	static uint8_t tx_data[8];

	// Initialize transmission header
	tx_header.DLC = 6;
	tx_header.StdId = FEB_CAN_ID_BMS_ACCUMULATOR_VOLTAGE;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.TransmitGlobalTime = DISABLE;

	// Get data
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	tx_data[0] = (accumulator.cell_avg_temperature_dC >> 8) && 0xFF;
	tx_data[1] = accumulator.cell_avg_temperature_dC && 0xFF;
	tx_data[2] = (accumulator.cell_min_temperature_dC >> 8) && 0xFF;
	tx_data[3] = accumulator.cell_min_temperature_dC && 0xFF;
	tx_data[4] = (accumulator.cell_max_temperature_dC >> 8) && 0xFF;
	tx_data[5] = accumulator.cell_max_temperature_dC && 0xFF;
	osMutexRelease(FEB_LTC6811_LockHandle);

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		// Shutdown ...
	}
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
	// Thresholds
	uint16_t threshold_min_voltage_mV = FEB_Config_Get_Cell_Min_Voltage_mV();
	uint16_t threshold_max_voltage_mV = FEB_Config_Get_Cell_Max_Voltage_mV();

	// Measured values
	uint32_t total_voltage_mV = 0;
	uint16_t min_voltage_mV = 0xFFFF;
	uint16_t max_voltage_mV = 0;

	bool pack_voltage_error = false;		// Invalid voltage read, do not update total voltage
	bool pack_voltage_fault = false;		// Fault detected, enter fault state

	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			uint16_t voltage_100uV = IC_config[get_IC(bank, cell)].cells.c_codes[get_IC_cell(cell)];
			cell_t *cell_data = &accumulator.cells[bank][cell];

			// Ignore if voltage error read
			if (voltage_100uV == FEB_LTC6811_VOLTAGE_ERROR_VALUE) {
				pack_voltage_error= true;
				cell_data->voltage_error_count += 1;

				// Fault if too many consecutive errors
				if (cell_data->voltage_error_count > FEB_LTC6811_VOLTAGE_MAX_ERROR_COUNT)
					pack_voltage_fault = true;
			} else {
				// Reset consecutive error count
				cell_data->voltage_error_count = 0;

				// Store voltage
				uint16_t voltage_mV = voltage_100uV * 1e-1;
				cell_data->voltage_mV = voltage_mV;

				// Update stat values
				total_voltage_mV += voltage_mV;
				if (voltage_mV < min_voltage_mV)
					min_voltage_mV = voltage_mV;
				if (voltage_mV > max_voltage_mV)
					max_voltage_mV = voltage_mV;

				// Check under/over voltage fault
				bool cell_voltage_fault = voltage_mV < threshold_min_voltage_mV || voltage_mV > threshold_max_voltage_mV;
				cell_data->voltage_fault = cell_voltage_fault;
				if (cell_voltage_fault)
					pack_voltage_fault = true;
			}
		}
	}
	if (!pack_voltage_error)
		accumulator.total_voltage_mV = total_voltage_mV;
	accumulator.cell_min_voltage_mV = min_voltage_mV;
	accumulator.cell_max_voltage_mV = max_voltage_mV;
	osMutexRelease(FEB_LTC6811_LockHandle);

	if (pack_voltage_fault)
		FEB_SM_Transition(FEB_SM_ST_FAULT);
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

/* ******** Temperature Functions ******** */

void FEB_LTC6811_Poll_Temperature(void) {
	int16_t min_temperature_dC = 0x7FFF;
	int16_t max_temperature_dC = 0x8000;
	int32_t total_temperature_dC = 0;
	uint8_t count = 0;

	// Poll temperature values
	for (uint8_t channel = 0; channel < FEB_CONFIG_NUM_TEMP_MUX_CH; channel++) {
		set_GPIO_bits(channel);
		start_GPIO_ADC_measurement();
		read_ADC_voltage_registers();
		store_temperature_values(channel, &min_temperature_dC, &max_temperature_dC, &total_temperature_dC, &count);
	}

	// Update accumulator temperature stats
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	accumulator.cell_min_temperature_dC = min_temperature_dC;
	accumulator.cell_max_temperature_dC = max_temperature_dC;
	accumulator.cell_avg_temperature_dC = (int16_t) (total_temperature_dC / count);
	osMutexRelease(FEB_LTC6811_LockHandle);
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

static void store_temperature_values(uint8_t channel, int16_t *min_temperature_dC,
	int16_t *max_temperature_dC, int32_t *total_temperature_dC, uint8_t *count) {
	bool pack_temperature_fault = false;
	while (osMutexAcquire(FEB_LTC6811_LockHandle, UINT32_MAX) != osOK);
	for (uint8_t bank = 0; bank < FEB_CONFIG_NUM_BANKS; bank++) {
		for (uint8_t cell = 0; cell < FEB_CONFIG_NUM_CELLS_PER_BANK; cell++) {
			if (get_channel(cell) == channel) {
				uint8_t IC = get_IC(bank, cell);
				uint8_t mux = get_mux(cell);
				uint16_t voltage_100uV = IC_config[IC].aux.a_codes[mux];
				int16_t temperature_dC = FEB_Temp_LUT_Get_Temp_100mC(voltage_100uV * 1e-1);
				cell_t *cell_data = &accumulator.cells[bank][cell];

				// Ignore disabled temperature sensors
				if (cell_data->temperature_disabled)
					continue;

				// Ignore if temperature error read
				if (temperature_dC == 0x8000 || temperature_dC == 0x7FFF) {
					cell_data->temperature_error_count += 1;

					// Fault if too many consecutive errors
					if (cell_data->temperature_error_count > FEB_LTC6811_TEMPERATURE_MAX_ERROR_COUNT)
						pack_temperature_fault = true;
				} else {
					// Reset consecutive error count
					cell_data->temperature_error_count = 0;

					// Store temperature
					cell_data->temperature_dC = temperature_dC;

					// Update temperature stats
					if (temperature_dC < *min_temperature_dC)
						*min_temperature_dC = temperature_dC;
					if (temperature_dC > *max_temperature_dC)
						*max_temperature_dC = temperature_dC;
					*total_temperature_dC += temperature_dC;
					*count += 1;

					// Check under/over temperature fault
					int16_t min_temperature_dC = FEB_Config_Get_Cell_Min_Temperature_dC();
					int16_t max_temperature_dC = FEB_Config_Get_Cell_Max_Temperature_dC();
					bool temperature_fault = (temperature_dC < min_temperature_dC || temperature_dC > max_temperature_dC);
					accumulator.cells[bank][cell].temperature_fault = temperature_fault;
					if (temperature_fault)
						pack_temperature_fault = true;
				}
			}
		}
	}
	osMutexRelease(FEB_LTC6811_LockHandle);
	if (pack_temperature_fault)
		FEB_SM_Transition(FEB_SM_ST_FAULT);
}

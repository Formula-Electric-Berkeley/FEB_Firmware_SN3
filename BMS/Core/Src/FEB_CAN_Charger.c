#include "FEB_CAN_Charger.h"
#include "FEB_SM.h"
#include "FEB_CAN_ID.h"
#include "FEB_Config.h"

extern CAN_HandleTypeDef hcan1;
extern uint8_t FEB_CAN_Tx_Data[8];
extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
extern uint32_t FEB_CAN_Tx_Mailbox;

typedef struct {
	uint16_t max_voltage_dV;		// Deci-volts
	uint16_t max_current_dA;		// Deci-amps
	uint8_t control;				// 0 (start charge), 1 (stop charge)
} BMS_message_t;
typedef struct {
	uint16_t op_voltage_dV;			// Operating voltage
	uint16_t op_current_dA;			// Ooperating current
	uint8_t status;
} CCS_message_t;
static BMS_message_t BMS_message;
static CCS_message_t CCS_message;

static void charger_CAN_transmit(void);

/* ******** CAN Functions ******** */

uint8_t FEB_CAN_Charger_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_Assignment, uint8_t filter_bank) {
	CAN_FilterTypeDef CAN_filter_config;

	CAN_filter_config.FilterActivation = CAN_FILTER_ENABLE;
	CAN_filter_config.FilterBank = filter_bank;
	CAN_filter_config.FilterFIFOAssignment = FIFO_Assignment;
	CAN_filter_config.FilterIdHigh = FEB_CAN_ID_CHARGER_CCS >> 13;
	CAN_filter_config.FilterIdLow = (FEB_CAN_ID_CHARGER_CCS & 0x1FFF) << 3;
	CAN_filter_config.FilterMaskIdHigh = 0xFFFF;
	CAN_filter_config.FilterMaskIdLow = 0xFFF8;
	CAN_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_filter_config.SlaveStartFilterBank = 27;

	if (HAL_CAN_ConfigFilter(hcan, &CAN_filter_config) != HAL_OK) {
//		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}

	return ++filter_bank;
}

void FEB_CAN_Charger_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t RxData[]) {
	switch(pHeader->ExtId) {
	    case FEB_CAN_ID_CHARGER_CCS:
	    	CCS_message.op_voltage_dV = (uint16_t) (RxData[0] << 8) + RxData[1];
	    	CCS_message.op_current_dA = (uint16_t) (RxData[2] << 8) + RxData[3];
	    	CCS_message.status = RxData[4];
			break;
	}
}

static void charger_CAN_transmit(void) {
	// Initialize transmission header
	FEB_CAN_Tx_Header.DLC = 8;
	FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_CHARGER_BMS;
	FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

	// Copy data to Tx buffer
	FEB_CAN_Tx_Data[0] = BMS_message.max_voltage_dV >> 8;
	FEB_CAN_Tx_Data[1] = BMS_message.max_voltage_dV & 0xFF;
	FEB_CAN_Tx_Data[2] = BMS_message.max_current_dA >> 8;
	FEB_CAN_Tx_Data[3] = BMS_message.max_current_dA & 0xFF;
	FEB_CAN_Tx_Data[4] = BMS_message.control;
	FEB_CAN_Tx_Data[5] = 0;
	FEB_CAN_Tx_Data[6] = 0;
	FEB_CAN_Tx_Data[7] = 0;

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
//		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}

// ******** Interface Functions ********

void FEB_CAN_Charger_Init(void) {
	BMS_message.max_voltage_dV = FEB_CONFIG_NUM_BANKS * FEB_CONFIG_NUM_CELLS_PER_BANK * (FEB_CONFIG_CELL_MAX_VOLTAGE_mV * 1e-2);
	BMS_message.max_current_dA = 50;
}

void FEB_CAN_Charger_Process(void) {
	if (FEB_SM_Get_Current_State() == FEB_SM_ST_CHARGE) {
		charger_CAN_transmit();
	}
}

void FEB_CAN_Charger_Start_Charge(void) {
	BMS_message.control = 0;
}

void FEB_CAN_Charger_Stop_Charge(void) {
	BMS_message.control = 1;
}


//// ******************************** Includes & External ********************************
//
//#include "FEB_CAN_Charger.h"
//
//extern CAN_HandleTypeDef hcan1;
//extern uint8_t FEB_CAN_Tx_Data[8];
//extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
//extern uint32_t FEB_CAN_Tx_Mailbox;
//
//// ******************************** CAN Data ********************************
//
//typedef struct {
//	uint16_t max_voltage_100mV;
//	uint16_t max_current_100mA;
//	uint8_t control;				// 0 (start charge), 1 (stop charge)
//} BMS_message_t;
//static BMS_message_t BMS_message;
//
//typedef struct {
//	volatile uint16_t op_voltage_100mV;
//	volatile uint16_t op_current_100mA;
//	volatile uint8_t status;
//} CCS_message_t;
//static CCS_message_t CCS_message;
//
//// ******************************** Static Function Declaration ********************************
//
//static void CAN_charger_transmit(void);
//
//// ******************************** CAN Functions ********************************
//
//uint8_t FEB_CAN_Charger_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_Assignment, uint8_t filter_bank) {
//	CAN_FilterTypeDef CAN_filter_config;
//
//	CAN_filter_config.FilterActivation = CAN_FILTER_ENABLE;
//	CAN_filter_config.FilterBank = filter_bank;
//	CAN_filter_config.FilterFIFOAssignment = FIFO_Assignment;
//	CAN_filter_config.FilterIdHigh = FEB_CAN_ID_CHARGER_CCS >> 13;
//	CAN_filter_config.FilterIdLow = (FEB_CAN_ID_CHARGER_CCS & 0x1FFF) << 3;
//	CAN_filter_config.FilterMaskIdHigh = 0xFFFF;
//	CAN_filter_config.FilterMaskIdLow = 0xFFF8;
//	CAN_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
//	CAN_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
//	CAN_filter_config.SlaveStartFilterBank = 27;
//
//	if (HAL_CAN_ConfigFilter(hcan, &CAN_filter_config) != HAL_OK) {
//		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//	}
//
//	return ++filter_bank;
//}
//
//void FEB_CAN_Charger_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t RxData[]) {
//	switch(pHeader->ExtId) {
//	    case FEB_CAN_ID_CHARGER_CCS:
//	    	CCS_message.op_voltage_100mV = (uint16_t) (RxData[0] << 8) + RxData[1];
//	    	CCS_message.op_current_100mA = (uint16_t) (RxData[2] << 8) + RxData[3];
//	    	CCS_message.status = RxData[4];
//			break;
//	}
//}
//
//static void CAN_charger_transmit(void) {
//	// Initialize transmission header
//	FEB_CAN_Tx_Header.DLC = 8;
//	FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_CHARGER_BMS;
//	FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
//	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
//	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;
//
//	// Copy data to Tx buffer
//	FEB_CAN_Tx_Data[0] = BMS_message.max_voltage_100mV >> 8;
//	FEB_CAN_Tx_Data[1] = BMS_message.max_voltage_100mV & 0xFF;
//	FEB_CAN_Tx_Data[2] = BMS_message.max_current_100mA >> 8;
//	FEB_CAN_Tx_Data[3] = BMS_message.max_current_100mA & 0xFF;
//	FEB_CAN_Tx_Data[4] = BMS_message.control;
//	FEB_CAN_Tx_Data[5] = 0;
//	FEB_CAN_Tx_Data[6] = 0;
//	FEB_CAN_Tx_Data[7] = 0;
//
//	// Delay until mailbox available
//	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}
//
//	// Add Tx data to mailbox
//	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
//		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//	}
//}
//
//// ******************************** Interface Functions ********************************
//
//void FEB_CAN_Charger_Init(void) {
//	BMS_message.max_voltage_100mV = FEB_CONST_NUM_CELLS * FEB_CONST_CELL_MAX_VOLT_V * 10;
//	BMS_message.max_current_100mA = FEB_CONST_MAX_CURR_CH * 10;
//}
//
//void FEB_CAN_Charger_Process(void) {
//	if (FEB_SM_Get_Current_State() == FEB_SM_ST_CHARGE) {
//		// TODO: Check battery charge state
//		// TODO: Start / stop charge
//	}
//}
//
//void FEB_CAN_Charger_Start_Charge(void) {
//	BMS_message.control = 0;
//	CAN_charger_transmit();
//}
//
//void FEB_CAN_Charger_Stop_Charge(void) {
//	BMS_message.control = 1;
//	CAN_charger_transmit();
//}
//

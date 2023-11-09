// Remove validate charger status

// ********************************** Includes & External **********************************

#include "FEB_CAN_Charger.h"

extern CAN_HandleTypeDef hcan1;

extern uint8_t FEB_CAN_Tx_Data[8];
extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
extern uint32_t FEB_CAN_Tx_Mailbox;
extern bool FEB_State_Charge;
extern bool FEB_State_Shutdown;

// ********************************** CAN Configuration **********************************

static FEB_CAN_Charger_BMS_Message_t FEB_CAN_Charger_BMS_Message;
static FEB_CAN_Charger_Charger_Message_t FEB_CAN_Charger_Charger_Message;
static bool FEB_CAN_Charger_Rx_Flag;

// ********************************** Functions **********************************

// ******************** CAN ********************

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

	if (HAL_CAN_ConfigFilter(hcan, &CAN_filter_config) != HAL_OK && !FEB_State_Shutdown) {
		FEB_State_Set_Shutdown("[FEB_CAN_Charger_Filter_Config] Invalid CAN filter configuration.");
	}

	return ++filter_bank;
}

void FEB_CAN_Charger_Transmit(void) {
	// Initialize transmission header
	FEB_CAN_Tx_Header.DLC = 8;
	FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_CHARGER_BMS;
	FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

	// Copy data to Tx buffer
	FEB_CAN_Tx_Data[0] = FEB_CAN_Charger_BMS_Message.max_voltage_100mV >> 8;
	FEB_CAN_Tx_Data[1] = FEB_CAN_Charger_BMS_Message.max_voltage_100mV & 0xFF;
	FEB_CAN_Tx_Data[2] = FEB_CAN_Charger_BMS_Message.max_current_100mA >> 8;
	FEB_CAN_Tx_Data[3] = FEB_CAN_Charger_BMS_Message.max_current_100mA & 0xFF;
	FEB_CAN_Tx_Data[4] = FEB_CAN_Charger_BMS_Message.control;
	FEB_CAN_Tx_Data[5] = 0;
	FEB_CAN_Tx_Data[6] = 0;
	FEB_CAN_Tx_Data[7] = 0;

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_State_Set_Shutdown("[FEB_CAN_Charger_Transmit] CAN transmission error.");
	}
}

void FEB_CAN_Charger_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t RxData[]) {
	switch(pHeader->ExtId) {
	    case FEB_CAN_ID_CHARGER_CCS:
	    	FEB_CAN_Charger_Rx_Flag = true;
	    	FEB_CAN_Charger_Charger_Message.operating_voltage_100mV = (RxData[0] << 8) + RxData[1];
	    	FEB_CAN_Charger_Charger_Message.operating_current_100mA = (RxData[2] << 8) + RxData[3];
	    	FEB_CAN_Charger_Charger_Message.status = RxData[4];
	    	// TODO: FIX
	    	// FEB_CAN_Charger_Validate_Status(RxData[4]);
			break;
	}
}

// ******************** Interface ********************

void FEB_CAN_Charger_Validate_Status(uint8_t status) {
	// Failure bits: 0 (no error), 1 (error)
	uint8_t hardware_failure 			= (status >> 7) & 0b1;
	uint8_t temperature_failure 		= (status >> 6) & 0b1;
	uint8_t input_voltage_failure 		= (status >> 5) & 0b1;
	uint8_t starting_state_failure 		= (status >> 4) & 0b1;
	uint8_t communication_state_failure = (status >> 3) & 0b1;

	if (hardware_failure == 1) {
		FEB_State_Set_Shutdown("[FEB_CAN_Charger_Validate_Status] CAN charger hardware_failure status error.");
	} else if (temperature_failure == 1) {
		FEB_State_Set_Shutdown("[FEB_CAN_Charger_Validate_Status] CAN charger temperature_failure status error.");
	} else if (input_voltage_failure == 1) {
		FEB_State_Set_Shutdown("[FEB_CAN_Charger_Validate_Status] CAN charger input_voltage_failure status error.");
	} else if (starting_state_failure == 1) {
		FEB_State_Set_Shutdown("[FEB_CAN_Charger_Validate_Status] CAN charger starting_state_failure status error.");
	} else if (communication_state_failure == 1) {
		FEB_State_Set_Shutdown("[FEB_CAN_Charger_Validate_Status] CAN charger communication_state_failure status error.");
	}
}

void FEB_CAN_Charger_Process(CAN_HandleTypeDef* hcan) {
	// TODO: Implement function
}

void FEB_CAN_Charger_Charge(void) {
	FEB_CAN_Charger_BMS_Message.max_voltage_100mV = (uint16_t) (FEB_CONSTANT_CELL_MAX_VOLT * FEB_CONSTANT_NUM_CELLS_PER_BANK * FEB_CONSTANT_NUM_BANKS * 10);
	FEB_CAN_Charger_BMS_Message.max_current_100mA = (uint16_t) (FEB_CONSTANT_MAX_CHARGING_CURRENT * 10);
	FEB_CAN_Charger_BMS_Message.control = 0;
	FEB_CAN_Charger_Transmit();
}

void FEB_CAN_Charger_Stop_Charge(void) {
	FEB_CAN_Charger_BMS_Message.max_voltage_100mV = 0;
	FEB_CAN_Charger_BMS_Message.max_current_100mA = 0;
	FEB_CAN_Charger_BMS_Message.control = 1;
	FEB_CAN_Charger_Transmit();
}

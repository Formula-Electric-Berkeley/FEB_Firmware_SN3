// ******************************** Includes & External ********************************

#include "FEB_CAN_IVT.h"

extern UART_HandleTypeDef huart2;
extern bool FEB_State_Debug;

// ******************************** Type Definitions ********************************

FEB_CAN_IVT_FLAG_TYPE FEB_CAN_IVT_Flags;
FEB_CAN_IVT_MESSAGE_TYPE FEB_CAN_IVT_Messages;

// ******************************** Functions ********************************

uint8_t FEB_CAN_IVT_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
	uint16_t ids[] = {FEB_CAN_ID_IVT_CURRENT, FEB_CAN_ID_IVT_VOLTAGE_1, FEB_CAN_ID_IVT_VOLTAGE_2, FEB_CAN_ID_IVT_VOLTAGE_3};

	for (uint8_t i = 0; i < 4; i++) {
		CAN_FilterTypeDef filter_config;

	    // Standard CAN - 2.0A - 11 bit
	    filter_config.FilterActivation = CAN_FILTER_ENABLE;
		filter_config.FilterBank = filter_bank;
		filter_config.FilterFIFOAssignment = FIFO_assignment;
		filter_config.FilterIdHigh = ids[i] << 5;
		filter_config.FilterIdLow = 0;
		filter_config.FilterMaskIdHigh = 0xFFE0;
		filter_config.FilterMaskIdLow = 0;
		filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
		filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
		filter_config.SlaveStartFilterBank = 27;
	    filter_bank++;

		if (HAL_CAN_ConfigFilter(hcan, &filter_config) != HAL_OK) {
	        FEB_State_Set_Shutdown("[FEB_CAN_IVT_Filter] Invalid filter assignment.");
		}
	}

	return filter_bank;
}

void FEB_CAN_IVT_Store_Msg(CAN_RxHeaderTypeDef* rx_header, uint8_t rx_data[]) {
	switch(rx_header->ExtId) {
	    case FEB_CAN_ID_IVT_CURRENT:
	    	FEB_CAN_IVT_Flags.current = true;
	    	FEB_CAN_IVT_Messages.current_mA = (int32_t) ((rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5]);
			break;
	    case FEB_CAN_ID_IVT_VOLTAGE_1:
	    	FEB_CAN_IVT_Flags.voltage_1 = true;
	    	FEB_CAN_IVT_Messages.voltage_1_mV = (int32_t) ((rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5]);
	    	break;
	    case FEB_CAN_ID_IVT_VOLTAGE_2:
	    	FEB_CAN_IVT_Flags.voltage_2 = true;
	    	FEB_CAN_IVT_Messages.voltage_2_mV = (int32_t) ((rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5]);
	    	break;
	    case FEB_CAN_ID_IVT_VOLTAGE_3:
	    	FEB_CAN_IVT_Flags.voltage_3 = true;
	    	FEB_CAN_IVT_Messages.voltage_3_mV = (int32_t) ((rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5]);
	    	break;
	}
}

void FEB_CAN_IVT_Process(void) {
	if (FEB_CAN_IVT_Flags.current) {
		// Do something
	}
	if (FEB_CAN_IVT_Flags.voltage_1) {
		// Do something
	}
	if (FEB_CAN_IVT_Flags.voltage_2) {
		// Do something
	}
	if (FEB_CAN_IVT_Flags.voltage_3) {
		// Do something
	}
}

void FEB_CAN_IVT_UART_Transmit(void) {
	if (!FEB_State_Debug) {
		return;
	}
	char UART_str[64];
	sprintf(UART_str, "%d %ld %ld %ld %ld\n", FEB_UART_IVT_Data_ID, FEB_CAN_IVT_Messages.voltage_1_mV, FEB_CAN_IVT_Messages.voltage_2_mV,
											  FEB_CAN_IVT_Messages.voltage_3_mV, FEB_CAN_IVT_Messages.current_mA);
	HAL_UART_Transmit_IT(&huart2, (uint8_t*) UART_str, strlen(UART_str));
}

#include "FEB_CAN_BMS.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
extern uint8_t FEB_CAN_Tx_Data[8];
extern uint32_t FEB_CAN_Tx_Mailbox;
extern uint8_t FEB_CAN_Measured_Speed[8];
extern UART_HandleTypeDef huart2;


uint8_t FEB_CAN_BMS_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
    // For multiple filters, create array of filter IDs and loop over IDs.

	uint16_t filter_arr[] = {FEB_CAN_ID_BMS_DART1_REQUESTED_FAN_SPEEDS};

	for (uint8_t i = 0; i < 1; i++) {

		CAN_FilterTypeDef filter_config;

		// Standard CAN - 2.0A - 11 bit
		filter_config.FilterActivation = CAN_FILTER_ENABLE;
		filter_config.FilterBank = filter_bank;
		filter_config.FilterFIFOAssignment = FIFO_assignment;
		filter_config.FilterIdHigh = filter_arr[i] << 5;
		filter_config.FilterIdLow = 0;
		filter_config.FilterMaskIdHigh = 0xFFE0;
		filter_config.FilterMaskIdLow = 0;
		filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
		filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
		filter_config.SlaveStartFilterBank = 27;
		filter_bank++;


			if (HAL_CAN_ConfigFilter(hcan, &filter_config) != HAL_OK) {
				// Code Error - Shutdown
			}
	}

	return filter_bank;
}

void FEB_CAN_BMS_Process_Message(CAN_RxHeaderTypeDef *rx_header, uint8_t FEB_CAN_Rx_Data[]) {
	switch(rx_header->ExtId) {
		case FEB_CAN_ID_BMS_DART1_REQUESTED_FAN_SPEEDS:
			FEB_Fan_CAN_Process(FEB_CAN_Rx_Data);
			break;
		}
	}


void FEB_CAN_Transmit() {
	// Initialize Transmission Header
	FEB_CAN_Tx_Header.DLC = 4;
	FEB_CAN_Tx_Header.StdId = FEB_CAN_ID_DART_DART1_MEASURED_FAN_SPEEDS;
	FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;


	// Configure FEB_CAN_Tx_Data
//    FEB_CAN_Tx_Data[0] = 0x0;

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}
	// Add Tx data to mailbox

	//Print values that would be transmitted via CAN
	HAL_UART_Transmit(&huart2, FEB_CAN_Measured_Speed, sizeof(FEB_CAN_Measured_Speed), 100);

	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Measured_Speed, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		// Code Error - Shutdown
	}
}



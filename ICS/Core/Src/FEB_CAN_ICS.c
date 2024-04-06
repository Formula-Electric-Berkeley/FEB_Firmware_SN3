// **************************************** Includes ****************************************

#include "FEB_CAN_ICS.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
extern uint8_t FEB_CAN_Tx_Data[8];
extern uint32_t FEB_CAN_Tx_Mailbox;

// ******************************** Variables ********************************

FEB_CAN_ICS_Message_t FEB_CAN_ICS_Message;

uint8_t speed = 0;

// **************************************** Functions ****************************************

uint8_t FEB_CAN_ICS_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
    // For multiple filters, create array of filter IDs and loop over IDs.
	uint16_t ids[] = {FEB_CAN_ID_ICS_TEST, FEB_CAN_ID_ICS_READY_TO_DRIVE, FEB_CAN_ID_ICS_BUTTON_2, FEB_CAN_ID_ICS_BUTTON_3, FEB_CAN_ID_ICS_BUTTON_4, FEB_CAN_ID_ICS_COOLANT_PUMP, FEB_CAN_ID_ICS_RADIATOR_FANS, FEB_CAN_ID_ICS_ACUMULATOR_FANS};

	for (uint8_t i = 0; i < 8; i++) {
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
			// Code Error - Shutdown
		}
	}

	return filter_bank;
}

void FEB_CAN_ICS_Rx_Handler(CAN_RxHeaderTypeDef *FEB_CAN_Rx_Header, uint8_t FEB_CAN_Rx_Data[]) {
	switch(FEB_CAN_Rx_Header->StdId) {
		case FEB_CAN_ID_ICS_READY_TO_DRIVE:
			FEB_UART_Transmit_String("[PRESSED] Button 1 (Ready To Drive)");
			break;
		case FEB_CAN_ID_ICS_BUTTON_2:
			FEB_UART_Transmit_String("[PRESSED] Button 2");
			break;
		case FEB_CAN_ID_ICS_BUTTON_3:
			FEB_UART_Transmit_String("[PRESSED] Button 3");
			break;
		case FEB_CAN_ID_ICS_BUTTON_4:
			FEB_UART_Transmit_String("[PRESSED] Button 4");
			break;
		case FEB_CAN_ID_ICS_COOLANT_PUMP:
			FEB_UART_Transmit_String("[PRESSED] Switch 1 (Coolant Pump)");
			break;
		case FEB_CAN_ID_ICS_RADIATOR_FANS:
			FEB_UART_Transmit_String("[PRESSED] Switch 2 (Radiator Fans)");
			break;
		case FEB_CAN_ID_ICS_ACUMULATOR_FANS:
			FEB_UART_Transmit_String("[PRESSED] Switch 3 (Accumulator Fans)");
			break;
		case FEB_CAN_ID_ICS_TEST:
			FEB_CAN_ICS_Message.speed = FEB_CAN_Rx_Data[0];
			break;
	}
}

void FEB_CAN_ICS_Transmit_Button_State(uint32_t Msg_ID, uint8_t transmit_button_state) {
	FEB_CAN_Tx_Header.DLC = 1;
	FEB_CAN_Tx_Header.StdId = Msg_ID;
	FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

	// Copy data to Tx buffer
	FEB_CAN_Tx_Data[0] = transmit_button_state;

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		//
	}
}

void FEB_CAN_ICS_Transmit_Speed(void) {
	FEB_CAN_Tx_Header.DLC = 1;
	FEB_CAN_Tx_Header.StdId = FEB_CAN_ID_ICS_TEST;
	FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

	// Copy data to Tx buffer
	speed += 1;
	FEB_CAN_Tx_Data[0] = (uint8_t) speed;

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		//
	}
}

uint8_t FEB_CAN_ICS_Get_Speed(void) {
	return speed;
}

#include "FEB_CAN_SW.h"

FEB_CAN_SW_Message_t FEB_CAN_SW_Message;

extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
extern uint8_t FEB_CAN_Tx_Data[8];
extern uint32_t SW_CAN_ID;
extern uint32_t FEB_CAN_Tx_Mailbox;

uint8_t FEB_CAN_SW_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
    // For multiple filters, create array of filter IDs and loop over IDs.

	uint16_t ids[] = {FEB_CAN_ID_SW_READY_TO_DRIVE};

	for(uint8_t i = 0; i < 1; i ++){

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


void FEB_CAN_SW_Str_Msg(CAN_RxHeaderTypeDef *FEB_CAN_Rx_Header, uint8_t FEB_CAN_Rx_Data[]) {

	switch(FEB_CAN_Rx_Header->StdId) {
	    case FEB_CAN_ID_SW_READY_TO_DRIVE:
			memcpy(&(FEB_CAN_SW_Message.ready_to_drive), &(FEB_CAN_Rx_Data[0]), 4);
	    	break;
	}
}


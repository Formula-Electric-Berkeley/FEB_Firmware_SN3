// **************************************** Includes & External ****************************************

#include "FEB_CAN.h"

extern CAN_HandleTypeDef hcan1;

// **************************************** CAN Configuration ****************************************

CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
static CAN_RxHeaderTypeDef FEB_CAN_Rx_Header;

extern CircularBuffer* FEBBuffer;
extern char* buffer;

uint8_t FEB_CAN_Tx_Data[8];
uint8_t FEB_CAN_Rx_Data[8];

uint32_t FEB_CAN_Tx_Mailbox;

// **************************************** Functions ****************************************


void FEB_CAN_Init(void) {
	FEB_CAN_Filter(&hcan1, 0, 0);
	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
        // Code Error - Shutdown
		printf("Could not initialize CAN Filter");
	}
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/*void FEB_CAN_Filter_Config(void) {
	uint8_t filter_bank = 0;
	// Assign Filter
    // filter_bank = Function(&hcan1, CAN_RX_FIFO0, filter_bank);
} */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &FEB_CAN_Rx_Header, FEB_CAN_Rx_Data) != HAL_OK) {
		// Store Message
        // Function(&FEB_CAN_Rx_Header, FEB_CAN_Rx_Data);
		printf("Could not receive CAN message from FIFO ");
	}

	if(FEB_CAN_Rx_Header.IDE == CAN_ID_STD){
		FEB_circBuf_write(FEBBuffer,(char*)FEB_CAN_Rx_Data);

	}

}

// **************************************** Template Code [Other Files] ****************************************

uint8_t FEB_CAN_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
    // For multiple filters, create array of filter IDs and loop over IDs.

	CAN_FilterTypeDef filter_config;

    // Standard CAN - 2.0A - 11 bit
    filter_config.FilterActivation = CAN_FILTER_ENABLE;
	filter_config.FilterBank = filter_bank;
	filter_config.FilterFIFOAssignment = FIFO_assignment;
	filter_config.FilterIdHigh = 0;
	filter_config.FilterIdLow = 0;
	filter_config.FilterMaskIdHigh = 0;
	filter_config.FilterMaskIdLow = 0;
	filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
	filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
	filter_config.SlaveStartFilterBank = 27;



	if (HAL_CAN_ConfigFilter(hcan, &filter_config) != HAL_OK) {
        // Code Error - Shutdown
	}

	return filter_bank;
}

void FEB_CAN_Transmit_Test_Float(CAN_HandleTypeDef* hcan,float data) {
	// Initialize Transmission Header
    // Write Code Here
	FEB_CAN_Tx_Header.StdId = 0x1;
	FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

	memcpy(FEB_CAN_Tx_Data, &data, sizeof(data));


	// Configure FEB_CAN_Tx_Data
    // Write Code Here

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(hcan, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		// Code Error - Shutdown
		printf("Unable to send CAN message");
	}
}

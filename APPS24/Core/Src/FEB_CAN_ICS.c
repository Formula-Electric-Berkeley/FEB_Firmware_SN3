// **************************************** Includes & External ****************************************

#include "FEB_CAN_ICS.h"

// *********************************** Struct ************************************

typedef struct ICS_MESSAGE_TYPE {
    ICS_READY_TO_DRIVE_TYPE ready_to_drive;
} ICS_MESSAGE_TYPE;
ICS_MESSAGE_TYPE ICS_MESSAGE;

// **************************************** Functions ****************************************
bool FEB_CAN_ICS_Ready(){
	return ICS_MESSAGE.ready_to_drive == 1;
}

// ***** CAN FUNCTIONS ****

uint8_t FEB_CAN_ICS_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
	uint16_t ids[] = {FEB_CAN_ID_ICS_BUTTON_STATE};

	for (uint8_t i = 0; i < 1; i++) {
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
			//Code Error - shutdown
		}
	}

	return filter_bank;
}

void FEB_CAN_ICS_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t *RxData) {
    switch (pHeader -> StdId){
        case FEB_CAN_ID_ICS_BUTTON_STATE :
        	ICS_MESSAGE.ready_to_drive = RxData[1];
            break;
    }
}

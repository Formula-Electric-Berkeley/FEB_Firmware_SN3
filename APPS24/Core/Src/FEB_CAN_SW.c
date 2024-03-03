// **************************************** Includes & External ****************************************

#include "FEB_CAN_SW.h"

// *********************************** Struct ************************************

typedef struct SW_MESSAGE_TYPE {
    SW_READY_TO_DRIVE_TYPE ready_to_drive;
    SW_COOLANT_PUMP_TYPE coolant_pump;
    SW_ACUMULATOR_FANS_TYPE acumulator_fans;
    SW_EXTRA_TYPE extra;
} SW_MESSAGE_TYPE;
SW_MESSAGE_TYPE SW_MESSAGE;

// **************************************** Functions ****************************************
bool FEB_CAN_SW_Ready(){
	return SW_MESSAGE.ready_to_drive == 1;
}

// ***** CAN FUNCTIONS ****

uint8_t FEB_CAN_SW_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
	uint16_t ids[] = {FEB_CAN_ID_SW_READY_TO_DRIVE};

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

//TODO: check if "datalength" is 4
void FEB_CAN_SW_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t *RxData) {
    switch (pHeader -> StdId){
        case FEB_CAN_ID_SW_READY_TO_DRIVE :
            memcpy(&(SW_MESSAGE.ready_to_drive), RxData, 4);
            break;
    }
}

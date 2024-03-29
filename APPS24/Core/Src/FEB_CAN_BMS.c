// ********************************** Includes & External **********************************

#include "FEB_CAN_BMS.h"

// *********************************** Struct ************************************

typedef struct BMS_MESSAGE_TYPE {
    uint16_t temp;
    uint8_t state;
} BMS_MESSAGE_TYPE;
BMS_MESSAGE_TYPE BMS_MESSAGE;

// **************************************** Functions ****************************************

uint16_t FEB_CAN_BMS_getTemp(){
	return BMS_MESSAGE.temp;
}

uint8_t FEB_CAN_BMS_getState(){
	return BMS_MESSAGE.state;
}



// ***** CAN FUNCTIONS ****

uint8_t FEB_CAN_BMS_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
	uint16_t ids[] = {FEB_CAN_ID_BMS_TEMPERATURE, FEB_CAN_ID_BMS_STATE};

	for (uint8_t i = 0; i < 2; i++) {
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

void FEB_CAN_BMS_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t *RxData) {
    switch (pHeader -> StdId){
        case FEB_CAN_ID_BMS_TEMPERATURE :
            memcpy(&(BMS_MESSAGE.temp), RxData, 4);
            break;
        case FEB_CAN_ID_BMS_STATE :
            memcpy(&(BMS_MESSAGE.state), RxData+4, 2);
        	break;
    }
}




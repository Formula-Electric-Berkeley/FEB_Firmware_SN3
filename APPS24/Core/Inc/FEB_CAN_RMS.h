
#ifndef INC_FEB_CAN_RMS_H_
#define INC_FEB_CAN_RMS_H_

// ********************************** Includes **********************************

#include "FEB_CAN_ID.h"

// ********************************** Functions **********************************
void FEB_CAN_RMS_Init(void);
uint8_t FEB_CAN_RMS_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void Store_RMS_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length);


#endif /* INC_FEB_CAN_RMS_H_ */

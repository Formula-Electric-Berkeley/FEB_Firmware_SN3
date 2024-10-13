#ifndef INC_FEB_CAN_BMS_H_
#define INC_FEB_CAN_BMS_H_

// ********************************** Includes **********************************

#include "stm32f4xx_hal.h"
#include "FEB_CAN_ID.h"
#include <stdio.h>


// ********************************** Functions **********************************

uint16_t FEB_CAN_BMS_getTemp();
uint8_t FEB_CAN_BMS_getState();
uint8_t FEB_CAN_BMS_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void FEB_CAN_BMS_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t *RxData);



#endif /* INC_FEB_CAN_BMS_H_ */

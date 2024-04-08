#ifndef INC_FEB_CAN_ICS_H_
#define INC_FEB_CAN_ICS_H_

// ******************************** Includes ********************************

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "FEB_CAN_ID.h"



#define ICS_READY_TO_DRIVE_TYPE uint8_t

// ******************************** Functions ********************************
bool FEB_CAN_ICS_Ready();
uint8_t FEB_CAN_ICS_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void FEB_CAN_ICS_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t *RxData);

#endif /* INC_FEB_CAN_ICS_H_ */

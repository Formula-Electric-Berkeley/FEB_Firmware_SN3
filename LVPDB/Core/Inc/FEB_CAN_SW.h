#ifndef INC_FEB_CAN_SW_H_
#define INC_FEB_CAN_SW_H_


#include <stdint.h>
#include <string.h>

#include "FEB_CAN_ID.h"

#include "stm32f4xx_hal.h"

typedef struct {
	uint8_t ready_to_drive;
} FEB_CAN_SW_Message_t;


void FEB_CAN_APPS_Str_Msg(CAN_RxHeaderTypeDef *FEB_CAN_Rx_Header, uint8_t FEB_CAN_Rx_Data[]);
uint8_t FEB_CAN_APPS_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);

#endif /* INC_FEB_CAN_SW_H_ */

#ifndef INC_FEB_CAN_H_
#define INC_FEB_CAN_H_

#include "stm32f4xx_hal.h"

void FEB_CAN_Init(CAN_HandleTypeDef *hcan);
void FEB_CAN_Transmit(CAN_HandleTypeDef *hcan);

#endif

#ifndef INC_FEB_CAN_ICS_H_
#define INC_FEB_CAN_ICS_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"

uint8_t FEB_CAN_ICS_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void FEB_CAN_ICS_Store_Msg(CAN_RxHeaderTypeDef *FEB_CAN_Rx_Header, uint8_t FEB_CAN_Rx_Data[]);
bool FEB_CAN_ICS_Ready_To_Drive(void);
uint8_t FEB_CAN_ICS_Data();

#endif /* INC_FEB_CAN_ICS_H_ */

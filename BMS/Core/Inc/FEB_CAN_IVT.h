#ifndef INC_FEB_CAN_IVT_H_
#define INC_FEB_CAN_IVT_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"

uint8_t FEB_CAN_IVT_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void FEB_CAN_IVT_Store_Msg(CAN_RxHeaderTypeDef* rx_header, uint8_t rx_data[]);
void FEB_CAN_IVT_Process(void);

#endif /* INC_FEB_CAN_IVT_H_ */

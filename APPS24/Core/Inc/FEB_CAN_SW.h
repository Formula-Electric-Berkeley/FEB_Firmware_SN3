#ifndef INC_FEB_CAN_SW_H_
#define INC_FEB_CAN_SW_H_

// ******************************** Includes ********************************

#include "FEB_CAN_ID.h"
#include <stdint.h>
#include <stdbool.h>


#define SW_READY_TO_DRIVE_TYPE uint8_t
#define SW_COOLANT_PUMP_TYPE uint8_t
#define SW_ACUMULATOR_FANS_TYPE uint8_t
#define SW_EXTRA_TYPE uint8_t

// ******************************** Functions ********************************
bool FEB_CAN_SW_Ready();
uint8_t FEB_CAN_SW_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void FEB_CAN_SW_Store_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length);

#endif /* INC_FEB_CAN_SW_H_ */

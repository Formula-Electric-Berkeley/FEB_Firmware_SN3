#ifndef INC_FEB_CHARGE_H_
#define INC_FEB_CHARGE_H_

// ******************************** Includes ********************************

#include <FEB_SM_2.h>
#include "FEB_CAN_ID.h"
#include "FEB_Const.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"

// ********************************** Functions **********************************

// CAN
uint8_t FEB_CAN_Charger_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_Assignment, uint8_t filter_bank);
void FEB_CAN_Charger_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t RxData[]);

// Interface
void FEB_CAN_Charger_Init(void);
void FEB_CAN_Charger_Process(void);
void FEB_CAN_Charger_Start_Charge(void);
void FEB_CAN_Charger_Stop_Charge(void);

#endif /* INC_FEB_CHARGE_H_ */

#ifndef INC_FEB_CAN_IVT_H_
#define INC_FEB_CAN_IVT_H_

// ******************************** Includes ********************************

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

#include "stm32f4xx_hal.h"

#include "FEB_CAN_ID.h"
#include "FEB_Constant.h"
#include "FEB_LTC6811.h"
#include "FEB_State.h"
#include "FEB_UART.h"

// ******************************** Type Definitions ********************************

typedef struct {
    bool current;
    bool voltage_1;
    bool voltage_2;
    bool voltage_3;
} FEB_CAN_IVT_Flag_t;

typedef struct FEB_CAN_IVT_MESSAGE_TYPE {
	int32_t current_mA;
	int32_t voltage_1_mV;
	int32_t voltage_2_mV;
	int32_t voltage_3_mV;
} FEB_CAN_IVT_Message_t;

#endif /* INC_FEB_CAN_IVT_H_ */

// ******************************** Functions ********************************

uint8_t FEB_CAN_IVT_Filter(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void FEB_CAN_IVT_Store_Msg(CAN_RxHeaderTypeDef* rx_header, uint8_t rx_data[]);
void FEB_CAN_IVT_Process(void);
void FEB_CAN_IVT_UART_Transmit(void);


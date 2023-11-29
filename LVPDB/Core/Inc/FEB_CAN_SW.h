#ifndef INC_FEB_CAN_SW_H_
#define INC_FEB_CAN_SW_H_

#include <stdint.h>

#include "FEB_CAN_ID.h"

#include "stm32f4xx_hal.h"

typedef struct FEB_CAN_SW_MESSAGE_TYPE {
	uint8_t READY_TO_DRIVE;
} FEB_CAN_SW_Message_t;

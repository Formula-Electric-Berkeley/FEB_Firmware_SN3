/*
 * FEB_CAN_Store.h
 *
 *  Created on: Dec 3, 2023
 *      Author: viploverahate
 */

#ifndef INC_FEB_STORE_H_
#define INC_FEB_STORE_H_

// **************************************** Includes ****************************************

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "FEB_CAN_ID.h"
#include "string.h"



//************************* SETTINGS ********************
typedef uint32_t AddressIdType;

/*** MESSSAGE BUFFER ***/
#define SW_READY_TO_DRIVE_TYPE uint8_t
#define SW_COOLANT_PUMP_TYPE uint8_t
#define SW_ACUMULATOR_FANS_TYPE uint8_t
#define SW_EXTRA_TYPE uint8_t

typedef struct RMS_MESSAGE_TYPE {
    int16_t HV_Bus_Voltage;
    int16_t Motor_Speed;
} RMS_MESSAGE_TYPE;

typedef struct SW_MESSAGE_TYPE {
    SW_READY_TO_DRIVE_TYPE ready_to_drive;
    SW_COOLANT_PUMP_TYPE coolant_pump;
    SW_ACUMULATOR_FANS_TYPE acumulator_fans;
    SW_EXTRA_TYPE extra;
} SW_MESSAGE_TYPE;

extern SW_MESSAGE_TYPE SW_MESSAGE;
extern RMS_MESSAGE_TYPE RMS_MESSAGE;

// **************************************** Functions ****************************************

void FEB_Store_RMS_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length);
void FEB_Store_SW_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length);


#endif /* INC_FEB_STORE_H_ */

/*
 * FEB_CAN_Store.c
 *
 *  Created on: Dec 3, 2023
 *      Author: viploverahate
 */
//************************ INCLUDES *********************

#include "FEB_Store.h"

SW_MESSAGE_TYPE SW_MESSAGE;
RMS_MESSAGE_TYPE RMS_MESSAGE;

//extern SW_MESSAGE_TYPE SW_MESSAGE;
//extern RMS_MESSAGE_TYPE RMS_MESSAGE;

//************************ FUNCTIONS ********************

//RMS Store Functions
void FEB_Store_RMS_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
	switch (RxId){
		case FEB_CAN_ID_RMS_VOLTAGE:
			memcpy(&(RMS_MESSAGE.HV_Bus_Voltage), RxData, 2);
			break;
		case FEB_CAN_ID_RMS_MOTOR:
			memcpy(&(RMS_MESSAGE.Motor_Speed), RxData+2, 2);
			break;
	}
}

//SW Store Functions

#define SW_READY_TO_DRIVE_TYPE uint8_t
#define SW_COOLANT_PUMP_TYPE uint8_t
#define SW_ACUMULATOR_FANS_TYPE uint8_t
#define SW_EXTRA_TYPE uint8_t



void FEB_Store_SW_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
    switch (RxId){
        case FEB_CAN_ID_SW_READY_TO_DRIVE:
            memcpy(&(SW_MESSAGE.ready_to_drive), RxData, data_length);
            break;
        case FEB_CAN_ID_SW_COOLANT_PUMP:
            memcpy(&(SW_MESSAGE.coolant_pump), RxData, data_length);
            break;
        /*case SW_ACUMULATOR_FANS:
            memcpy(&(SW_MESSAGE.acumulator_fans), RxData, data_length);
            break;
        case SW_EXTRA:
            memcpy(&(SW_MESSAGE.extra), RxData, data_length);
            break;*/
        // implement above code when this is necessary
    }
}


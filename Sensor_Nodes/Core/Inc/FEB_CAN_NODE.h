#ifndef INC_FEB_CAN_NODE_H_
#define INC_FEB_CAN_NODE_H_

#include "string.h"

/*** SETTINGS ***/
#define ID_LENGTH_BITS 11
#define BITS_PER_ID 7
#define BITS_PER_MESSAGE_TYPE 4

typedef uint32_t AddressIdType;
typedef uint8_t FilterArrayLength;

/*** EMERGENCY IDs ***/
#define EMERGENCY_ID 0b0000000
#define EMERGENCY_APPS_EMERGENCY 0b00000000000
#define EMERGENCY_SW_EMERGENCY 0b00000000001

/*** EMERGENCY MESSSAGE BUFFER ***/
#define EMERGENCY_APPS_EMERGENCY_TYPE uint8_t
#define EMERGENCY_SW_EMERGENCY_TYPE uint8_t

typedef struct EMERGENCY_MESSAGE_TYPE {
    EMERGENCY_APPS_EMERGENCY_TYPE apps_emergency;
    EMERGENCY_SW_EMERGENCY_TYPE sw_emergency;
} EMERGENCY_MESSAGE_TYPE;
EMERGENCY_MESSAGE_TYPE EMERGENCY_MESSAGE;

void Store_EMERGENCY_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
    switch (RxId){
        case EMERGENCY_APPS_EMERGENCY:
            memcpy(&(EMERGENCY_MESSAGE.apps_emergency), RxData, data_length);
            break;
        case EMERGENCY_SW_EMERGENCY:
            memcpy(&(EMERGENCY_MESSAGE.sw_emergency), RxData, data_length);
            break;
    }
}

/*** BMS IDs ***/
#define BMS_ID 0b0000001
#define BMS_TEMPERATURE 0b00000010000
#define BMS_VOLTAGE 0b00000010001

/*** BMS MESSSAGE BUFFER ***/
#define BMS_TEMPERATURE_TYPE float
#define BMS_VOLTAGE_TYPE float

typedef struct BMS_MESSAGE_TYPE {
    BMS_TEMPERATURE_TYPE temperature;
    BMS_VOLTAGE_TYPE voltage;
} BMS_MESSAGE_TYPE;
BMS_MESSAGE_TYPE BMS_MESSAGE;

void Store_BMS_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
    switch (RxId){
        case BMS_TEMPERATURE:
            memcpy(&(BMS_MESSAGE.temperature), RxData, data_length);
            break;
        case BMS_VOLTAGE:
            memcpy(&(BMS_MESSAGE.voltage), RxData, data_length);
            break;
    }
}

/*** SW IDs ***/
#define SW_ID 0b0000010
#define SW_READY_TO_DRIVE 0b00000100000
#define SW_COOLANT_PUMP 0b00000100001
#define SW_ACUMULATOR_FANS 0b00000100010
#define SW_EXTRA 0b00000100011

/*** SW MESSSAGE BUFFER ***/
#define SW_READY_TO_DRIVE_TYPE uint8_t
#define SW_COOLANT_PUMP_TYPE uint8_t
#define SW_ACUMULATOR_FANS_TYPE uint8_t
#define SW_EXTRA_TYPE uint8_t

typedef struct SW_MESSAGE_TYPE {
    SW_READY_TO_DRIVE_TYPE ready_to_drive;
    SW_COOLANT_PUMP_TYPE coolant_pump;
    SW_ACUMULATOR_FANS_TYPE acumulator_fans;
    SW_EXTRA_TYPE extra;
} SW_MESSAGE_TYPE;
SW_MESSAGE_TYPE SW_MESSAGE;

void Store_SW_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
    switch (RxId){
        case SW_READY_TO_DRIVE:
            memcpy(&(SW_MESSAGE.ready_to_drive), RxData, data_length);
            break;
        case SW_COOLANT_PUMP:
            memcpy(&(SW_MESSAGE.coolant_pump), RxData, data_length);
            break;
        case SW_ACUMULATOR_FANS:
            memcpy(&(SW_MESSAGE.acumulator_fans), RxData, data_length);
            break;
        case SW_EXTRA:
            memcpy(&(SW_MESSAGE.extra), RxData, data_length);
            break;
    }
}

/*** APPS IDs ***/
#define APPS_ID                 0b0000011
#define APPS_ACCELERATOR1_PEDAL 0b00000110000
#define APPS_ACCELERATOR2_PEDAL 0b00000110001
#define APPS_BRAKE_PEDAL        0b00000110010
#define APPS_TORQUE             0b00000110011
#define APPS_TPS                0b00000110100

/*** APPS MESSSAGE BUFFER ***/
#define APPS_ACCELERATOR1_PEDAL_TYPE float
#define APPS_ACCELERATOR2_PEDAL_TYPE float
#define APPS_BRAKE_PEDAL_TYPE float
#define APPS_TORQUE_TYPE float

typedef struct APPS_MESSAGE_TYPE {
    APPS_ACCELERATOR1_PEDAL_TYPE accelerator1_pedal;
    APPS_ACCELERATOR2_PEDAL_TYPE accelerator2_pedal;
    APPS_BRAKE_PEDAL_TYPE brake_pedal;
    APPS_TORQUE_TYPE torque;
    float current;
} APPS_MESSAGE_TYPE;
APPS_MESSAGE_TYPE APPS_MESSAGE;




void Store_APPS_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
    switch (RxId){
        case APPS_ACCELERATOR1_PEDAL:
            memcpy(&(APPS_MESSAGE.accelerator1_pedal), RxData, data_length);
            break;
        case APPS_ACCELERATOR2_PEDAL:
            memcpy(&(APPS_MESSAGE.accelerator2_pedal), RxData, data_length);
            break;
        case APPS_BRAKE_PEDAL:
            memcpy(&(APPS_MESSAGE.brake_pedal), RxData, data_length);
            break;
        case APPS_TORQUE:
            memcpy(&(APPS_MESSAGE.torque), RxData, data_length);
            break;
        case APPS_TPS:
        	memcpy(&(APPS_MESSAGE.current), RxData, data_length);
        	break;
    }
}



/*** RMS ID's ***/
#define RMS_ID 0b0001010
#define RMS_VOLTAGE_INFO 0b00010100111
#define RMS_MOTOR_INFO   0b00010100101

typedef struct RMS_MESSAGE_TYPE {
    int16_t HV_Bus_Voltage;
    int16_t Motor_Speed;
} RMS_MESSAGE_TYPE;
RMS_MESSAGE_TYPE RMS_MESSAGE;

void Store_RMS_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
	switch (RxId){
		case RMS_VOLTAGE_INFO:
			memcpy(&(RMS_MESSAGE.HV_Bus_Voltage), RxData, 2);
			break;
		case RMS_MOTOR_INFO:
			memcpy(&(RMS_MESSAGE.Motor_Speed), RxData+2, 2);
			break;
	}
}

/*** LVPDB IDs ***/
#define LVPDB_ID 0b0000100
#define LVPDB_LV 0b00001000000
#define LVPDB_CP 0b00001000001
#define LVPDB_AF 0b00001000010
#define LVPDB_EX 0b00001000011

typedef struct LVPDB_MESSAGE_TYPE {
    float lv_current;
    float cp_current;
    float af_current;
    float ex_current;
} LVPDB_MESSAGE_TYPE;
LVPDB_MESSAGE_TYPE LVPDB_MESSAGE;

void Store_LVPDB_Msg(AddressIdType RxId, uint8_t *RxData, uint32_t data_length) {
	switch (RxId){
		case LVPDB_LV:
			memcpy(&(LVPDB_MESSAGE.lv_current), RxData, data_length);
			break;
		case LVPDB_CP:
			memcpy(&(LVPDB_MESSAGE.cp_current), RxData, data_length);
			break;
		case LVPDB_AF:
			memcpy(&(LVPDB_MESSAGE.af_current), RxData, data_length);
			break;
		case LVPDB_EX:
			memcpy(&(LVPDB_MESSAGE.ex_current), RxData, data_length);
			break;
	}
}



/*** RX Arrays ***/
const AddressIdType BMS_RX_ID[] = {SW_ID};
const FilterArrayLength BMS_RX_NUM = 1;

const AddressIdType SW_RX_ID[] = {APPS_ID};
const FilterArrayLength SW_RX_NUM = 1;

const AddressIdType APPS_RX_ID[] = {BMS_ID, SW_ID, RMS_ID};
const FilterArrayLength APPS_RX_NUM = 3;

const AddressIdType LVPDB_RX_ID[] = {SW_ID, APPS_ID};
const FilterArrayLength LVPDB_RX_NUM = 2;

const AddressIdType* assign_filter_array(AddressIdType NODE_ID) {
    switch(NODE_ID) {
        case BMS_ID:
            return BMS_RX_ID;
            break;
        case SW_ID:
            return SW_RX_ID;
            break;
        case APPS_ID:
            return APPS_RX_ID;
            break;
        case LVPDB_ID:
            return LVPDB_RX_ID;
            break;
    }
    return 0;
}

FilterArrayLength assign_filter_array_legnth(AddressIdType NODE_ID) {
    switch(NODE_ID) {
        case BMS_ID:
            return BMS_RX_NUM;
            break;
        case SW_ID:
            return SW_RX_NUM;
            break;
        case APPS_ID:
            return APPS_RX_NUM;
            break;
        case LVPDB_ID:
            return LVPDB_RX_NUM;
            break;
    }
    return 0;
}

void store_msg(CAN_RxHeaderTypeDef *pHeader, uint8_t RxData[]) {
    switch(pHeader->StdId >> BITS_PER_MESSAGE_TYPE) {
        case EMERGENCY_ID:
            Store_EMERGENCY_Msg(pHeader->StdId, RxData, pHeader->DLC);
            break;
        case BMS_ID:
            Store_BMS_Msg(pHeader->StdId, RxData, pHeader->DLC);
            break;
        case SW_ID:
            Store_SW_Msg(pHeader->StdId, RxData, pHeader->DLC);
            break;
        case APPS_ID:
            Store_APPS_Msg(pHeader->StdId, RxData, pHeader->DLC);
            break;
        case RMS_ID:
        	Store_RMS_Msg(pHeader->StdId, RxData, pHeader->DLC);
        	break;
        case LVPDB_ID:
        	Store_LVPDB_Msg(pHeader->StdId, RxData, pHeader->DLC);
        	break;
    }
}

#endif /* INC_FEB_CAN_H_ */

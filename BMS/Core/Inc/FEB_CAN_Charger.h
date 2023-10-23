#ifndef INC_FEB_CHARGE_H_
#define INC_FEB_CHARGE_H_

// ********************************** Includes **********************************

#include <inttypes.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

#include "FEB_CAN_ID.h"
#include "FEB_Constant.h"
#include "FEB_State.h"


// ********************************** Type Definitions **********************************

typedef struct {
	uint16_t max_voltage_100mV;
	uint16_t max_current_100mA;
	uint8_t control;	// 0 (start charging), 1 (stop charging)
} FEB_CAN_Charger_BMS_Message_t;

typedef struct {
	uint16_t operating_voltage_100mV;
	uint16_t operating_current_100mA;
	uint8_t status;
} FEB_CAN_Charger_Charger_Message_t;

// ********************************** Functions **********************************

// CAN
uint8_t FEB_CAN_Charger_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_Assignment, uint8_t filter_bank);
void FEB_CAN_Charger_Transmit(void);
void FEB_CAN_Charger_Store_Msg(CAN_RxHeaderTypeDef* pHeader, uint8_t RxData[]);

// Interface
void FEB_CAN_Charger_Validate_Status(uint8_t status);
void FEB_CAN_Charger_Process(CAN_HandleTypeDef* hcan);
void FEB_CAN_Charger_Charge(void);
void FEB_CAN_Charger_Stop_Charge(void);

#endif /* INC_FEB_CHARGE_H_ */

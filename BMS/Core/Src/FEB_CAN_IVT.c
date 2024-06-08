#include "FEB_CAN_IVT.h"
#include "FEB_CAN_Library/FEB_CAN_ID.h"
#include "FEB_Config.h"
#include "FEB_Const.h"
#include "FEB_LTC6811.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"

extern UART_HandleTypeDef huart2;

typedef struct {
	int32_t current_mA;
	int32_t voltage_1_mV;
	int32_t voltage_2_mV;
	int32_t voltage_3_mV;
} IVT_message_t;
typedef struct {
	bool current;
	bool voltage_1;
	bool voltage_2;
	bool voltage_3;
} IVT_message_flag_t;
static IVT_message_t IVT_message;				// IVT values
static IVT_message_flag_t IVT_message_flag;		// Flags indicating new IVT messages

/* ******** CAN Functions ******** */

uint8_t FEB_CAN_IVT_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
	uint16_t ids[] = {FEB_CAN_ID_IVT_CURRENT, FEB_CAN_ID_IVT_VOLTAGE_1, FEB_CAN_ID_IVT_VOLTAGE_2, FEB_CAN_ID_IVT_VOLTAGE_3};

	for (uint8_t i = 0; i < 4; i++) {
		CAN_FilterTypeDef filter_config;

	    // Standard CAN - 2.0A - 11 bit
	    filter_config.FilterActivation = CAN_FILTER_ENABLE;
		filter_config.FilterBank = filter_bank;
		filter_config.FilterFIFOAssignment = FIFO_assignment;
		filter_config.FilterIdHigh = ids[i] << 5;
		filter_config.FilterIdLow = 0;
		filter_config.FilterMaskIdHigh = 0xFFE0;
		filter_config.FilterMaskIdLow = 0;
		filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
		filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
		filter_config.SlaveStartFilterBank = 27;
	    filter_bank++;

		if (HAL_CAN_ConfigFilter(hcan, &filter_config) != HAL_OK) {
//			FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
		}
	}

	return filter_bank;
}

void FEB_CAN_IVT_Process(void) {
	if (IVT_message_flag.current) {
		IVT_message_flag.current = false;
		 int32_t current_mA = IVT_message.current_mA;
		 if (current_mA < FEB_Config_Get_Pack_Min_Current_mA() ||
			 current_mA > FEB_Config_Get_Pack_Max_Current_mA())
			 FEB_SM_Transition(FEB_SM_ST_FAULT);
	}
	if (IVT_message_flag.voltage_1) {
		IVT_message_flag.voltage_1 = false;
		if (FEB_SM_Get_Current_State() == FEB_SM_ST_PRECHARGE) {
			float voltage_V = (float) IVT_message.voltage_1_mV * 1e-3;
			float target_voltage_V = FEB_LTC6811_Get_Total_Voltage() * FEB_CONST_PRECHARGE_PCT * 1e-3;
			if (voltage_V >= target_voltage_V)
				FEB_SM_Transition(FEB_SM_ST_DRIVE_STANDBY);
		}
	}
	if (IVT_message_flag.voltage_2) {
		IVT_message_flag.voltage_2 = false;
		// ...
	}
	if (IVT_message_flag.voltage_3) {
		IVT_message_flag.voltage_3 = false;
		// ...
	}
}

void FEB_CAN_IVT_Store_Msg(CAN_RxHeaderTypeDef* rx_header, uint8_t rx_data[]) {
	switch(rx_header->StdId) {
	    case FEB_CAN_ID_IVT_CURRENT:
	    	IVT_message_flag.current = true;
	    	IVT_message.current_mA = ((rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5]) * -1;
			break;
	    case FEB_CAN_ID_IVT_VOLTAGE_1:
	    	IVT_message_flag.voltage_1 = true;
	    	IVT_message.voltage_1_mV = (rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5];
	    	break;
	    case FEB_CAN_ID_IVT_VOLTAGE_2:
	    	IVT_message_flag.voltage_2 = true;
	    	IVT_message.voltage_2_mV = (rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5];
	    	break;
	    case FEB_CAN_ID_IVT_VOLTAGE_3:
	    	IVT_message_flag.voltage_3 = true;
	    	IVT_message.voltage_3_mV = (rx_data[2] << 24) + (rx_data[3] << 16) + (rx_data[4] << 8) + rx_data[5];
	    	break;
	}
}

void FEB_CAN_IVT_UART_Transmit(void) {
	static char str[64];
	sprintf(str, "IVT %ld %ld %ld %ld\n",
			IVT_message.voltage_1_mV, IVT_message.voltage_2_mV, IVT_message.voltage_3_mV,
			IVT_message.current_mA);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
}

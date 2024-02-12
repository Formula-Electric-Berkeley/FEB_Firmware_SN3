// ********************************** Includes & External **********************************

#include "FEB_CAN_RMS.h"


// *********************************** Struct ************************************

struct {
  uint16_t torque;
  uint8_t enabled;
} RMSControl;

typedef struct RMS_MESSAGE_TYPE {
    int16_t HV_Bus_Voltage;
    int16_t Motor_Speed;
} RMS_MESSAGE_TYPE;
RMS_MESSAGE_TYPE RMS_MESSAGE;

// ********************************** Functions **********************************

void FEB_CAN_RMS_Setup(void){
	 RMSControl.enabled = 0;
	 RMSControl.torque= 0.0;
}

void FEB_CAN_RMS_Process(void){
	if (!RMSControl.enabled){
		FEB_CAN_RMS_Init();
		RMSControl.enabled = 1;
	}
}



// ***** CAN FUNCTIONS ****
//TODO: CREATE FEB_CAN_SW_Trasnmit Function and update below function
void FEB_RMS_Init(){
	// Clear fault in case inverter is powered up before disable command is sent
	uint8_t fault_clear_addr = 20;
	uint8_t fault_clear_data = 0;
	// param msg format (little-endian):
	// 0,1: param addr
	// 2: r/w cmd
	// 3: NA
	// 4,5: data
	// 6,7: NA
	uint8_t param_msg[8] = {fault_clear_addr, 0, 1, 0, fault_clear_data, 0, 0, 0};
	FEB_CAN_Transmit(&hcan1, 0x0C1, param_msg, 8);

	// send disable command to remove lockout
	uint8_t message_data[8] = {0,0,0,0,0,0,0};
	for (int i = 0; i < 10; i++) {
		FEB_CAN_Transmit(&hcan1, 0x0C0, message_data, 8);
		HAL_Delay(10);
	}

	// Select CAN msg to broadcast
	uint8_t param_addr = 148;
//	uint8_t CAN_active_msg_byte4 = 0b10100000; // motor position, input voltage
//	uint8_t CAN_active_msg_byte5 = 0b00010101; // flux info (dq axes), torque/timer info, internal states
	uint8_t CAN_active_msg_byte4 = 0xff; // literally log everything
	uint8_t CAN_active_msg_byte5 = 0xff;
	uint8_t broadcast_msg[8] = {param_addr, 0, 1, 0, CAN_active_msg_byte4, CAN_active_msg_byte5, 0, 0};
	FEB_CAN_Transmit(&hcan1, 0x0C1, broadcast_msg, 8);
}


uint8_t FEB_CAN_RMS_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank) {
	uint16_t ids[] = {FEB_CAN_ID_RMS_VOLTAGE, FEB_CAN_ID_RMS_MOTOR};

	for (uint8_t i = 0; i < 2; i++) {
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
			//Code Error - shutdown
		}
	}

	return filter_bank;
}
// @lovehate TODO: update below function to fit with call
void FEB_CAN_RMS_Store_Msg(AddressIdType RxId, uint8_t *RxData) {
	switch (RxId){
		case RMS_VOLTAGE_INFO:
			memcpy(&(RMS_MESSAGE.HV_Bus_Voltage), RxData, 2);
			break;
		case RMS_MOTOR_INFO:
			memcpy(&(RMS_MESSAGE.Motor_Speed), RxData+2, 2);
			break;
	}
}







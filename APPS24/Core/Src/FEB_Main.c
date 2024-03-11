// ********************************** Includes & External **********************************

#include "FEB_Main.h"

extern ADC_HandleTypeDef hadc1;;

// ********************************** Variables **********************************
char buf[128];
uint8_t buf_len; //stolen from Main_Setup (SN2)

// ********************************** Functions **********************************

void FEB_Main_Setup(void){
	HAL_ADCEx_InjectedStart(&hadc1); //@lovehate - where does this go
	FEB_Timer_Init();
	FEB_TPS2482_Setup();
	FEB_CAN_Init(); //FEB_CAN_Init() // The transceiver must be connected otherwise you get sent into an infinite loop
	FEB_CAN_RMS_Setup();
}

void FEB_Main_While(void){
	if (FEB_CAN_SW_Ready()){
		FEB_Normalized_updateAcc();
		FEB_CAN_RMS_Process();
	} else {
		FEB_Normalized_setAcc0();
		FEB_CAN_RMS_Disable();
	}
	FEB_HECS_update();
	FEB_Normalized_updateAcc();
	FEB_CAN_RMS_Torque();
	//TODO: FEB_CAN_sendBrake(); //TODO: create transmit function on CAN file for sending brake
	FEB_TPS2482_sendReadings(); //TODO: create transmit function for TPS2482
	FEB_CAN_RMS_torqueTransmit();
}

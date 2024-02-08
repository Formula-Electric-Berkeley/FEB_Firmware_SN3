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
	FEB_CAN_Init(); //FEB_CAN_Init() // The transceiver must be connected otherwise you get sent into an infinite loop
	FEB_RMS_Init();
}

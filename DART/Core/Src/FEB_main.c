#include "FEB_main.h"

void FEB_Init(void){
	FEB_CAN_Init();
	FEB_Fan_Init();
}

void FEB_Main_Loop(void){
	//Fan speed set by CAN receive function
	HAL_Delay(100);
}

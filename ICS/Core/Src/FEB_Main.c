// **************************************** Includes ****************************************

#include "FEB_Main.h"

// **************************************** Functions ****************************************

void FEB_Main_Setup(void) {
	FEB_IO_ICS_Init();

	FEB_UI_Init();

	FEB_CAN_Init();
}

void FEB_Main_Loop(void) {
	FEB_UI_Update();

	FEB_IO_ICS_Loop();

//	FEB_CAN_ICS_Transmit_Speed();

	HAL_Delay(20);
}

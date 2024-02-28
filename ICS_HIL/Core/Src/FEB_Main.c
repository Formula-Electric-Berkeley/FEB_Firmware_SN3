#include "FEB_Main.h"

// **************************************** Includes ****************************************

// **************************************** Functions ****************************************

void FEB_Main_Setup(void) {
	FEB_CAN_Init();
}

void FEB_Main_Loop(void) {
	FEB_UART_Transmit_Process();

	HAL_Delay(1000);
}

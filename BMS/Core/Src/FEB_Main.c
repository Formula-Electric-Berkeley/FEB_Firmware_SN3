// ******************************** Includes ********************************

#include "FEB_Main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {
	// Core
	FEB_Relay_Init();
	FEB_State_Init();

	// Other
	FEB_CAN_Init();
}

void FEB_Main_Loop(void) {
	// State-Relay
	FEB_State_Process();
	FEB_State_UART_Transmit();
	FEB_Relay_UART_Transmit();

	// Voltage

	// Temperature

	// IVT
	FEB_CAN_IVT_Process();
	FEB_CAN_IVT_UART_Transmit();

	// Charger

	// Balance

	// Fans

}

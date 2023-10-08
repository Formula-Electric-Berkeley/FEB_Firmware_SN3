// ******************************** Includes ********************************

#include "FEB_Main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {
	// Core
	FEB_Relay_Init();
	FEB_State_Init();

	// Other
}

void FEB_Main_Loop(void) {
	// State
	FEB_State_Process();
	FEB_State_UART_Transmit();

	// Relay
	FEB_Relay_UART_Transmit();

	// Charger

	// Fans

	// Balance

	// Voltage

	// Temperature


}

// ******************************** Includes ********************************

#include "FEB_Main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {
	// Core
	FEB_Relay_Init();
	FEB_State_Init();

	// Other
	FEB_CAN_Init();
	FEB_LTC6811_Init();
}

void FEB_Main_Loop(void) {
	// State-Relay
	FEB_State_Process();
	FEB_State_UART_Transmit();
	FEB_Relay_UART_Transmit();

	// Voltage
	FEB_LTC6811_Poll_Voltage();
	FEB_LTC6811_Validate_Voltage();
	FEB_LTC6811_UART_Transmit_Voltage();

	// Temperature
	FEB_LTC6811_Poll_Temperature();
	FEB_LTC6811_Validate_Temperature();
	FEB_LTC6811_UART_Transmit_Temperature();
	FEB_LTC6811_CAN_Transmit_Temperature();

	// IVT
	FEB_CAN_IVT_Process();
	FEB_CAN_IVT_UART_Transmit();

	// TODO: Charger
	// FEB_CAN_Charger_Process();

	// TODO: Balance

	// TODO: Fans

}

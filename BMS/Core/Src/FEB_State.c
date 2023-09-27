// ******************************** Includes & External ********************************

#include "FEB_State.h"

extern UART_HandleTypeDef huart2;

// ******************************** Global Variables ********************************

bool FEB_State_Balance = false;
bool FEB_State_Charge = false;
bool FEB_State_Debug = false;
bool FEB_State_Drive = false;
bool FEB_State_Precharge = false;
bool FEB_State_Shutdown = false;

// ******************************** Core Functions ********************************

void FEB_State_Set_Balance(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_State_Charge = false;
	FEB_State_Balance = true;
	FEB_State_Precharge = false;
	FEB_Relay_State_Balance();
}

void FEB_State_Set_Charge(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_State_Charge = true;
	FEB_State_Balance = false;
	FEB_State_Precharge = false;
	FEB_Relay_State_Charge();
}

void FEB_State_Set_Debug(void) {
	FEB_State_Debug = true;
}

void FEB_State_Reset_Debug(void) {
	FEB_State_Debug = false;
}

void FEB_State_Set_Drive(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_State_Precharge = false;
	FEB_State_Drive = true;
	FEB_Relay_State_Precharge();
}

void FEB_State_Set_Precharge(void) {
	if (FEB_State_Shutdown || FEB_State_Drive) {
		return;
	}
	FEB_State_Charge = false;
	FEB_State_Balance = false;
	FEB_State_Precharge = true;
	FEB_Relay_State_Precharge();
}

void FEB_State_Set_Shutdown(void) {
	FEB_State_Balance = false;
	FEB_State_Charge = false;
	FEB_State_Drive = false;
	FEB_State_Precharge = false;
	FEB_State_Shutdown = true;

	FEB_Relay_State_Shutdown();

	// Stop Charge
	// Stop Balance
	// Transmit Shutdown Message (UART, CAN)
}

// ******************************** Interface ********************************

void FEB_State_Init(void) {
	FEB_State_Set_Precharge();
}

void FEB_State_Process(void) {
	uint8_t total = (uint8_t) (FEB_State_Balance + FEB_State_Charge + FEB_State_Drive + FEB_State_Precharge + FEB_State_Shutdown);
	if (total != 1) {
		FEB_State_Set_Shutdown();
	}
}

void FEB_State_UART_Transmit(void) {
	if (!FEB_State_Debug) {
		return;
	}
	char UART_str[32];
	sprintf(UART_str, "%d %d %d %d %d %d\n", FEB_UART_BMS_State_ID, (uint8_t) FEB_State_Balance, (uint8_t) FEB_State_Charge,
											 (uint8_t) FEB_State_Drive, (uint8_t) FEB_State_Precharge, (uint8_t) FEB_State_Shutdown);
	HAL_UART_Transmit_IT(&huart2, (uint8_t*) UART_str, strlen(UART_str));
}

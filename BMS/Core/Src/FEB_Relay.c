// ******************************** Includes & External ********************************

#include "FEB_Relay.h"

extern UART_HandleTypeDef huart2;
extern bool FEB_State_Debug;
extern bool FEB_State_Drive;
extern bool FEB_State_Precharge;
extern bool FEB_State_Shutdown;

// ******************************** Global Variables ********************************

static uint8_t FEB_Relay_Shutdown_Value;
static uint8_t FEB_Relay_Precharge_Value;
static uint8_t FEB_Relay_Air_Plus_Value;

// ******************************** Static Function Declarations ********************************

static void FEB_Relay_Shutdown(uint8_t state);
static void FEB_Relay_Precharge(uint8_t state);
static void FEB_Relay_Air_Plus(uint8_t state);

// ******************************** Core Functions ********************************

static void FEB_Relay_Shutdown(uint8_t state) {
	if (state == FEB_Relay_Close) {
		// Close Shutdown Relay
		FEB_Relay_Shutdown_Value = 0;

	} else if (state == FEB_Relay_Open) {
		// Open Shutdown Relay
		FEB_Relay_Shutdown_Value = 1;
	}
}

static void FEB_Relay_Precharge(uint8_t state) {
	if (state == FEB_Relay_Close) {
		// Close Precharge Relay
		FEB_Relay_Precharge_Value = 0;
	} else if (state == FEB_Relay_Open) {
		// Open Precharge Relay
		FEB_Relay_Precharge_Value = 1;
	}
}

static void FEB_Relay_Air_Plus(uint8_t state) {
	if (state == FEB_Relay_Close) {
		// Close Air+ Relay
		FEB_Relay_Air_Plus_Value = 0;
	} else if (state == FEB_Relay_Open) {
		// Open Air+ Relay
		FEB_Relay_Air_Plus_Value = 1;
	}
}

// ******************************** Interface ********************************

void FEB_Relay_Init(void) {
	static uint8_t guard = 0;
	if (guard != 0) {
		return;
	}
	guard = 1;
	FEB_Relay_Shutdown(FEB_Relay_Close);
}

void FEB_Relay_State_Balance(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_Relay_Precharge(FEB_Relay_Open);
	FEB_Relay_Air_Plus(FEB_Relay_Open);
}

void FEB_Relay_State_Charge(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_Relay_Precharge(FEB_Relay_Open);
	FEB_Relay_Air_Plus(FEB_Relay_Close);
}

void FEB_Relay_State_Drive(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_Relay_Air_Plus(FEB_Relay_Close);
}

void FEB_Relay_State_Precharge(void) {
	if (FEB_State_Shutdown || FEB_State_Drive) {
		return;
	}
	FEB_Relay_Precharge(FEB_Relay_Close);
	FEB_Relay_Air_Plus(FEB_Relay_Open);
}

void FEB_Relay_State_Shutdown(void) {
	FEB_Relay_Shutdown(FEB_Relay_Open);
	FEB_Relay_Precharge(FEB_Relay_Open);
	FEB_Relay_Air_Plus(FEB_Relay_Open);
}

// ******************************** UART ********************************

void FEB_Relay_UART_Transmit(void) {
	if (!FEB_State_Debug) {
		return;
	}
	char UART_str[32];
	sprintf(UART_str, "%d %d %d\n", FEB_UART_Relay_State_ID, FEB_Relay_Precharge_Value, FEB_Relay_Air_Plus_Value);
	HAL_UART_Transmit_IT(&huart2, (uint8_t*) UART_str, strlen(UART_str));
}


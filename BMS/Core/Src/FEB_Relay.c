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
	if (state == FEB_RELAY_CLOSE) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		FEB_Relay_Shutdown_Value = 0;

	} else if (state == FEB_RELAY_OPEN) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		FEB_Relay_Shutdown_Value = 1;
	}
}

static void FEB_Relay_Precharge(uint8_t state) {
	if (state == FEB_RELAY_CLOSE) {
		// Close Precharge Relay
		FEB_Relay_Precharge_Value = 0;
	} else if (state == FEB_RELAY_OPEN) {
		// Open Precharge Relay
		FEB_Relay_Precharge_Value = 1;
	}
}

static void FEB_Relay_Air_Plus(uint8_t state) {
	if (state == FEB_RELAY_CLOSE) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		FEB_Relay_Air_Plus_Value = 0;
	} else if (state == FEB_RELAY_OPEN) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
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
	FEB_Relay_Shutdown(FEB_RELAY_CLOSE);
}

void FEB_Relay_State_Balance(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_Relay_Precharge(FEB_RELAY_OPEN);
	FEB_Relay_Air_Plus(FEB_RELAY_OPEN);
}

void FEB_Relay_State_Charge(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_Relay_Precharge(FEB_RELAY_OPEN);
	FEB_Relay_Air_Plus(FEB_RELAY_CLOSE);
}

void FEB_Relay_State_Drive(void) {
	if (FEB_State_Shutdown || !FEB_State_Precharge) {
		return;
	}
	FEB_Relay_Air_Plus(FEB_RELAY_CLOSE);
	HAL_Delay(100);
	FEB_Relay_Precharge(FEB_RELAY_OPEN);
}

void FEB_Relay_State_Precharge(void) {
	if (FEB_State_Shutdown || FEB_State_Drive) {
		return;
	}
	FEB_Relay_Precharge(FEB_RELAY_CLOSE);
	FEB_Relay_Air_Plus(FEB_RELAY_OPEN);
}

void FEB_Relay_State_Shutdown(void) {
	FEB_Relay_Shutdown(FEB_RELAY_OPEN);
	FEB_Relay_Precharge(FEB_RELAY_OPEN);
	FEB_Relay_Air_Plus(FEB_RELAY_OPEN);
}

// ******************************** Communication ********************************

void FEB_Relay_UART_Transmit(void) {
	if (!FEB_State_Debug) {
		return;
	}
	char UART_str[32];
	sprintf(UART_str, "%" PRIu8 " %" PRIu8 " %" PRIu8 "\n", FEB_UART_Relay_State_ID, FEB_Relay_Precharge_Value, FEB_Relay_Air_Plus_Value);
	HAL_UART_Transmit_IT(&huart2, (uint8_t*) UART_str, strlen(UART_str));
}


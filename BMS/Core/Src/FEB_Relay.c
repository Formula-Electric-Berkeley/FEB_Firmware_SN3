// ******************************** Includes & External ********************************

#include "FEB_Relay.h"

// ******************************** Static ********************************

typedef struct {
	FEB_Relay_ST_t shutdown;
	FEB_Relay_ST_t AIR_plus;
	FEB_Relay_ST_t precharge;
} relay_ST_t;
static relay_ST_t relay_ST;

// ******************************** Relay Control ********************************

void FEB_Relay_Shutdown(FEB_Relay_ST_t relay_state) {
	static uint8_t close_guard = 0;
	if (relay_state == FEB_RELAY_ST_CLOSE && close_guard == 0) {
		close_guard = 1;
		relay_ST.shutdown = FEB_RELAY_ST_CLOSE;
		// TODO: Close relay
	} else if (relay_state == FEB_RELAY_ST_OPEN) {
		relay_ST.shutdown = FEB_RELAY_ST_OPEN;
		// TODO: Open relay
	}
}

void FEB_Relay_AIR_Plus(FEB_Relay_ST_t relay_state) {
	if (relay_state == FEB_RELAY_ST_CLOSE) {
		relay_ST.AIR_plus = FEB_RELAY_ST_CLOSE;
		// TODO: Close relay
	} else if (relay_state == FEB_RELAY_ST_OPEN) {
		relay_ST.AIR_plus = FEB_RELAY_ST_OPEN;
		// TODO: Open relay
	}
}

void FEB_Relay_Precharge(FEB_Relay_ST_t relay_state) {
	if (relay_state == FEB_RELAY_ST_CLOSE) {
		relay_ST.precharge = FEB_RELAY_ST_CLOSE;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	} else if (relay_state == FEB_RELAY_ST_OPEN) {
		relay_ST.precharge = FEB_RELAY_ST_OPEN;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}

void FEB_Relay_Buzzer(FEB_Relay_ST_t relay_state) {
	if (relay_state == FEB_RELAY_ST_CLOSE) {
		relay_ST.precharge = FEB_RELAY_ST_CLOSE;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	} else if (relay_state == FEB_RELAY_ST_OPEN) {
		relay_ST.precharge = FEB_RELAY_ST_OPEN;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	}
}

// ******************************** Relay State ********************************

// Return state of the shutdown relay
FEB_Relay_ST_t FEB_Relay_ST_Shutdown(void) {
	return relay_ST.shutdown;
}

// Return state of the AIR+ relay
FEB_Relay_ST_t FEB_Relay_ST_AIR_Plus(void) {
	return relay_ST.AIR_plus;
}

// Return state of the precharge relay
FEB_Relay_ST_t FEB_Relay_ST_Precharge(void) {
	return relay_ST.precharge;
}

#include "FEB_Hw.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"

typedef struct {
	FEB_Hw_Relay_t bms_shutdown;
	FEB_Hw_Relay_t air_plus;
	FEB_Hw_Relay_t precharge;
} relay_state_t;

/* Stores all relay state. Requires synchronization. */
static relay_state_t relay_state;

/* Open/close shutdown relay. Assume serial access (SM lock held). */
void FEB_Hw_Set_BMS_Shutdown_Relay(FEB_Hw_Relay_t state) {
	static bool close_guard = false;
	if (state == FEB_HW_RELAY_CLOSE && !close_guard) {
		close_guard = true;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);		// BMS Shutdown Relay
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);	// Indicator light
	} else if (state == FEB_HW_RELAY_OPEN){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);	// BMS Shutdown Relay
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);		// Indicator light
	}
	relay_state.bms_shutdown = state;
}

/* Open/close AIR+ relay. Assume serial access  (SM lock held). */
void FEB_Hw_Set_AIR_Plus_Relay(FEB_Hw_Relay_t state) {
	if (state == FEB_HW_RELAY_CLOSE) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	} else if (state == FEB_HW_RELAY_OPEN){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	}
	relay_state.air_plus = state;
}

/* Open/close precharge relay. Assume serial access  (SM lock held). */
void FEB_Hw_Set_Precharge_Relay(FEB_Hw_Relay_t state) {
	if (state == FEB_HW_RELAY_CLOSE) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	} else if (state == FEB_HW_RELAY_OPEN){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	}
	relay_state.precharge = state;
}

/* Assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_Get_BMS_Shutdown_Relay(void) {
	return relay_state.bms_shutdown;
}

/* Assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_Get_AIR_Plus_Relay(void) {
	return relay_state.air_plus;
}

/* Assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_Get_Precharge_Relay(void) {
	return relay_state.precharge;
}

/* Read AIR-, assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_AIR_Minus_Sense(void) {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_SET)
		return FEB_HW_RELAY_CLOSE;
	return FEB_HW_RELAY_OPEN;
}

/* Read AIR+, assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_AIR_Plus_Sense(void) {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_SET)
		return FEB_HW_RELAY_CLOSE;
	return FEB_HW_RELAY_OPEN;
}

/* Read BMS shutdown sense, assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_BMS_Shutdown_Sense(void) {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_SET)
		return FEB_HW_RELAY_CLOSE;
	return FEB_HW_RELAY_OPEN;
}

/* Read IMD shutdown sense, assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_IMD_Shutdown_Sense(void) {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET)
		return FEB_HW_RELAY_CLOSE;
	return FEB_HW_RELAY_OPEN;
}

/* Read charge sense, assume SM lock held. */
FEB_Hw_Relay_t FEB_Hw_Charge_Sense(void) {
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET)
		return FEB_HW_RELAY_CLOSE;
	return FEB_HW_RELAY_OPEN;
}

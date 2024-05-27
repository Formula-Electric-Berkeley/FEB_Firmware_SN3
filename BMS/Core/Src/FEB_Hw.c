#include "FEB_Hw.h"

#include <stdbool.h>
#include "stm32f4xx_hal.h"

typedef struct {
	FEB_Hw_Relay_t shutdown;
	FEB_Hw_Relay_t AIR_plus;
	FEB_Hw_Relay_t precharge;
} relay_state_t;

/* Stores all relay state. Requires synchronization. */
relay_state_t relay_state;

/* Open/close shutdown relay. Assume serial access (SM lock held). */
void FEB_Hw_Shutdown_Relay(FEB_Hw_Relay_t state) {
	static bool close_guard = false;
	if (state == FEB_HW_RELAY_CLOSE && !close_guard) {
		close_guard = true;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
	} else if (state == FEB_HW_RELAY_OPEN){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	relay_state.shutdown = state;
}

/* Open/close AIR+ relay. Assume serial access  (SM lock held). */
void FEB_Hw_AIR_Plus_Relay(FEB_Hw_Relay_t state) {
	if (state == FEB_HW_RELAY_CLOSE) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	} else if (state == FEB_HW_RELAY_OPEN){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	}
	relay_state.AIR_plus = state;
}

/* Open/close precharge relay. Assume serial access  (SM lock held). */
void FEB_Hw_Precharge_Relay(FEB_Hw_Relay_t state) {
	if (state == FEB_HW_RELAY_CLOSE) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	} else if (state == FEB_HW_RELAY_OPEN){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	}
	relay_state.precharge = state;
}

/* Read shutdown circuit. Assume serial access  (SM lock held). */
FEB_Hw_Relay_t FEB_Hw_Read_Shutdown_Circuit(void) {
	GPIO_PinState pin_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12);
	if (pin_state == GPIO_PIN_SET)
		return FEB_HW_RELAY_CLOSE;
	else
		return FEB_HW_RELAY_OPEN;
}

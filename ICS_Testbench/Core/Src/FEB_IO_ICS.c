// **************************************** Includes ****************************************

#include "FEB_IO_ICS.h"

extern I2C_HandleTypeDef hi2c1;

static const uint16_t IOEXP_ADDR = 0x40;
static const uint32_t BTN_HOLD_TIME = 2000;

// **************************************** Functions ****************************************
uint32_t rtd_press_start_time;

void FEB_IO_ICS_Init(void) {
	uint8_t initial_io_exp_state = 0b11111110; // initialize RTD (P0) to low

	HAL_I2C_Master_Transmit(&hi2c1, IOEXP_ADDR, &initial_io_exp_state, sizeof(initial_io_exp_state), HAL_MAX_DELAY);
}

void FEB_IO_ICS_Loop(void) {
	uint8_t received_data;

	HAL_I2C_Master_Receive(&hi2c1, IOEXP_ADDR, &received_data, 1, HAL_MAX_DELAY);

	uint8_t button_state = ~received_data;

	button_state = set_n_bit(button_state, 0, 0);
	button_state = set_n_bit(button_state, 1, 0);

	uint8_t set_rtd_buzzer = 0;

	// Button 1 = Ready-to-Drive (RTD) button
	if (!(received_data & (1<<1))) {
		if ((HAL_GetTick() - rtd_press_start_time) >= BTN_HOLD_TIME) {
			button_state = (uint8_t) set_n_bit(button_state, 1, 1);
			set_rtd_buzzer = 1;
		}
	} else {
		button_state = (uint8_t) set_n_bit(button_state, 1, 0);
		rtd_press_start_time = HAL_GetTick();
		set_rtd_buzzer = 0;
	}

	// Button 2
	if (!(received_data & (1<<2))) {
		button_state = (uint8_t) set_n_bit(button_state, 2, 1);
	} else {
		button_state = (uint8_t) set_n_bit(button_state, 2, 0);
	}

	// Button 3
	if (!(received_data & (1<<3))) {
		button_state = (uint8_t) set_n_bit(button_state, 3, 1);
	} else {
		button_state = (uint8_t) set_n_bit(button_state, 3, 0);
	}

	// Button 4
	if (!(received_data & (1<<4))) {
		button_state = (uint8_t) set_n_bit(button_state, 4, 1);
	} else {
		button_state = (uint8_t) set_n_bit(button_state, 4, 0);
	}

	// Switch 1
	if (!(received_data & (1<<7))) {
		button_state = (uint8_t) set_n_bit(button_state, 5, 1);
	} else {
		button_state = (uint8_t) set_n_bit(button_state, 5, 0);
	}

	// Switch 2
	if (!(received_data & (1<<5))) {
		button_state = (uint8_t) set_n_bit(button_state, 6, 1);
	} else {
		button_state = (uint8_t) set_n_bit(button_state, 6, 0);
	}

	// Switch 3
	if (!(received_data & (1<<6))) {
		button_state = (uint8_t) set_n_bit(button_state, 7, 1);
	} else {
		button_state = (uint8_t) set_n_bit(button_state, 7, 0);
	}

	// display button state on UI
	char button_state_str[9];
	uint8_to_binary_string(button_state, button_state_str);
	lv_label_set_text(ui_buttonField, button_state_str);
}

uint8_t set_n_bit(uint8_t x, uint8_t n, uint8_t bit_value) {
	return (x & (~(1 << n))) | (bit_value << n);
}

void uint8_to_binary_string(uint8_t value, char *binary_string) {
    for (int i = 7; i >= 0; i--) {
    	binary_string[7 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    binary_string[8] = '\0';
}

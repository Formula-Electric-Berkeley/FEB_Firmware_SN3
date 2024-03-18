// **************************************** Includes ****************************************

#include "FEB_IO_ICS.h"

extern I2C_HandleTypeDef hi2c1;

static const uint16_t IOEXP_ADDR = 0x40;

// **************************************** Functions ****************************************

void FEB_IO_ICS_Init(void) {
	uint8_t initial_io_exp_state = 0b11111110; // initialize RTD (P0) to low

	HAL_I2C_Master_Transmit(&hi2c1, IOEXP_ADDR, &initial_io_exp_state, sizeof(initial_io_exp_state), HAL_MAX_DELAY);
}

void FEB_IO_ICS_Loop(void) {
	uint8_t received_data;

	HAL_I2C_Master_Receive(&hi2c1, IOEXP_ADDR, &received_data, 1, HAL_MAX_DELAY);

	uint8_t currButton = 0;

	if (!(received_data & (1<<1))) {
		currButton = 1;
	}

	if (!(received_data & (1<<2))) {
		currButton = 2;
	}

	if (!(received_data & (1<<3))) {
		currButton = 3;
	}

	if (!(received_data & (1<<4))) {
		currButton = 4;
	}

	if (currButton != 0) {
		char buttonStr[16] = "Button X Pressed";
		char buttonNumStr[4];

		sprintf(buttonNumStr, "%d", currButton);

		buttonStr[7] = buttonNumStr[0];

		lv_label_set_text(ui_buttonField, buttonStr);
	} else {
		lv_label_set_text(ui_buttonField, "No Buttons Pressed");
	}
}

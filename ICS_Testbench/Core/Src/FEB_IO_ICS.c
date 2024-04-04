// **************************************** Includes ****************************************

#include "FEB_IO_ICS.h"

extern I2C_HandleTypeDef hi2c1;

static const uint16_t IOEXP_ADDR = 0x40;

// **************************************** Functions ****************************************
uint32_t rtdPressStartTime;

void FEB_IO_ICS_Init(void) {
	uint8_t initial_io_exp_state = 0b11111110; // initialize RTD (P0) to low

	HAL_I2C_Master_Transmit(&hi2c1, IOEXP_ADDR, &initial_io_exp_state, sizeof(initial_io_exp_state), HAL_MAX_DELAY);
}

void FEB_IO_ICS_Loop(void) {
	uint8_t received_data;

	HAL_I2C_Master_Receive(&hi2c1, IOEXP_ADDR, &received_data, 1, HAL_MAX_DELAY);

//	uint8_t currButton = 0;

	uint8_t rtdButtonState = 0;
	uint8_t setRtdBuzzer = 0;
	uint32_t rtdHoldTimeMs = 2000;

	// Port 1 = Ready-to-Drive (RTD) button
	if (!(received_data & (1<<1))) {
		rtdButtonState = 1;

		if ((HAL_GetTick() - rtdPressStartTime) >= rtdHoldTimeMs) {
			setRtdBuzzer = 1;
			rtdButtonState = 0;
		}
	} else {
		rtdButtonState = 0;
		setRtdBuzzer = 0;
		rtdPressStartTime = HAL_GetTick();
	}

	if (setRtdBuzzer == 1) {
		lv_label_set_text(ui_buttonField, "RTD Button Held");
	} else if (rtdButtonState == 1) {
		lv_label_set_text(ui_buttonField, "RTD Button Pressed");
	} else {
		lv_label_set_text(ui_buttonField, "No Buttons Pressed");
	}

//	if (!(received_data & (1<<2))) {
//		currButton = 2;
//	}
//
//	if (!(received_data & (1<<3))) {
//		currButton = 3;
//	}
//
//	if (!(received_data & (1<<4))) {
//		currButton = 4;
//	}

//	if (currButton != 0) {
//		char buttonStr[16] = "Button X Pressed";
//		char buttonNumStr[4];
//
//		sprintf(buttonNumStr, "%d", currButton);
//
//		buttonStr[7] = buttonNumStr[0];
//
//		lv_label_set_text(ui_buttonField, buttonStr);
//	} else {
//		lv_label_set_text(ui_buttonField, "No Buttons Pressed");
//	}
}

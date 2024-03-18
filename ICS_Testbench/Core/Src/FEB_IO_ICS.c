// **************************************** Includes ****************************************

#include "FEB_IO_ICS.h"

extern I2C_HandleTypeDef hi2c1;

static const uint16_t IOEXP_ADDR = 0x40;

HAL_StatusTypeDef ret;

uint8_t initial_io_exp_state = 0b11111110; // initialize RTD (P0) to low
uint8_t received_data;

// **************************************** Functions ****************************************

void FEB_IO_ICS_Init(void) {
	HAL_I2C_Master_Transmit(&hi2c1, IOEXP_ADDR, &initial_io_exp_state, sizeof(initial_io_exp_state), HAL_MAX_DELAY);
}

void FEB_IO_ICS_Loop(void) {
	ret = HAL_I2C_Master_Receive(&hi2c1, IOEXP_ADDR, &received_data, 1, HAL_MAX_DELAY);
}

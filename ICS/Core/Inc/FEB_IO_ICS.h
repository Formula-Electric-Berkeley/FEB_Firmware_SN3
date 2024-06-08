#ifndef INC_FEB_IO_ICS_H_
#define INC_FEB_IO_ICS_H_

// **************************************** Includes ****************************************

#include "FEB_UI.h"
#include "FEB_CAN_BMS.h"

#include "stm32f4xx_hal.h"

// **************************************** Functions ****************************************

void FEB_IO_ICS_Init(void);

void FEB_IO_ICS_Loop(void);

static uint8_t set_n_bit(uint8_t num, uint8_t n, uint8_t bit_value);

static void uint8_to_binary_string(uint8_t value, char *binary_string);

#endif /* INC_FEB_IO_ICS_H_ */

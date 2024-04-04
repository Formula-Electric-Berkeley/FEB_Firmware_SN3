#ifndef INC_FEB_IMU_H_
#define INC_FEB_IMU_H_

#include "stm32f4xx_hal.h"

void IMU_ReadData(uint8_t reg_addr, uint8_t *data_buffer, uint16_t size);

#endif /* INC_FEB_IMU_H_ */

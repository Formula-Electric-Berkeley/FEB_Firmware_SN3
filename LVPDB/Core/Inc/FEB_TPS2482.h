/*
 * FEB_TPS2482.h
 *
 *  Created on: Apr 25, 2023
 *      Author: sshashi
 */

#ifndef INC_FEB_TPS2482_H_
#define INC_FEB_TPS2482_H_

#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdint.h"

#define LV_ADDR (0b1000000 << 1) //add these to header file

//Coolant pump Address
#define CP_ADDR (0b1000100 << 1)

// Accumulator Fan Address
#define AF_ADDR (0b1000101 << 1)

//Extra TPS address
#define EX_ADDR (0b1000001 << 1)

//Shutdown Address
#define SH_ADDR (0b1001000 << 1)


void FEB_SETUP_TPS2482(I2C_HandleTypeDef* hi2c, uint8_t DEV_ADDR, uint8_t CONFIG[], uint8_t CAL_REG[], uint8_t ALERT[], uint8_t LIMIT[]);

void FEB_TPS2482_shutdownIfError(I2C_HandleTypeDef * hi2c, uint8_t DEV_ADDR, GPIO_TypeDef * EN, uint16_t EN_NUM, GPIO_TypeDef * AL, uint16_t AL_NUM,
		GPIO_TypeDef * PG, uint16_t PG_NUM, float VMAX, float VMIN, float IMAX, float IMIN, float PMAX, float PMIN);

void FEB_TPS2482_pullLowIfOutOfBounds(I2C_HandleTypeDef * hi2c, uint8_t DEV_ADDR, GPIO_TypeDef * EN, uint16_t EN_NUM, float MAX, float MIN,
		uint8_t REG);

float FEB_TPS2482_PollBusCurrent(I2C_HandleTypeDef * hi2c, uint8_t DEV_ADDR);
float FEB_TPS2482_PollBusVoltage(I2C_HandleTypeDef * hi2c, uint8_t DEV_ADDR);

void FEB_TPS2482_Poll_Currents();



float FEB_TPS2482_Bus_Current();

float FEB_TPS2482_RF_Current();

float FEB_TPS2482_CP_Current();

float FEB_TPS2482_AF_Current();

#endif /* INC_FEB_TPS2482_H_ */
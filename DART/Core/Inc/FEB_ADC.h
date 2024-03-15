#ifndef INC_FEB_ADC_H_
#define INC_FEB_ADC_H_

#include "stm32f0xx_hal.h"
#include "main.h" //for error handler

//void FEB_ADC_Init(void);
uint32_t FEB_Read_Tachometer_Fan_1();
uint32_t FEB_Read_Tachometer_Fan_2();
uint32_t FEB_Read_Tachometer_Fan_3();
uint32_t FEB_Read_Tachometer_Fan_4();
uint32_t FEB_Read_Tachometer_Fan_5();

#endif /* INC_FEB_ADC_H_ */

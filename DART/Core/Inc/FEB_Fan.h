#ifndef INC_FEB_FAN_H_
#define INC_FEB_FAN_H_

#include <stm32f0xx_hal.h>
#include "FEB_PID.h"
#include "FEB_ADC.h"


void FEB_Update_PWM_All_Fans();
uint32_t FEB_Fan_Read_Tachometer_Fan_1();
void FEB_Fan_All_Speed_Set(uint8_t speed);
void FEB_Fan_1_Speed_Set(uint8_t speed);
void FEB_Fan_2_Speed_Set(uint8_t speed);
void FEB_Fan_3_Speed_Set(uint8_t speed);
void FEB_Fan_4_Speed_Set(uint8_t speed);
void FEB_Fan_5_Speed_Set(uint8_t speed);

void FEB_Update_PWM_Fan_1();
void FEB_Update_PWM_Fan_2();
void FEB_Update_PWM_Fan_3();
void FEB_Update_PWM_Fan_4();
void FEB_Update_PWM_Fan_5();


void FEB_Fan_PWM_Init();
void FEB_PID_Init_All();
void FEB_Fan_Init();
void FEB_Test_PWM();




#endif /* INC_FEB_FAN_H_ */

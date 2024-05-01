#ifndef INC_FEB_FAN_H_
#define INC_FEB_FAN_H_

#include <stm32f0xx_hal.h>
#include "FEB_main.h"

void FEB_Fan_CAN_Process(uint8_t FEB_CAN_Rx_Data[]);
void FEB_Update_Fan_Speed_Measurements();

void FEB_Update_PWM_All_Fans();
uint32_t FEB_Fan_Read_Tachometer_Fan_1();

uint16_t FEB_Get_F1_Speed();
uint16_t FEB_Get_F2_Speed();
uint16_t FEB_Get_F3_Speed();
uint16_t FEB_Get_F4_Speed();
uint16_t FEB_Get_F5_Speed();

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

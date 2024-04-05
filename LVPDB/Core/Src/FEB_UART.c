#include "FEB_UART.h"

extern FEB_CAN_APPS_Message_t FEB_CAN_APPS_Message;
extern FEB_CAN_SW_Message_t FEB_CAN_SW_Message;
extern float current_reading;
extern float ex_current_reading;
extern float cp_current_reading;
extern float apps_current_reading;

//char buf[128];
//int buf_len;

//buf_len = sprintf((char*)buf, "ready: %d, brake: %.3f\r\n", FEB_CAN_SW_Message.ready_to_drive, FEB_CAN_APPS_Message.brake_pedal);
//HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

//buf_len = sprintf((char*) buf, "Current Draw (LV, EX, CP, APPS): %.3f, %.3f, %.3f, %.3f\r\n", current_reading, ex_current_reading, cp_current_reading, apps_current_reading);
//HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

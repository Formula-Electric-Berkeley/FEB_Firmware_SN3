#ifndef INC_FEB_LTC6811_H_
#define INC_FEB_LTC6811_H_

#include "FEB_Config.h"

void FEB_LTC6811_Init(void);
void FEB_LTC6811_Poll_Voltage(void);
void FEB_LTC6811_Init_Balance(void);
void FEB_LTC6811_Balance_Process(void);
void FEB_LTC6811_Stop_Balance(void);
void FEB_LTC6811_Poll_Temperature(void);
uint32_t FEB_LTC6811_Get_Total_Voltage(void);
uint16_t FEB_LTC6811_Get_Min_Voltage(void);
void FEB_LTC6811_UART_Transmit(void);
void FEB_LTC6811_Cell_Data_CAN_Transmit(void);

#endif /* INC_FEB_LTC6811_H_ */

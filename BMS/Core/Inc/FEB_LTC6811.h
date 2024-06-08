#ifndef INC_FEB_LTC6811_H_
#define INC_FEB_LTC6811_H_

#include "FEB_Config.h"


/* ******** LTC6811 Configuration ******** */


/* ******** Functions ******** */

void FEB_LTC6811_Init(void);
void FEB_LTC6811_Poll_Voltage(void);
void FEB_LTC6811_Init_Balance(void);
void FEB_LTC6811_Balance_Process(void);
void FEB_LTC6811_Stop_Balance(void);
void FEB_LTC6811_Poll_Temperature(void);
uint32_t FEB_LTC6811_Get_Total_Voltage(void);
uint16_t FEB_LTC6811_Get_Min_Voltage(void);
void FEB_LTC6811_UART_Transmit(void);

// ******************************** Functions ********************************

//void FEB_LTC6811_Init(void);
//
//// Voltage functions
//void FEB_LTC6811_Poll_Voltage(void);
//void FEB_LTC6811_Validate_Voltage(void);
//float FEB_LTC6811_Get_Total_Voltage(void);
//void FEB_LTC6811_CAN_Transmit_Voltage(void);
//
//// Balance
//void FEB_LTC6811_Init_Balance(void);
//void FEB_LTC6811_Balance_Process(void);
//bool FEB_LTC6811_Balance_Complete(void);
//void FEB_LTC6811_Stop_Balance(void);
//
//// Temperature functions
//void FEB_LTC6811_Poll_Temperature(void);
//void FEB_LTC6811_Validate_Temperature(void);
//void FEB_LTC6811_CAN_Transmit_Temperature(void);

#endif /* INC_FEB_LTC6811_H_ */

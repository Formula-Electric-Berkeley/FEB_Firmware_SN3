#ifndef INC_FEB_RELAY_H_
#define INC_FEB_RELAY_H_

// ******************************** Includes ********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "FEB_UART.h"

// ******************************** Defines ********************************

#define FEB_RELAY_CLOSE 	0
#define FEB_RELAY_OPEN 		1

// ******************************** Functions ********************************

// Interface
void FEB_Relay_Init(void);
void FEB_Relay_State_Balance(void);
void FEB_Relay_State_Charge(void);
void FEB_Relay_State_Drive(void);
void FEB_Relay_State_Precharge(void);
void FEB_Relay_State_Shutdown(void);

// UART
void FEB_Relay_UART_Transmit(void);

#endif /* INC_FEB_RELAY_H_ */

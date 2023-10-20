// ******************************** Includes ********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "FEB_Relay.h"
#include "FEB_UART.h"

// ******************************** Definitions ********************************



// ******************************** Functions ********************************

// Core
void FEB_State_Set_Balance(void);
void FEB_State_Set_Charge(void);
void FEB_State_Set_Debug(void);
void FEB_State_Reset_Debug(void);
void FEB_State_Set_Drive(void);
void FEB_State_Set_Precharge(void);
void FEB_State_Set_Shutdown(char* msg);

// Interface
void FEB_State_Init(void);
void FEB_State_Process(void);
void FEB_State_UART_Transmit(void);

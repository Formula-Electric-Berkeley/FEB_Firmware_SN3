// ******************************** Includes ********************************

#include <stdbool.h>
#include <stdint.h>

#include "FEB_Relay.h"
#include "FEB_UART.h"

// ******************************** Definitions ********************************



// ******************************** Functions ********************************

// Core
void FEB_State_Set_Balance(void);
void FEB_State_Set_Charge(void);
void FEB_State_Set_Debug(void);
void FEB_State_Reset_Debug(void);
void FEB_State_Set_Precharge(void);
void FEB_State_Set_Shutdown(void);

// Interface
void FEB_State_Init(void);
void FEB_State_Process(void);

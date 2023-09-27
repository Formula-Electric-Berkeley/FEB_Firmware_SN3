#ifndef INC_FEB_RELAY_H_
#define INC_FEB_RELAY_H_

// ******************************** Includes ********************************

#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

// ******************************** Defines ********************************

#define FEB_Relay_Close 	0
#define FEB_Relay_Open 		1

// ******************************** Functions ********************************

void FEB_Relay_Init(void);
void FEB_Relay_State_Precharge(void);
void FEB_Relay_State_Balance(void);
void FEB_Relay_State_Charge(void);
void FEB_Relay_State_Drive(void);
void FEB_Relay_State_Shutdown(void);

#endif /* INC_FEB_RELAY_H_ */

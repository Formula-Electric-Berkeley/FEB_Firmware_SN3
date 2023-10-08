#ifndef INC_FEB_MAIN_H_
#define INC_FEB_MAIN_H_

// ******************************** Includes ********************************

#include "FEB_CAN.h"
#include "FEB_CAN_IVT.h"
#include "FEB_Relay.h"
#include "FEB_State.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void);
void FEB_Main_Loop(void);

#endif /* INC_FEB_MAIN_H_ */

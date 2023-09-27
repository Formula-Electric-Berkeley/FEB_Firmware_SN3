// ******************************** Includes ********************************

#include "FEB_Main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {
	// Core
	FEB_Relay_Init();
	FEB_State_Init();

	// Other
}

void FEB_Main_Loop(void) {
	FEB_State_Process();
}

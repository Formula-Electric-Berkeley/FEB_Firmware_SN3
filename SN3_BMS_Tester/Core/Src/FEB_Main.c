// ******************************** Includes ********************************

#include "FEB_Main.h"
#include "main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {

}

void FEB_Main_Loop(void) {
	//If Start Pressed:
	if (FEB_BMS_Tester_Hardware_Read_Start()) {
		//If Daughter:
		if (FEB_BMS_Tester_Hardware_Read_Mode()) {
			FEB_Daughter_Tester_Test_Daughter();
		//If Aux:
		} else {
			FEB_Aux_Tester_Test_Aux();
		}
	}
}

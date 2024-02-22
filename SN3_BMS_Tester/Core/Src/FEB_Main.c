// ******************************** Includes ********************************

#include "FEB_Main.h"
#include "main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {

}

void FEB_Main_Loop(void) {
	//If Start Pressed, we start testing process, based on mode: either aux or daughter
	if (FEB_BMS_Tester_Hardware_Read_Start() == FEB_CONSTANT_START) {

		bool mode = FEB_BMS_Tester_Hardware_Read_Mode();
		if (mode == FEB_CONSTANT_DAUGHTER_MODE) {
			FEB_Daughter_Tester_Test_Daughter();
		} else if (mode == FEB_CONSTANT_AUX_MODE){
			FEB_Aux_Tester_Test_Aux();
		}
	}
	HAL_Delay(100);
}

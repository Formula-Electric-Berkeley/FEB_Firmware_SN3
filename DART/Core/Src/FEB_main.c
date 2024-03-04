#include "FEB_main.h"

void FEB_Init(void){
	//FEB_ADC_Init();
	FEB_Fan_Init();

}

void FEB_Main_Loop(void){
	FEB_Update_PWM_All_Fans();
}

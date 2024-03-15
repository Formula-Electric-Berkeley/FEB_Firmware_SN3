#include "FEB_Main.h"

extern FEB_CAN_APPS_Message_t FEB_CAN_APPS_Message;

extern I2C_HandleTypeDef hi2c1;
extern CAN_HandleTypeDef hcan1;

extern int LV_ADDR;
extern int CP_ADDR;
extern int AF_ADDR;
extern int EX_ADDR;

extern float current_reading;
extern float ex_current_reading;
extern float cp_current_reading;

float apps_current_reading;

void FEB_Main_Setup(void) {

	FEB_CAN_Init();

	FEB_SETUP();
}


void FEB_Main_Loop(void) {

  FEB_Brake_Light_Control();

  FEB_Peripherals_Control();

  FEB_TPS2482_Poll_Currents();

  FEB_CAN_Transmit(&hcan1, LVPDB_LV_CURRENT, &current_reading);
  FEB_CAN_Transmit(&hcan1, LVPDB_EX_CURRENT, &ex_current_reading);
  FEB_CAN_Transmit(&hcan1, LVPDB_CP_CURRENT, &cp_current_reading);

  apps_current_reading = FEB_CAN_APPS_Message.current;

  FEB_UART();
}

// **************************************** Includes ****************************************

#include "FEB_UI.h"

extern FEB_CAN_ICS_Message_t FEB_CAN_ICS_Message;

// **************************************** Functions ****************************************

void FEB_UI_Init(void) {
	lv_init();

	screen_driver_init();
	touch_sensor_driver_init();

	ui_init();
}

void FEB_UI_Update(void) {
	lv_task_handler();

//	FEB_UI_Set_Values();
}

void FEB_UI_Set_Values(void) {
	char speedStr[20];

	sprintf(speedStr, "%d", FEB_CAN_ICS_Message.speed);
//	lv_label_set_text(ui_speedField, speedStr);
}

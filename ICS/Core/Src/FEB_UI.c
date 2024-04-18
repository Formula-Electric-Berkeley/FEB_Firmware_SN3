// **************************************** Includes ****************************************

#include "FEB_UI.h"

extern FEB_CAN_ICS_Message_t FEB_CAN_ICS_Message;
extern ICS_CAN_Rx_t ICS_CAN_Rx;

// **************************************** Functions ****************************************

void FEB_UI_Init(void) {
	lv_init();

	screen_driver_init();
	touch_sensor_driver_init();

	ui_init();
}

void FEB_UI_Update(void) {
	lv_task_handler();

	FEB_UI_Set_Values();
}

void FEB_UI_Set_Values(void) {
	char canIdStr[4];
	char dlcStr[4];
	char dataStr[32];

	sprintf(dataStr, "%x %x %x %x %x %x %x %x", ICS_CAN_Rx.data[0], ICS_CAN_Rx.data[1], ICS_CAN_Rx.data[2], ICS_CAN_Rx.data[3], ICS_CAN_Rx.data[4], ICS_CAN_Rx.data[5], ICS_CAN_Rx.data[6], ICS_CAN_Rx.data[7]);

	sprintf(canIdStr, "0x%x", ICS_CAN_Rx.id);
	sprintf(dlcStr, "%ld", ICS_CAN_Rx.dlc);

	lv_label_set_text(ui_canIdLabel, canIdStr);
	lv_label_set_text(ui_dlcLabel, dlcStr);
	lv_label_set_text(ui_dataLabel, dataStr);
}

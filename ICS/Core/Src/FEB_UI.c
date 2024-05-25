// **************************************** Includes ****************************************

#include "FEB_UI.h"

// **************************************** Variables ****************************************

extern ICS_UI_Values_t ICS_UI_Values;

const char* BMS_STATE_LABELS[] = {"PRECHARGE", "CHARGE", "BALANCE", "DRIVE", "SHUTDOWN", "NULL"};
const int BMS_STATE_COLORS[] = {0xFAFF00, 0x33FF00, 0x00F0FF, 0xFA00FF, 0xFF0000, 0xC2C2C2};

const char* HV_STATUS_LABELS[] = {"HV OFF", "HV ON"};
const int HV_STATUS_COLORS[] = {0xFF0000, 0xFF8A00};

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

	UI_Demo_Mode();
}

void UI_Demo_Mode(void) {
	ICS_UI_Values.bms_state++;
}

void FEB_UI_Set_Values(void) {
	lv_label_set_text(ui_Label10, BMS_STATE_LABELS[ICS_UI_Values.bms_state % 6]);
	lv_obj_set_style_text_color(ui_Label10, lv_color_hex(BMS_STATE_COLORS[ICS_UI_Values.bms_state % 6]), LV_PART_MAIN | LV_STATE_DEFAULT );

	lv_label_set_text(ui_Label6, HV_STATUS_LABELS[ICS_UI_Values.bms_state % 6 != 4 && ICS_UI_Values.bms_state % 6 != 5]);
	lv_obj_set_style_text_color(ui_Label6, lv_color_hex(HV_STATUS_COLORS[ICS_UI_Values.bms_state % 6 != 4 && ICS_UI_Values.bms_state % 6 != 5]), LV_PART_MAIN | LV_STATE_DEFAULT );
}

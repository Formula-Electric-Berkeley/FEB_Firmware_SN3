// **************************************** Includes ****************************************

#include "FEB_UI.h"

// **************************************** Variables ****************************************

Screen_Info_t screen_info;

extern ICS_UI_Values_t ICS_UI_Values;

const char* BMS_STATE_LABELS[] = {"PRECHARGE", "CHARGE", "BALANCE", "DRIVE", "SHUTDOWN", "NULL"};
const int BMS_STATE_COLORS[] = {0xFAFF00, 0x33FF00, 0x00F0FF, 0xFA00FF, 0xFF0000, 0xC2C2C2};

const char* HV_STATUS_LABELS[] = {"HV OFF", "HV ON"};
const int HV_STATUS_COLORS[] = {0xFF0000, 0xFF8A00};
uint8_t iter;

// **************************************** Functions ****************************************

void FEB_UI_Init(void) {
	screen_info.test_str = "0";
	iter = 1;
	lv_init();

	screen_driver_init();
//	touch_sensor_driver_init();

	ICS_UI_Values.bms_state = 5;

	ui_init();
}

void FEB_UI_Update(void) {
	lv_task_handler();

	FEB_UI_Set_Values();

//	ui_init();

//	UI_Demo_Mode();
	iter++;
}

void FEB_UI_Set_Values(void) {
	char test_feb_str[20];
	sprintf(test_feb_str, "%d", iter);
	screen_info.test_str = test_feb_str;

	set_soc_value(iter);
	set_temp_value(iter + 7);
	set_speed_value(iter + 13);

	lv_bar_set_value(ui_Bar1, iter, LV_ANIM_OFF);
	lv_bar_set_value(ui_Bar3, iter + 7, LV_ANIM_OFF);

//	lv_obj_clean(canvas1);
//	lv_canvas_fill_bg(canvas1, LV_COLOR_CHROMA_KEY, LV_OPA_TRANSP);
//	set_soc_value(iter);
}
//	lv_label_set_text(ui_Label10, BMS_STATE_LABELS[ICS_UI_Values.bms_state % 6]);
//	lv_obj_set_style_text_color(ui_Label10, lv_color_hex(BMS_STATE_COLORS[ICS_UI_Values.bms_state % 6]), LV_PART_MAIN | LV_STATE_DEFAULT );
//
//	lv_label_set_text(ui_Label6, HV_STATUS_LABELS[ICS_UI_Values.ivt_voltage > 60.0]);
//	lv_obj_set_style_text_color(ui_Label6, lv_color_hex(HV_STATUS_COLORS[ICS_UI_Values.ivt_voltage > 60.0]), LV_PART_MAIN | LV_STATE_DEFAULT );
//}

void UI_Demo_Mode(void) {
	ICS_UI_Values.bms_state++;
	ICS_UI_Values.ivt_voltage += 20.0;
	ICS_UI_Values.acc_temp += 5;

	lv_bar_set_value(ui_Bar1, abs((100 - (((int) ((ICS_UI_Values.ivt_voltage / 600.0) * 100)) % 600)) % 100), LV_ANIM_OFF);
    char soc_str[20];
    sprintf(soc_str, "%d%%", abs((100 - (((int) ((ICS_UI_Values.ivt_voltage / 600.0) * 100)) % 600)) % 100));
	lv_label_set_text(ui_Label2, soc_str);

	if ((int) ICS_UI_Values.acc_temp % 60 < 30) ICS_UI_Values.acc_temp = 30.0;

	lv_bar_set_value(ui_Bar3, (int) (((((int) ICS_UI_Values.acc_temp) % 65) - 25) / 35.0 * 100.0), LV_ANIM_OFF);
    char temp_str[20];
    sprintf(temp_str, "%d°C", ((int) ICS_UI_Values.acc_temp) % 65);
	lv_label_set_text(ui_Label5, temp_str);
}

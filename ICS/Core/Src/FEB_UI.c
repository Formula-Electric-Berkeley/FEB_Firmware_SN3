// **************************************** Includes ****************************************

#include "FEB_UI.h"

// **************************************** Variables ****************************************

extern ICS_UI_Values_t ICS_UI_Values;



const char* BMS_STATE_LABELS[] = {"STARTUP", "STANDBY", "BALANCE", "CHARGE", "PRECHARGE", "DR STDBY","DRIVE","REG READY", "FAULT"};
const int BMS_STATE_COLORS[] = {0xFAFF00, 0x33FF00, 0x00F0FF, 0xFA00FF, 0xFF0000, 0xC2C2C2,0xFA00FF,0xFA00FF,0xFA00FF};

const char* HV_STATUS_LABELS[] = {"HV OFF", "HV ON"};
const int HV_STATUS_COLORS[] = {0xFF0000, 0xFF8A00};

// **************************************** Functions ****************************************

void FEB_UI_Init(void) {
	lv_init();

	screen_driver_init();
	touch_sensor_driver_init();

	ICS_UI_Values.bms_state = 0;

	ui_init();
}

void FEB_UI_Update(void) {
	lv_task_handler();

	FEB_UI_Set_Values();

//	UI_Demo_Mode();
}

void FEB_UI_Set_Values(void) {
	lv_label_set_text(ui_Label10, BMS_STATE_LABELS[ICS_UI_Values.bms_state]);
	lv_obj_set_style_text_color(ui_Label10, lv_color_hex(BMS_STATE_COLORS[ICS_UI_Values.bms_state]), LV_PART_MAIN | LV_STATE_DEFAULT );

	lv_label_set_text(ui_Label6, HV_STATUS_LABELS[ICS_UI_Values.ivt_voltage > 60.0]);
	lv_obj_set_style_text_color(ui_Label6, lv_color_hex(HV_STATUS_COLORS[ICS_UI_Values.ivt_voltage > 60.0]), LV_PART_MAIN | LV_STATE_DEFAULT );
}

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

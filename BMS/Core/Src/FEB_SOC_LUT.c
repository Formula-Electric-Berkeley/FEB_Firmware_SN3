// **************** Includes ****************

#include "FEB_SOC_LUT.h"

// **************** Macro ****************

#define DIFF(x, y) ((x) > (y) ? (x) - (y) : (y) - (x))

// **************** LUT ****************

static float soc_volt_lut[21] = {2.499965, 2.985317, 3.154061, 3.288567, 3.389828, 3.463056, 3.503835, 3.567787, 3.61969, 3.667488, 3.717472, 3.763494, 3.802567, 3.836061, 3.874704, 3.933176, 3.995929, 4.035351, 4.053971, 4.077756, 4.2};

// **************** Functions ****************

int16_t FEB_SOC_LUT_Get_Closest_SOC(uint16_t voltage_mV) {
	int closest_ind = 0;
	float voltage_V = (float) voltage_mV / 100;
	float closest_voltage = soc_volt_lut[0];
	for (int lut_index = 0; lut_index < 21; lut_index++) {
		if (DIFF(soc_volt_lut[lut_index], voltage_V) < DIFF(closest_voltage, voltage_V)) {
			closest_voltage = soc_volt_lut[lut_index];
			closest_ind = lut_index;
		}
	}
	return closest_ind * 5;
}

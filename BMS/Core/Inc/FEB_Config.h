#ifndef INC_FEB_CONFIG_H_
#define INC_FEB_CONFIG_H_

#include "FEB_SM.h"
#include <stdint.h>

#define FEB_CONFIG_NUM_BANKS 7
#define FEB_CONFIG_NUM_CELLS_PER_BANK 20
#define FEB_CONFIG_NUM_ACC_CELLS_PER_IC 10
#define FEB_CONFIG_NUM_PINS_PER_IC 12
#define FEB_CONFIG_NUM_TEMP_MUX_CH 5

// NOTE: Do not use these values. Reference respective functions to get cell voltage boundaries.
#define FEB_CONFIG_CELL_MIN_VOLTAGE_mV 2500
#define FEB_CONFIG_CELL_MAX_VOLTAGE_mV 4200
#define FEB_CONFIG_CELL_MIN_VOLTAGE_100uV (FEB_CONFIG_CELL_MIN_VOLTAGE_mV * 10)
#define FEB_CONFIG_CELL_MAX_VOLTAGE_100uV (FEB_CONFIG_CELL_MAX_VOLTAGE_mV * 10)

void FEB_Config_Update(FEB_SM_ST_t current_state);
uint16_t FEB_Config_Get_Cell_Min_Voltage_mV(void);
uint16_t FEB_Config_Get_Cell_Max_Voltage_mV(void);
int16_t FEB_Config_Get_Cell_Min_Temperature_dC(void);
int16_t FEB_Config_Get_Cell_Max_Temperature_dC(void);
int32_t FEB_Config_Get_Pack_Min_Current_mA(void);
int32_t FEB_Config_Get_Pack_Max_Current_mA(void);

#endif /* INC_FEB_CONFIG_H_ */

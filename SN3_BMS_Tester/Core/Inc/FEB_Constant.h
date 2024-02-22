/*
 * FEB_Constant.h
 *
 *  Created on: Jan 16, 2024
 *      Author: andyyang
 */

#ifndef INC_FEB_CONSTANT_H_
#define INC_FEB_CONSTANT_H_

// **************************************** Definitions ****************************************
#define FEB_CONSTANT_NUM_ACC_CELLS_PER_IC 10
#define FEB_CONST_NUM_LTC_CELLS_PER_IC 12

#define FEB_CONSTANT_CELL_DEFAULT_VOLTAGE_V 3.75
#define FEB_CONSTANT_TEMP_DEFAULT_VOLTAGE_V 1.75

#define FEB_CONSTANT_CELL_MIN_VOLTAGE_V 2.2
#define FEB_CONSTANT_CELL_MAX_VOLTAGE_V 4.5
#define FEB_CONSTANT_CELL_MIN_TEMP_VOLTAGE_V 1
#define FEB_CONSTANT_CELL_MAX_TEMP_VOLTAGE_V 2.7

#define FEB_CONSTANT_CELL_VOLTAGE_THRESHOLD_V (60 * 1e-3) //TODO: TBD, update name
#define FEB_CONSTANT_CELL_TEMP_THRESHOLD_V (30 * 1e-3) //TODO: TBD

#define FEB_CONSTANT_AUX_TESTER_IC 0
#define FEB_CONSTANT_DAUGHTER_TESTER_IC 1

#define FEB_CONSTANT_DAUGHTER_MODE true
#define FEB_CONSTANT_AUX_MODE false

#define FEB_CONSTANT_START true
#define FEB_CONSTANT_NOT_START false

//all start with FEB_Constant

#endif /* INC_FEB_CONSTANT_H_ */

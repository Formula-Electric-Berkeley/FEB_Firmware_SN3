/*
 * FEB_Constant.h
 *
 *  Created on: Jan 16, 2024
 *      Author: andyyang
 */

#ifndef INC_FEB_CONSTANT_H_
#define INC_FEB_CONSTANT_H_

// **************************************** Definitions ****************************************
#define FEB_CONSTANT_NUM_CELLS 10

#define FEB_CONSTANT_CELL_DEFAULT_VOLTAGE 3.75
#define FEB_CONSTANT_TEMP_DEFAULT_VOLTAGE 1.75

#define FEB_CONSTANT_CELL_MIN_VOLTAGE 2.2
#define FEB_CONSTANT_CELL_MAX_VOLTAGE 4.5
#define FEB_CONSTANT_CELL_MIN_TEMP_VOLTAGE 1
#define FEB_CONSTANT_CELL_MAX_TEMP_VOLTAGE 2.7

#define FEB_CONSTANT_CELL_VOLTAGE_THRESHOLD_mV 60 //TODO: TBD
#define FEB_CONSTANT_CELL_TEMP_THRESHOLD_mV 30 //TODO: TBD

#define FEB_CONSTANT_AUX_TESTER_IC 0
#define FEB_CONSTANT_DAUGHTER_TESTER_IC 1

#define FEB_CONSTANT_DAUGHTER_MODE true
#define FEB_CONSTANT_AUX_MODE false

#define FEB_CONSTANT_START true
#define FEB_CONSTANT_NOT_START false

//all start with FEB_Constant

#endif /* INC_FEB_CONSTANT_H_ */

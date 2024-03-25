#ifndef INC_FEB_CONST_H_
#define INC_FEB_CONST_H_

//***************************************** INCLUDES *****************************8

#include <stdint.h>


// **************************************** Definitions ****************************************

#define SLEEP_TIME 10

#define ACC_PEDAL_1_START 860.0
#define ACC_PEDAL_1_END 1110.0

#define ACC_PEDAL_2_START 3245.0
#define ACC_PEDAL_2_END 2995.0

//#define BRAKE_PEDAL_1_START 1350.0
//#define BRAKE_PEDAL_1_END 910.0

#define BRAKE_PEDAL_1_START 385.0
#define BRAKE_PEDAL_1_END 565.0

#define BRAKE_PEDAL_2_START 1390.0
#define BRAKE_PEDAL_2_END 1160.0

#define PRESURE_START 345.0
#define PRESURE_END 380.0

#define INIT_VOLTAGE 242 // initial voltage of accumulator
#define PEAK_CURRENT 95  // max current (in amps) we want to pull
#define MAX_TORQUE 230
#define RPM_TO_RAD_S .10472

#define HECS_CURRENT 0.0 //check if current is too high for BSPD

#endif /* INC_FEB_CONST_H_ */

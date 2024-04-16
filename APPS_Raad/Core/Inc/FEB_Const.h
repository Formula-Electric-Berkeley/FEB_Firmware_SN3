#ifndef INC_FEB_CONST_H_
#define INC_FEB_CONST_H_

//***************************************** INCLUDES *****************************8

#include <stdint.h>


// **************************************** Definitions ****************************************

#define SLEEP_TIME 10

#define ACC_PEDAL_1_START 410.0
#define ACC_PEDAL_1_END 3360.0

#define ACC_PEDAL_2_START 310.0
#define ACC_PEDAL_2_END 3560.0

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

#define ACC_PEDAL_1 ADC_CHANNEL_1
#define ACC_PEDAL_2 ADC_CHANNEL_0
#define BRAKE_PRESS_1 ADC_CHANNEL_7
#define BRAKE_PRESS_2 ADC_CHANNEL_6
#define BRAKE_PEDAL ADC_CHANNEL_13
#define HECS_SIGNAL ADC_CHANNEL_11
#define MICRO_SHUTDOWN ADC_CHANNEL_10

#endif /* INC_FEB_CONST_H_ */

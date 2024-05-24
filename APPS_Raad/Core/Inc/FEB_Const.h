#ifndef INC_FEB_CONST_H_
#define INC_FEB_CONST_H_

//***************************************** INCLUDES *****************************8

#include <stdint.h>


// **************************************** Definitions ****************************************

#define SLEEP_TIME 10

#define ACC_PEDAL_1_START 1517.0
#define ACC_PEDAL_1_END 1352.0

#define ACC_PEDAL_2_START 820.0
#define ACC_PEDAL_2_END 1806.0

//#define BRAKE_PEDAL_1_START 1350.0
//#define BRAKE_PEDAL_1_END 910.0

#define BRAKE_PEDAL_1_START 1250.0
#define BRAKE_PEDAL_1_END 2300.0

#define BRAKE_PEDAL_2_START 1390.0
#define BRAKE_PEDAL_2_END 1160.0

#define PRESSURE_START 189.0
#define PRESSURE_END 1025.0

#define INIT_VOLTAGE 242 // initial voltage of accumulator
#define PEAK_CURRENT 95  // max current (in amps) we want to pull
#define MAX_TORQUE 230
#define RPM_TO_RAD_S .10472

#define HECS_CURRENT 3025.0 //check if current is too high for BSPD

#define ACC_PEDAL_1 ADC_CHANNEL_1
#define ACC_PEDAL_2 ADC_CHANNEL_0
#define BRAKE_PRESS_1 ADC_CHANNEL_7
#define BRAKE_PRESS_2 ADC_CHANNEL_6
#define BRAKE_PEDAL ADC_CHANNEL_13
#define HECS_SIGNAL ADC_CHANNEL_11
#define MICRO_SHUTDOWN ADC_CHANNEL_10

#endif /* INC_FEB_CONST_H_ */

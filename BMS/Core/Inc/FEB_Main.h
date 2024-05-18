#ifndef INC_FEB_MAIN_H_
#define INC_FEB_MAIN_H_

// ******************************** Includes ********************************

#include "FEB_CAN.h"
#include "FEB_CAN_Charger.h"
#include "FEB_CAN_IVT.h"
#include "FEB_LTC6811.h"
#include "FEB_SM.h"

#include "cmsis_os.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void);


// Tasks
void FEB_Main_Task1_VT(void);
void FEB_Main_Task2_State(void);		// TODO: remove
void FEB_Main_Task3_Charge(void);
void FEB_Main_Task4_Balance(void);
void FEB_Main_Task5_IVT(void);
void FEB_Main_Task6_CAN(void);

#endif /* INC_FEB_MAIN_H_ */

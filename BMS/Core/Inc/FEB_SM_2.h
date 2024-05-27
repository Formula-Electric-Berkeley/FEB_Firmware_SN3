//#ifndef INC_FEB_SM_2_H_
//#define INC_FEB_SM_2_H_
//
//// ******************************** Includes ********************************
//
//#include "FEB_CAN_ID.h"
//#include "FEB_LTC6811.h"
//#include "FEB_Relay.h"
//
//#include <stdint.h>
//#include "stm32f4xx_hal.h"
//
//// ******************************** Enum & Struct ********************************
//
//// List of states for state machine
//typedef enum {
//	FEB_SM_ST_BALANCE,
//	FEB_SM_ST_CHARGE,
//	FEB_SM_ST_DRIVE,
//	FEB_SM_ST_PRECHARGE,
//	FEB_SM_ST_SHUTDOWN
//} FEB_SM_ST_t;
//
//// Transitions between states
//typedef struct {
//	FEB_SM_ST_t curr_state;
//	void (*action) (void);
//	FEB_SM_ST_t next_state;
//} FEB_SM_Transition_t;
//
//// ******************************** Functions ********************************
//
//void FEB_SM_Init(void);
//FEB_SM_ST_t FEB_SM_Get_Current_State(void);
//uint8_t FEB_SM_Set_Current_State(FEB_SM_ST_t next_state);
//void FEB_SM_CAN_Transmit(void);
//
//#endif /* INC_FEB_SM_2_H_ */

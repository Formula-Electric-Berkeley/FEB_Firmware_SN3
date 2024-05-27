#ifndef INC_FEB_SM_H_
#define INC_FEB_SM_H_

// States
typedef enum {
	FEB_SM_ST_STARTUP,
	FEB_SM_ST_STANDBY,
	FEB_SM_ST_BALANCE,
	FEB_SM_ST_CHARGE,
	FEB_SM_ST_PRECHARGE,
	FEB_SM_ST_DRIVE_STANDBY,
	FEB_SM_ST_DRIVE,
	FEB_SM_ST_DRIVE_REGEN,
	FEB_SM_ST_FAULT
} FEB_SM_ST_t;

void FEB_SM_Init(void);
FEB_SM_ST_t FEB_SM_Get_Current_State(void);
void FEB_SM_Transition(FEB_SM_ST_t next_state);
void FEB_SM_Process(void);

/* Faults
CAN Initialization fails
Under/Over cell voltage
Under/Over cell temperature
*/

#endif /* INC_FEB_SM_H_ */

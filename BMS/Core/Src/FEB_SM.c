#include "FEB_SM.h"
#include "FEB_Config.h"
#include "FEB_Hw.h"
#include "FEB_CAN.h"
#include "FEB_LTC6811.h"
#include "FEB_CAN_Charger.h"
#include "FEB_CAN_ICS.h"
#include "FEB_CAN_Library/FEB_CAN_ID.h"
#include "cmsis_os2.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

static void bootTransition(FEB_SM_ST_t);
static void LVPowerTransition(FEB_SM_ST_t);
static void ESCCompleteTransition(FEB_SM_ST_t);
static void PrechargeTransition(FEB_SM_ST_t);
static void EnergizedTransition(FEB_SM_ST_t);
static void DriveTransition(FEB_SM_ST_t);
static void FreeTransition(FEB_SM_ST_t);
static void ChargingTransition(FEB_SM_ST_t);
static void BalanceTransition(FEB_SM_ST_t);
static void BMSFaultTransition(FEB_SM_ST_t);
static void BSPDFaultTransition(FEB_SM_ST_t);
static void IMDFaultTransition(FEB_SM_ST_t);
static void ChargingFaultTransition(FEB_SM_ST_t);

extern osMutexId_t FEB_SM_LockHandle;
extern osMutexId_t FEB_UART_LockHandle;
extern UART_HandleTypeDef huart2;
extern CAN_HandleTypeDef hcan1;
extern uint8_t FEB_CAN_Tx_Data[8];
extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
extern uint32_t FEB_CAN_Tx_Mailbox;

// Shared variable, requires synchronization
static FEB_SM_ST_t current_state;

/* ******** State Transitions ******** */

/* List of transition functions*/
static void (*transitionVector[13])(FEB_SM_ST_t)={
		bootTransition,
		LVPowerTransition,
		ESCCompleteTransition,
		PrechargeTransition,
		EnergizedTransition,
		DriveTransition,
		FreeTransition,
		ChargingTransition,
		BalanceTransition,
		BMSFaultTransition,
		BSPDFaultTransition,
		IMDFaultTransition,
		ChargingFaultTransition
};

/* ******** State Transition Functions ******** */
/* State transition functions assume serial access (SM lock held). */

/* Initiate state transition. Assume SM lock held. */
static void transition(FEB_SM_ST_t next_state) {
	(transitionVector[FEB_SM_Get_Current_State()])(next_state);
}

/* Function called from initial thread (no other threads created).
 * No synchronization needed. */
static void startup(void) {
	current_state = FEB_SM_ST_BOOT;
	FEB_Config_Update(current_state);

	FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Set_BMS_Shutdown_Relay(FEB_HW_RELAY_CLOSE);
	FEB_CAN_Charger_Init();


	if (!FEB_CAN_Init()) {
		current_state = FEB_SM_ST_FAULT_BMS;
		FEB_Hw_Set_BMS_Shutdown_Relay(FEB_HW_RELAY_OPEN);
		return;
	}

	FEB_LTC6811_Init();

}

/* ******** Interface ******** */

/* Function called from initial thread (no other threads created).
 * No synchronization needed. */
void FEB_SM_Init(void) {
	startup();
}

/* Get current state of state machine. */
FEB_SM_ST_t FEB_SM_Get_Current_State(void) {
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	FEB_SM_ST_t state = current_state;
	osMutexRelease(FEB_SM_LockHandle);
	return state;
}

/* Initiate state transition. */
void FEB_SM_Transition(FEB_SM_ST_t next_state) {
	transition(next_state);
}

/* Check for conditions necessary for state transitions. */
void FEB_SM_Process(void) {
	transitionVector[FEB_SM_Get_Current_State()](FEB_SM_ST_DEFAULT);
}

//FAULT HELPER FUNCTION
static void fault(FEB_SM_ST_t FAULT_TYPE) {
	current_state = FAULT_TYPE;
	FEB_Config_Update(current_state);

	FEB_Hw_Set_BMS_Shutdown_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);

}

/** Transition Functions **/
static FEB_SM_ST_t updateStateProtected(FEB_SM_ST_t next_state){
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	if(current_state==FEB_SM_ST_FAULT_BMS)
		return FEB_SM_ST_FAULT_BMS;
	current_state=next_state;
	osMutexRelease(FEB_SM_LockHandle);
	return next_state;
}

static void bootTransition(FEB_SM_ST_t next_state){
	switch(next_state){
	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
	case FEB_SM_ST_FAULT_BSPD:
		fault(next_state);
		break;

	case FEB_SM_ST_LV:
		FEB_Config_Update(updateStateProtected(FEB_SM_ST_LV));
		break;

	default:
		return;
	}

}

static void LVPowerTransition(FEB_SM_ST_t next_state){
	switch(next_state){
	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
	case FEB_SM_ST_FAULT_BSPD:
		fault(next_state);
		break;

	case FEB_SM_ST_ESC:
	case FEB_SM_ST_FREE:
		FEB_Config_Update(updateStateProtected(next_state));
		break;

	case FEB_SM_ST_DEFAULT:
		if(FEB_Hw_BMS_Shutdown_Sense()==FEB_HW_RELAY_CLOSE)
			LVPowerTransition(FEB_SM_ST_ESC);
		if (FEB_CAN_Charger_Received())
			LVPowerTransition(FEB_SM_ST_FREE);
		break;

	default:
		return;
	}
}

static void ESCCompleteTransition(FEB_SM_ST_t next_state){
	switch(next_state){
	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
	case FEB_SM_ST_FAULT_BSPD:
		fault(next_state);
		break;

	case FEB_SM_ST_LV:
		FEB_Config_Update(updateStateProtected(next_state));
		break;

	case FEB_SM_ST_PRECHARGE:
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
		FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_CLOSE);
		updateStateProtected(FEB_SM_ST_PRECHARGE);
		FEB_Config_Update(current_state);
		break;

	case FEB_SM_ST_DEFAULT:
		if (FEB_Hw_AIR_Minus_Sense() == FEB_HW_RELAY_CLOSE && FEB_Hw_AIR_Plus_Sense() == FEB_HW_RELAY_OPEN && FEB_Hw_Charge_Sense() == FEB_HW_RELAY_OPEN)
			ESCCompleteTransition(FEB_SM_ST_PRECHARGE);
		else if(FEB_Hw_BMS_Shutdown_Sense()==FEB_HW_RELAY_OPEN)
			ESCCompleteTransition(FEB_SM_ST_LV);
		break;

	default:
		return;
	}


}

static void PrechargeTransition(FEB_SM_ST_t next_state){
	switch(next_state){
	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
	case FEB_SM_ST_FAULT_BSPD:
		fault(next_state);
		break;

	case FEB_SM_ST_LV:
	case FEB_SM_ST_ESC:
		FEB_Config_Update(updateStateProtected(next_state));
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
		FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);
		break;

	case FEB_SM_ST_ENERGIZED:
		FEB_Config_Update(updateStateProtected(FEB_SM_ST_ENERGIZED));
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_CLOSE);
		osDelay(100);
		FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);

		break;

	case FEB_SM_ST_DEFAULT:
		if(FEB_Hw_BMS_Shutdown_Sense()==FEB_HW_RELAY_OPEN||FEB_Hw_AIR_Minus_Sense()==FEB_HW_RELAY_OPEN)
			PrechargeTransition(FEB_SM_ST_LV);
		else if(FEB_Hw_Precharge_Sense()==FEB_HW_RELAY_OPEN)
			PrechargeTransition(FEB_SM_ST_ESC);
		break;

	default:
		return;
	}


}

static void EnergizedTransition(FEB_SM_ST_t next_state){
	switch(next_state){
	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
	case FEB_SM_ST_FAULT_BSPD:
		fault(next_state);
		break;

	case FEB_SM_ST_DRIVE:
		FEB_Config_Update(updateStateProtected(next_state));
		break;

	case FEB_SM_ST_LV:
	case FEB_SM_ST_ESC:
		FEB_Config_Update(updateStateProtected(next_state));
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
		FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);
		break;

	case FEB_SM_ST_DEFAULT:
		if(FEB_Hw_BMS_Shutdown_Sense()==FEB_HW_RELAY_OPEN||FEB_Hw_AIR_Minus_Sense()==FEB_HW_RELAY_OPEN)
			EnergizedTransition(FEB_SM_ST_LV);
		else if(FEB_CAN_ICS_Ready_To_Drive())
			EnergizedTransition(FEB_SM_ST_DRIVE);
		break;

	default:
		return;
	}

}

static void DriveTransition(FEB_SM_ST_t next_state){
	switch(next_state){
	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
	case FEB_SM_ST_FAULT_BSPD:
		fault(next_state);
		break;

	case FEB_SM_ST_LV:
	case FEB_SM_ST_ESC:
		FEB_Config_Update(updateStateProtected(next_state));
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
		FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);
		break;

	case FEB_SM_ST_ENERGIZED:
		FEB_Config_Update(updateStateProtected(FEB_SM_ST_ENERGIZED));
		break;

	case FEB_SM_ST_DEFAULT:
		if(FEB_Hw_BMS_Shutdown_Sense()==FEB_HW_RELAY_OPEN||FEB_Hw_AIR_Minus_Sense()==FEB_HW_RELAY_OPEN)
			DriveTransition(FEB_SM_ST_LV);
		else if(!FEB_CAN_ICS_Ready_To_Drive())
			DriveTransition(FEB_SM_ST_ENERGIZED);
		break;

	default:
		return;
	}
}

static void FreeTransition(FEB_SM_ST_t next_state){
	switch(next_state){

	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
		fault(FEB_SM_ST_FAULT_CHARGING);
		break;

	case FEB_SM_ST_FREE:
	case FEB_SM_ST_LV:
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
		FEB_Config_Update(updateStateProtected(FEB_SM_ST_FREE));
		break;

	case FEB_SM_ST_CHARGING:
		FEB_Config_Update(updateStateProtected(next_state));
		osDelay(100);
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_CLOSE);
		break;
	case FEB_SM_ST_BALANCE:
		FEB_Config_Update(updateStateProtected(next_state));
		break;
	case FEB_SM_ST_DEFAULT:
		if(FEB_Hw_AIR_Minus_Sense()==FEB_HW_RELAY_CLOSE && FEB_Hw_Charge_Sense()==FEB_HW_RELAY_CLOSE && FEB_CAN_Charger_Received())
			FreeTransition(FEB_SM_ST_CHARGING);
	default:
		return;
	}

}

static void ChargingTransition(FEB_SM_ST_t next_state){
	switch(next_state){

	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
		fault(FEB_SM_ST_FAULT_CHARGING);
		break;

	case FEB_SM_ST_LV:
	case FEB_SM_ST_FREE:
		FEB_Config_Update(updateStateProtected(FEB_SM_ST_FREE));

		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
		FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);
		break;

	case FEB_SM_ST_DEFAULT:
		if(FEB_Hw_AIR_Minus_Sense()==FEB_HW_RELAY_OPEN)
			ChargingTransition(FEB_SM_ST_FREE);
	default:
		break;
	}

}

static void BalanceTransition(FEB_SM_ST_t next_state){
	switch(next_state){

	case FEB_SM_ST_FAULT_BMS:
	case FEB_SM_ST_FAULT_IMD:
		fault(FEB_SM_ST_FAULT_CHARGING);
		osMutexRelease(FEB_SM_LockHandle);
		FEB_LTC6811_Stop_Balance();
		while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
		break;

	case FEB_SM_ST_LV:
	case FEB_SM_ST_FREE:
		FEB_Hw_Set_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
		FEB_Hw_Set_Precharge_Relay(FEB_HW_RELAY_OPEN);
		osMutexRelease(FEB_SM_LockHandle);
		FEB_LTC6811_Stop_Balance();
		FEB_Config_Update(updateStateProtected(FEB_SM_ST_FREE));
		break;

	case FEB_SM_ST_DEFAULT:
		if(FEB_Hw_AIR_Minus_Sense()==FEB_HW_RELAY_OPEN)
			BalanceTransition(FEB_SM_ST_FREE);

	default:
		break;

	}
}

/** Hard Fault Functions **/
static void BMSFaultTransition(FEB_SM_ST_t next_state){
	fault(current_state);
}
static void BSPDFaultTransition(FEB_SM_ST_t next_state){
	fault(current_state);
}
static void IMDFaultTransition(FEB_SM_ST_t next_state){
	fault(current_state);
}
static void ChargingFaultTransition(FEB_SM_ST_t next_state){
	fault(current_state);
}

/* Output current state, relay state, and GPIO sense. */
void FEB_SM_UART_Transmit(void) {
	if (!FEB_SM_ST_DEBUG)
		return;

	FEB_SM_ST_t state = FEB_SM_Get_Current_State();
	char* state_str;
	switch(state) {
		case FEB_SM_ST_BOOT:
			state_str = "Boot";
			break;
		case FEB_SM_ST_LV:
			state_str = "LV Power";
			break;
		case FEB_SM_ST_ESC:
			state_str = "ESC Complete";
			break;
		case FEB_SM_ST_PRECHARGE:
			state_str = "PRECHARGE";
			break;
		case FEB_SM_ST_ENERGIZED:
			state_str = "Energized";
			break;
		case FEB_SM_ST_DRIVE:
			state_str = "Drive";
			break;
		case FEB_SM_ST_FREE:
			state_str = "Accumulator Free";
			break;
		case FEB_SM_ST_CHARGING:
			state_str = "Charging";
			break;
		case FEB_SM_ST_BALANCE:
			state_str = "Balancing";
			break;
		case FEB_SM_ST_FAULT_BMS:
			state_str = "BMS Fault";
			break;
		case FEB_SM_ST_FAULT_IMD:
			state_str = "IMD Fault";
			break;
		case FEB_SM_ST_FAULT_BSPD:
			state_str = "BSPD Fault";
			break;
		default:
			state_str= "undef";
			break;
	}

	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	FEB_Hw_Relay_t bms_shutdown_relay = FEB_Hw_Get_BMS_Shutdown_Relay();
	FEB_Hw_Relay_t air_plus_relay = FEB_Hw_Get_AIR_Plus_Relay();
	FEB_Hw_Relay_t precharge_relay = FEB_Hw_Get_Precharge_Relay();
	FEB_Hw_Relay_t air_minus_sense = FEB_Hw_AIR_Minus_Sense();
	FEB_Hw_Relay_t air_plus_sense = FEB_Hw_AIR_Plus_Sense();
	FEB_Hw_Relay_t bms_shutdown_sense = FEB_Hw_BMS_Shutdown_Sense();
	FEB_Hw_Relay_t imd_shutdown_sense = FEB_Hw_IMD_Shutdown_Sense();
	bool r2d = FEB_CAN_ICS_Ready_To_Drive();
	uint8_t ics = FEB_CAN_ICS_Data();
	osMutexRelease(FEB_SM_LockHandle);

	static char str[128];
	sprintf(str, "state %s %d %d %d %d %d %d %d\n r2d_state: %d, ics_data:%d\n", state_str,
			bms_shutdown_relay, air_plus_relay, precharge_relay,
			air_minus_sense, air_plus_sense,
			bms_shutdown_sense, imd_shutdown_sense, r2d, ics);

	while (osMutexAcquire(FEB_UART_LockHandle, UINT32_MAX) != osOK);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
	osMutexRelease(FEB_UART_LockHandle);
}

void FEB_SM_CAN_Transmit(void) {
	// Initialize transmission header
	FEB_CAN_Tx_Header.DLC = 3;
	FEB_CAN_Tx_Header.StdId = FEB_CAN_ID_BMS_STATE;
	FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

	// Get data
	uint16_t cell_min_voltage = FEB_LTC6811_Get_Min_Voltage();

	// Copy data to Tx buffer
	FEB_CAN_Tx_Data[0] = FEB_SM_Get_Current_State();
	FEB_CAN_Tx_Data[1] = cell_min_voltage && 0xFF;
	FEB_CAN_Tx_Data[2] = cell_min_voltage >> 8;

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		// FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}

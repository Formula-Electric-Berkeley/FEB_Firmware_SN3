#include "FEB_SM.h"
#include "FEB_Config.h"
#include "FEB_Hw.h"
#include "FEB_CAN.h"
#include "cmsis_os2.h"
#include <stdint.h>

// Lock used to synchronize state machine
extern osMutexId_t FEB_SM_LockHandle;

// Shared variable, requires synchronization
static FEB_SM_ST_t current_state;

static void startup(void);

static void idle(void);
static void idle_to_standby(void);

static void standby_to_balance(void);
static void standby_to_charge(void);
static void standby_to_precharge(void);

static void balance_to_fault(void);
static void balance_to_idle(void);
static void balance_to_standby(void);

static void charge_to_fault(void);
static void charge_to_idle(void);
static void charge_to_standby(void);

static void precharge_to_standby(void);
static void precharge_to_drive(void);

static void drive_to_drive_standby(void);
static void drive_to_drive_regen(void);

static void drive_standby_to_drive(void);

static void drive_regen_to_drive(void);

static void fault(void);

/* ******** State Transitions ******** */

// State transition model
typedef struct {
	FEB_SM_ST_t current_state;
	void (*action)(void);
	FEB_SM_ST_t next_state;
} FEB_SM_Transition_t;

/* List of transitions pairs (s, a, s') */
#define NUM_TRANSITIONS 29
static FEB_SM_Transition_t transitions[] = {
	{FEB_SM_ST_STARTUP, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_STARTUP, idle, FEB_SM_ST_IDLE},

	{FEB_SM_ST_IDLE, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_IDLE, idle_to_standby, FEB_SM_ST_STANDBY},

	{FEB_SM_ST_STANDBY, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_STANDBY, idle, FEB_SM_ST_IDLE},
	{FEB_SM_ST_STANDBY, standby_to_balance, FEB_SM_ST_BALANCE},
	{FEB_SM_ST_STANDBY, standby_to_charge, FEB_SM_ST_CHARGE},
	{FEB_SM_ST_STANDBY, standby_to_precharge, FEB_SM_ST_PRECHARGE},

	{FEB_SM_ST_BALANCE, balance_to_fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_BALANCE, balance_to_idle, FEB_SM_ST_IDLE},
	{FEB_SM_ST_BALANCE, balance_to_standby, FEB_SM_ST_STANDBY},

	{FEB_SM_ST_CHARGE, charge_to_fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_CHARGE, charge_to_idle, FEB_SM_ST_IDLE},
	{FEB_SM_ST_CHARGE, charge_to_standby, FEB_SM_ST_STANDBY},

	{FEB_SM_ST_PRECHARGE, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_PRECHARGE, idle, FEB_SM_ST_IDLE},
	{FEB_SM_ST_PRECHARGE, precharge_to_standby, FEB_SM_ST_STANDBY},
	{FEB_SM_ST_PRECHARGE, precharge_to_drive, FEB_SM_ST_DRIVE},

	{FEB_SM_ST_DRIVE, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_DRIVE, idle, FEB_SM_ST_IDLE},
	{FEB_SM_ST_DRIVE, drive_to_drive_standby, FEB_SM_ST_DRIVE_STANDBY},
	{FEB_SM_ST_DRIVE, drive_to_drive_regen, FEB_SM_ST_DRIVE_REGEN},

	{FEB_SM_ST_DRIVE_STANDBY, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_DRIVE_STANDBY, idle, FEB_SM_ST_IDLE},
	{FEB_SM_ST_DRIVE_STANDBY, drive_standby_to_drive, FEB_SM_ST_DRIVE},

	{FEB_SM_ST_DRIVE_REGEN, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_DRIVE_REGEN, idle, FEB_SM_ST_IDLE},
	{FEB_SM_ST_DRIVE_REGEN, drive_regen_to_drive, FEB_SM_ST_DRIVE}
};

/* ******** State Transition Functions ******** */
/* State transition functions assume serial access (SM lock held). */

/* Initiate state transition. Assume lock held. */
static void transition(FEB_SM_ST_t next_state) {
	if (current_state == next_state)
		return;
	for (uint8_t i = 0; i < NUM_TRANSITIONS; i++) {
		if (transitions[i].current_state == current_state &&
			transitions[i].next_state == next_state) {
			transitions[i].action();
			break;
		}
	}
}

/* Function called from initial thread (no other threads created).
 * No synchronization needed. */
static void startup(void) {
	current_state = FEB_SM_ST_STARTUP;
	FEB_Config_Update(current_state);

	FEB_Hw_Shutdown_Relay(FEB_HW_RELAY_CLOSE);
	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);

	if (!FEB_CAN_Init()) {
		transition(FEB_SM_ST_FAULT);
		return;
	}
}

static void idle(void) {
	current_state = FEB_SM_ST_IDLE;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);
}

static void idle_to_standby(void) {
	current_state = FEB_SM_ST_STANDBY;
	FEB_Config_Update(current_state);
}

static void standby_to_balance(void);
static void standby_to_charge(void);

static void standby_to_precharge(void) {
	current_state = FEB_SM_ST_PRECHARGE;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_CLOSE);
}

static void balance_to_fault(void);
static void balance_to_idle(void);
static void balance_to_standby(void);

static void charge_to_fault(void);
static void charge_to_idle(void);
static void charge_to_standby(void);

static void precharge_to_standby(void) {
	current_state = FEB_SM_ST_STANDBY;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);
}

static void precharge_to_drive(void) {
	current_state = FEB_SM_ST_DRIVE;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_CLOSE);
	osDelay(100);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);
}

static void drive_to_drive_standby(void) {
	current_state = FEB_SM_ST_DRIVE_STANDBY;
	FEB_Config_Update(current_state);
}


static void drive_to_drive_regen(void) {
	current_state = FEB_SM_ST_DRIVE_REGEN;
	FEB_Config_Update(current_state);
}

static void drive_standby_to_drive(void) {
	current_state = FEB_SM_ST_DRIVE;
	FEB_Config_Update(current_state);
}

static void drive_regen_to_drive(void) {
	current_state = FEB_SM_ST_DRIVE;
	FEB_Config_Update(current_state);
}

/* Handle transition from any state to fault state.
 * Function may be called to transition or during a different transition. */
static void fault(void) {
	FEB_SM_ST_t previous_state = current_state;
	current_state = FEB_SM_ST_FAULT;
	FEB_Config_Update(current_state);

	FEB_Hw_Shutdown_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);
}

/* ******** Interface ******** */

/* Function called by initial thread. No synchronization required. */
void FEB_SM_Init(void) {
	startup();
}

/* Get current state of state machine. Synchronization required. */
void FEB_SM_Get_Current_State(void) {
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	FEB_SM_ST_t state = current_state;
	osMutexRelease(FEB_SM_LockHandle);
	return state;
}

/* Initiate state transition. Synchronization required. */
void FEB_SM_Transition(FEB_SM_ST_t next_state) {
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	transition(next_state);
	osMutexRelease(FEB_SM_LockHandle);
}

/* Check for conditions necessary for state transitions. Synchronization required. */
void FEB_SM_Process(void) {
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	switch (current_state) {
		case FEB_SM_ST_STARTUP:
			break;
		case FEB_SM_ST_IDLE:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_CLOSE)
				transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_STANDBY:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_IDLE);
			break;
		case FEB_SM_ST_BALANCE:
			break;
		case FEB_SM_ST_CHARGE:
			break;
		case FEB_SM_ST_PRECHARGE:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_IDLE);
			break;
		case FEB_SM_ST_DRIVE:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_IDLE);
			break;
		case FEB_SM_ST_DRIVE_STANDBY:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_IDLE);
			break;
		case FEB_SM_ST_DRIVE_REGEN:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_IDLE);
			break;
		case FEB_SM_ST_FAULT:
			break;
	}
	osMutexRelease(FEB_SM_LockHandle);
}


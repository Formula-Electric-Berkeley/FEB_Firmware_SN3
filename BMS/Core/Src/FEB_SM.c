#include "FEB_SM.h"
#include "FEB_Config.h"
#include "FEB_Hw.h"
#include "FEB_CAN.h"
#include "FEB_LTC6811.h"
#include "FEB_CAN_Charger.h"
#include "cmsis_os2.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

extern osMutexId_t FEB_SM_LockHandle;	// Lock used to synchronize state machine
extern UART_HandleTypeDef huart2;

// Shared variable, requires synchronization
static FEB_SM_ST_t current_state;

static void startup(void);

static void standby(void);
static void standby_to_balance(void);
static void standby_to_charge(void);
static void standby_to_precharge(void);

static void precharge_to_drive_standby(void);

static void drive_standby_to_drive(void);

static void drive_to_drive_standby(void);
static void drive_to_drive_regen(void);

static void drive_regen_to_drive(void);
static void drive_regen_to_drive_standby(void);

static void fault(void);

/* ******** State Transitions ******** */

// State transition model
typedef struct {
	FEB_SM_ST_t current_state;
	void (*action)(void);
	FEB_SM_ST_t next_state;
} FEB_SM_Transition_t;

/* List of transitions pairs (s, a, s') */
#define NUM_TRANSITIONS 24
static FEB_SM_Transition_t transitions[] = {
	{FEB_SM_ST_STARTUP, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_STARTUP, standby, FEB_SM_ST_STANDBY},

	{FEB_SM_ST_STANDBY, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_STANDBY, standby_to_balance, FEB_SM_ST_BALANCE},
	{FEB_SM_ST_STANDBY, standby_to_charge, FEB_SM_ST_CHARGE},
	{FEB_SM_ST_STANDBY, standby_to_precharge, FEB_SM_ST_PRECHARGE},

	{FEB_SM_ST_BALANCE, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_BALANCE, standby, FEB_SM_ST_STANDBY},

	{FEB_SM_ST_CHARGE, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_CHARGE, standby, FEB_SM_ST_STANDBY},

	{FEB_SM_ST_PRECHARGE, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_PRECHARGE, standby, FEB_SM_ST_STANDBY},
	{FEB_SM_ST_PRECHARGE, precharge_to_drive_standby, FEB_SM_ST_DRIVE_STANDBY},

	{FEB_SM_ST_DRIVE_STANDBY, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_DRIVE_STANDBY, standby, FEB_SM_ST_STANDBY},
	{FEB_SM_ST_DRIVE_STANDBY, drive_standby_to_drive, FEB_SM_ST_DRIVE},

	{FEB_SM_ST_DRIVE, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_DRIVE, standby, FEB_SM_ST_STANDBY},
	{FEB_SM_ST_DRIVE, drive_to_drive_standby, FEB_SM_ST_DRIVE_STANDBY},
	{FEB_SM_ST_DRIVE, drive_to_drive_regen, FEB_SM_ST_DRIVE_REGEN},

	{FEB_SM_ST_DRIVE_REGEN, fault, FEB_SM_ST_FAULT},
	{FEB_SM_ST_DRIVE_REGEN, standby, FEB_SM_ST_STANDBY},
	{FEB_SM_ST_DRIVE_REGEN, drive_regen_to_drive, FEB_SM_ST_DRIVE},
	{FEB_SM_ST_DRIVE_REGEN, drive_regen_to_drive_standby, FEB_SM_ST_DRIVE_STANDBY}
};

/* ******** State Transition Functions ******** */
/* State transition functions assume serial access (SM lock held). */

/* Initiate state transition. Assume SM lock held. */
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
		current_state = FEB_SM_ST_FAULT;
		FEB_Hw_Shutdown_Relay(FEB_HW_RELAY_OPEN);
		return;
	}
	FEB_LTC6811_Init();
	FEB_CAN_Charger_Init();
}

/* Assume SM lock held. */
static void standby_to_balance(void) {
	current_state = FEB_SM_ST_BALANCE;
	FEB_Config_Update(current_state);

	// Initialize balance
	osMutexRelease(FEB_SM_LockHandle);
	FEB_LTC6811_Init_Balance();

	// Initialize state variables
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	current_state = FEB_SM_ST_BALANCE;
	FEB_Config_Update(current_state);
}

/* Assume SM lock held. */
static void standby(void) {
	FEB_SM_ST_t previous_state = current_state;
	current_state = FEB_SM_ST_STANDBY;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);

	// Stop Balance
	if (previous_state == FEB_SM_ST_BALANCE) {
		osMutexRelease(FEB_SM_LockHandle);
		FEB_LTC6811_Stop_Balance();
		while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	}

	// TODO: Stop Charge
}

/* Assume SM lock held. */
static void standby_to_charge(void) {
	current_state = FEB_SM_ST_CHARGE;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_CLOSE);
}

/* Assume SM lock held. */
static void standby_to_precharge(void) {
	current_state = FEB_SM_ST_PRECHARGE;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_CLOSE);
}

/* Assume SM lock held. */
static void precharge_to_drive_standby(void) {
	current_state = FEB_SM_ST_DRIVE_STANDBY;
	FEB_Config_Update(current_state);

	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_CLOSE);
	osDelay(100);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);
}

/* Assume SM lock held. */
static void drive_standby_to_drive(void) {
	current_state = FEB_SM_ST_DRIVE;
	FEB_Config_Update(current_state);
}

/* Assume SM lock held. */
static void drive_to_drive_standby(void) {
	current_state = FEB_SM_ST_DRIVE_STANDBY;
	FEB_Config_Update(current_state);
}

/* Assume SM lock held. */
static void drive_to_drive_regen(void) {
	current_state = FEB_SM_ST_DRIVE_REGEN;
	FEB_Config_Update(current_state);
}

/* Assume SM lock held. */
static void drive_regen_to_drive(void) {
	current_state = FEB_SM_ST_DRIVE;
	FEB_Config_Update(current_state);
}

/* Assume SM lock held. */
static void drive_regen_to_drive_standby(void) {
	current_state = FEB_SM_ST_DRIVE_STANDBY;
	FEB_Config_Update(current_state);
}

/* Handle transition from any state to fault state. Assume SM lock held. */
static void fault(void) {
	FEB_SM_ST_t previous_state = current_state;
	current_state = FEB_SM_ST_FAULT;
	FEB_Config_Update(current_state);

	FEB_Hw_Shutdown_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_AIR_Plus_Relay(FEB_HW_RELAY_OPEN);
	FEB_Hw_Precharge_Relay(FEB_HW_RELAY_OPEN);

	if (previous_state == FEB_SM_ST_BALANCE) {
		osMutexRelease(FEB_SM_LockHandle);
		FEB_LTC6811_Stop_Balance();
		while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	}

	// TODO: Stop charge
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
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	transition(next_state);
	osMutexRelease(FEB_SM_LockHandle);
}

/* Check for conditions necessary for state transitions. */
void FEB_SM_Process(void) {
	while (osMutexAcquire(FEB_SM_LockHandle, UINT32_MAX) != osOK);
	switch (current_state) {
		case FEB_SM_ST_STARTUP:
			transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_STANDBY:
			// TODO: transition to charge, balance, precharge
			transition(FEB_SM_ST_CHARGE);
			break;
		case FEB_SM_ST_BALANCE:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_CHARGE:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_PRECHARGE:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_DRIVE_STANDBY:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_DRIVE:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_DRIVE_REGEN:
			if (FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_OPEN)
				transition(FEB_SM_ST_STANDBY);
			break;
		case FEB_SM_ST_FAULT:
			break;
	}
	osMutexRelease(FEB_SM_LockHandle);

	static char str[64];
	sprintf(str, "state %d, SHD %d, - %d, + %d, cs %d, imd %d\n",
			current_state,
			FEB_Hw_Read_Shutdown_Circuit() == FEB_HW_RELAY_CLOSE,
			HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5),
			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10));
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
}

void FEB_SM_UART_Transmit(void) {
	FEB_SM_ST_t state = FEB_SM_Get_Current_State();
	char* state_str;
	switch(state) {
		case FEB_SM_ST_STARTUP:
			state_str = "startup";
			break;
		case FEB_SM_ST_STANDBY:
			state_str = "standby";
			break;
		case FEB_SM_ST_BALANCE:
			state_str = "balance";
			break;
		case FEB_SM_ST_CHARGE:
			state_str = "charge";
			break;
		case FEB_SM_ST_PRECHARGE:
			state_str = "precharge";
			break;
		case FEB_SM_ST_DRIVE_STANDBY:
			state_str = "drive-standby";
			break;
		case FEB_SM_ST_DRIVE:
			state_str = "drive";
			break;
		case FEB_SM_ST_DRIVE_REGEN:
			state_str = "drive-regen";
			break;
		case FEB_SM_ST_FAULT:
			state_str = "fault";
			break;
	}

	static char str[32];
	sprintf(str, "state %s\n", state_str);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), 100);
}

// ******************************** Includes & External ********************************

#include "FEB_Relay.h"

extern bool FEB_State_Shutdown;

// ******************************** Static Function Declarations ********************************

static void FEB_Relay_Shutdown(uint8_t state);
static void FEB_Relay_Precharge(uint8_t state);
static void FEB_Relay_Air_Plus(uint8_t state);

// ******************************** Core Functions ********************************

static void FEB_Relay_Shutdown(uint8_t state) {
	if (state == FEB_Relay_Close) {
		// Close Shutdown Relay

	} else if (state == FEB_Relay_Open) {
		// Open Shutdown Relay
	}
}

static void FEB_Relay_Precharge(uint8_t state) {
	if (state == FEB_Relay_Close) {
		// Close Precharge Relay
	} else if (state == FEB_Relay_Open) {
		// Open Precharge Relay
	}
}

static void FEB_Relay_Air_Plus(uint8_t state) {
	if (state == FEB_Relay_Close) {
		// Close Air+ Relay
	} else if (state == FEB_Relay_Open) {
		// Open Air+ Relay
	}
}

// ******************************** Interface ********************************

void FEB_Relay_Init(void) {
	static uint8_t guard = 0;
	if (guard != 0) {
		return;
	}
	guard = 1;
	FEB_Relay_Shutdown(FEB_Relay_Close);
}

void FEB_Relay_State_Precharge(void) {
	if (FEB_State_Shutdown) {
		return;
	}
	FEB_Relay_Precharge(FEB_Relay_Close);
	FEB_Relay_Air_Plus(FEB_Relay_Open);
}

void FEB_Relay_State_Balance(void) {
	if (FEB_State_Shutdown) {
		return;
	}
	FEB_Relay_Precharge(FEB_Relay_Open);
	FEB_Relay_Air_Plus(FEB_Relay_Open);
}

void FEB_Relay_State_Charge(void) {
	if (FEB_State_Shutdown) {
		return;
	}
	FEB_Relay_Precharge(FEB_Relay_Open);
	FEB_Relay_Air_Plus(FEB_Relay_Close);
}

void FEB_Relay_State_Drive(void) {
	if (FEB_State_Shutdown) {
		return;
	}
	FEB_Relay_Air_Plus(FEB_Relay_Close);
}

void FEB_Relay_State_Shutdown(void) {
	FEB_Relay_Shutdown(FEB_Relay_Open);
	FEB_Relay_Precharge(FEB_Relay_Open);
	FEB_Relay_Air_Plus(FEB_Relay_Open);
}

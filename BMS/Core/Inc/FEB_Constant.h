#ifndef INC_FEB_CONSTANT_H_
#define INC_FEB_CONSTANT_H_

// **************************************** Definitions ****************************************

// Accumulator configuration
#define FEB_CONSTANT_NUM_CELLS_PER_DAUGHTER_BOARD 12
#define FEB_CONSTANT_NUM_CELLS_PER_BANK 17
#define FEB_CONSTANT_NUM_BANKS 8

// Accumulator thresholds
#define FEB_CONSTANT_MIN_CURRENT -0.1
#define FEB_CONSTANT_MAX_CURRENT 60

// Cell thresholds
#define FEB_CONSTANT_CELL_MIN_TEMP (0 + 1)
#define FEB_CONSTANT_CELL_MAX_TEMP (60 - 1)
#define FEB_CONSTANT_CELL_MIN_VOLT (2.5 + 0.01)
#define FEB_CONSTANT_CELL_MAX_VOLT (4.2 - 0.01)

// Pre-charge
#define FEB_CONSTANT_PRECHARGE_PERCENT 0.9

// Charging
#define FEB_CONSTANT_MAX_CHARGING_CURRENT 7 	// [0, 12], 0.1A resolution

#endif /* INC_FEB_CONSTANT_H_ */

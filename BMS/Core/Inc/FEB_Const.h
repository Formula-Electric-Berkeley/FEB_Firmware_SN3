//#ifndef INC_FEB_CONST_H_
//#define INC_FEB_CONST_H_
//
//// **************************************** Definitions ****************************************
//
//// Accumulator configuration
//#define FEB_CONST_NUM_ACC_CELLS_PER_IC 10 		// Number of accumulator cells per LTC6811 IC
//#define FEB_CONST_NUM_LTC_CELLS_PER_IC 12		// Number of LTC6811 cells per LTC6811 IC
//#define FEB_CONST_NUM_CELLS_PER_BANK 20
//#define FEB_CONST_NUM_BANKS 1
//#define FEB_CONST_NUM_CELLS (FEB_CONST_NUM_CELLS_PER_BANK * FEB_CONST_NUM_BANKS)
//
//// Cell temperature thresholds
//// DC (discharge), CH (charge)
//#define FEB_CONST_CELL_MIN_TEMP_DC_C -20
//#define FEB_CONST_CELL_MAX_TEMP_DC_C 60
//#define FEB_CONST_CELL_MIN_TEMP_CH_C 0
//#define FEB_CONST_CELL_MAX_TEMP_CH_C 45
//
//#define FEB_CONST_CELL_MIN_TEMP_DC_100mC (FEB_CONST_CELL_MIN_TEMP_DC_C * 10)
//#define FEB_CONST_CELL_MAX_TEMP_DC_100mC (FEB_CONST_CELL_MAX_TEMP_DC_C * 10)
//#define FEB_CONST_CELL_MIN_TEMP_CH_100mC (FEB_CONST_CELL_MIN_TEMP_CH_C * 10)
//#define FEB_CONST_CELL_MAX_TEMP_CH_100mC (FEB_CONST_CELL_MAX_TEMP_CH_C * 10)
//
//// Cell voltage (V) thresholds
//#define FEB_CONST_CELL_MIN_VOLT_V 2.5
//#define FEB_CONST_CELL_MAX_VOLT_V 4.2
//#define FEB_CONST_CELL_MIN_VOLT_100uV (FEB_CONST_CELL_MIN_VOLT_V * 10000)
//#define FEB_CONST_CELL_MAX_VOLT_100uV (FEB_CONST_CELL_MAX_VOLT_V * 10000)
//
//// Cell current (A) thresholds
//// DC (discharge), CH (charge)
//#define FEB_CONST_MAX_CURR_DC 60
//#define FEB_CONST_MAX_CURR_CH 20
//
//// Pre-charge percent
//#define FEB_CONST_PRECHARGE_PCT 0.9
//
//// Number of temperature MUX channels
//#define FEB_CONST_NUM_TEMP_MUX_CH 5
//
//#endif /* INC_FEB_CONST_H_ */

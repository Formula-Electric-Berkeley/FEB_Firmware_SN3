#ifndef INC_FEB_PID_H_
#define INC_FEB_PID_H_

#include <stm32f4xx_hal.h>

typedef struct {

	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin; // set to 255
	float limMax; //set to 0

	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;
	float prevError;			/* Required for integrator */
	float differentiator;
	float prevMeasurement;		/* Required for differentiator */

	/* Controller output */
	float out;

} PIDController;

void FEB_PID_Init(PIDController *pid);
float FEB_PID_Update(PIDController *pid, float setpoint, float measurement);

#endif /* INC_FEB_PID_H_ */

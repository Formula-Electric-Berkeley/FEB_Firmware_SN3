#include <FEB_PID.h>

void FEB_PID_Init(PIDController *pid) {

	//Initialize constants
	pid->tau = 0; //time constant of the filter

	//TODO initalize the rest of the constants
	pid->limMax = 0;
	pid->limMin = 0;

	pid->limMaxInt = 0;
	pid->limMinInt = 0;

	pid->T = 0;

	// Clear controller variables
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;
}

float FEB_PID_Update(PIDController *pid, float setpoint, float measurement) {

	uint32_t error = setpoint - measurement;

	// Proportional term
	float proportional = pid->Kp * error;

    // Integral term
    pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

   	// Anti-wind-up via integrator clamping
   if (pid->integrator > pid->limMaxInt) {
	   pid->integrator = pid->limMaxInt;

   } else if (pid->integrator < pid->limMinInt) {
	   pid->integrator = pid->limMinInt;
   }

   // Derivative term
   pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)
                           + (2.0f * pid->tau - pid->T) * pid->differentiator)
                           / (2.0f * pid->tau + pid->T);

   // Calculate output
   pid->out = proportional + pid->integrator + pid->differentiator;

   if (pid->out > pid->limMax) {
	   pid->out = pid->limMax;

   } else if (pid->out < pid->limMin) {
	   pid->out = pid->limMin;
   }

   pid->prevError = error;
   pid->prevMeasurement = measurement;

   return pid->out;
}



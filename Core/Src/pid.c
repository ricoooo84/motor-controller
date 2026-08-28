#include "pid.h"

static float prev_error = 0;

float pid_update(float target, float actual) {
	static float integral = 0;
	float derivative;
	float error = target - actual;

	// integral, saturate
	integral += error * dt;

	if (integral > integral_max) integral = integral_max;
	else if (integral < -integral_max) integral = -integral_max;

	// derivative
	derivative = (error - prev_error) / dt;
	prev_error = error;

	// PID calculation, saturate
	float output = (kp * error) + (ki * integral) + (kd * derivative);

	if (output > output_max) output = output_max;
	else if (output < -output_max) output = -output_max;

	return output;
}

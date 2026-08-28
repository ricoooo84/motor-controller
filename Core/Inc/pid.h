#ifndef INC_PID_H_
#define INC_PID_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "motor.h"

#define kp		.1
#define ki		0
#define kd		0
#define dt		0.001f

#define integral_max	.1
#define output_max		255

float pid_update(float target, float actual);

#endif

#ifndef INC_PID_H_
#define INC_PID_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define kp		20
#define ki		10
#define kd		0
#define dt		0.001f

#define integral_max	100
#define output_max		255

float pid_update(float target, float actual);

#endif

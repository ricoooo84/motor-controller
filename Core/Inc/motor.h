#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "pwm_driver.h"

typedef struct {
	int16_t ideal_rpm;
	int16_t actual_rpm;
} motor_data_t;

void motor_move(int16_t move);
void motor_brake(void);

#endif /* INC_MOTOR_H_ */

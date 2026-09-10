#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>
#include "pwm_driver.h"

void motor_move(int16_t move);
void motor_brake(void);

#endif /* INC_MOTOR_H_ */

#include "motor.h"

void motor_move(int16_t move) {
	// move is -255 to 255
	if (move == 0) {
		pwm_duty1(PWM_TIMER, (uint8_t)move);
		pwm_duty2(PWM_TIMER, (uint8_t)move);
	} else if (move > 0) {
		pwm_duty1(PWM_TIMER, (uint8_t)move);
		pwm_duty2(PWM_TIMER, 0);
	} else if (move < 0) {
		pwm_duty1(PWM_TIMER, 0);
		pwm_duty2(PWM_TIMER, (uint8_t)(-move));
	}
}

void motor_brake(void) {
	pwm_duty1(PWM_TIMER,255);
	pwm_duty2(PWM_TIMER,255);
}

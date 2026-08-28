#include "control_driver.h"

static float delta_to_rpm(int32_t delta) {
	float revs_per_loop = delta / ENCODER_CPR; // revolutions in 0.001 seconds
	return revs_per_loop*60000.0f;
}

void control_config(TIM_TypeDef *timer) {

	timer->CR1 &= ~(TIM_CR1_CEN);

	timer->ARR = CONTROL_AUTORELOAD;
	timer->PSC = CONTROL_PRESCALER;

	// update = wraparound
	timer->DIER |= TIM_DIER_UIE;

	// **HARDCODED TO TIM4
	NVIC_SetPriority(TIM4_IRQn, 5);
	NVIC_EnableIRQ(TIM4_IRQn);

	timer->CR1 |= TIM_CR1_CEN;
}

void control_cfg_all(void) {
	control_config(CONTROL_TIMER);
	encoder_config(ENCODER_TIMER);
	pwm_config(PWM_TIMER);
}

void control_irq(TIM_TypeDef *timer) {
	if (timer->SR & TIM_SR_UIF) {
		timer->SR &= ~(TIM_SR_UIF);

		control_flag = 1;
	}
}

void control_loop(void) {
	// read adc, check to shutdown (reasoning is something about no more back EMF, like back EMF is good idk)
	// SAFE ADC FOR LATER, I DONT HAVE SHUNT RESISTOR

	// read encoder
	uint32_t delta = encoder_delta(ENCODER_TIMER);
	float actual_rpm = delta_to_rpm(delta);

	// pass into pid
	float pid_output = pid_update(target_rpm, actual_rpm);

	// drive motor
	motor_move(pid_output);
}

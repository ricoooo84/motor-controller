#include "pwm_driver.h"

// configure pwm on both channels
void pwm_config(TIM_TypeDef *timer) {

	timer->CR1 &= ~(TIM_CR1_CEN);

	timer->ARR = PWM_AUTORELOAD;
	timer->PSC = PWM_PRESCALER;

	// PWM mode 1, set preloads for duty cycle and max count
	timer->CCMR1 |= (0x6 << TIM_CCMR1_OC1M_Pos) | (TIM_CCMR1_OC1PE);
	timer->CCMR1 |= (0x6 << TIM_CCMR1_OC2M_Pos) | (TIM_CCMR1_OC2PE);
	timer->CR1 |= TIM_CR1_ARPE;

	// load changes above with update event
	timer->EGR |= TIM_EGR_UG;
	timer->SR &= ~(TIM_SR_UIF);

	// default 0% duty cycle
	timer->CCR1 = 0;
	timer->CCR2 = 0;

	// trigger output for ADC sample
//	timer->CR2 |= (2 << TIM_CR2_MMS_Pos);

	timer->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
	timer->CR1 |= TIM_CR1_CEN;
}

// duty is 0-255
void pwm_duty1(TIM_TypeDef *timer, uint8_t duty) {
	timer->CCR1 = (uint32_t)((timer->ARR + 1)*duty)/255;
}

void pwm_duty2(TIM_TypeDef *timer, uint8_t duty) {
	timer->CCR2 = (uint32_t)((timer->ARR + 1)*duty)/255;
}

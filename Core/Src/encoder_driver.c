#include "encoder_driver.h"

static uint16_t prev_count = 0;

void encoder_config(TIM_TypeDef *timer) {

	timer->CR1 &= ~(TIM_CR1_CEN);

	timer->PSC = ENCODER_PRESCALER;
	timer->ARR = ENCODER_AUTORELOAD;

	// map inputs
	timer->CCMR1 |= 1 << TIM_CCMR1_CC1S_Pos;
	timer->CCMR1 |= 1 << TIM_CCMR1_CC2S_Pos;

	// filter (no clock divide, 8 consecutive samples)
	timer->CCMR1 |= (3 << TIM_CCMR1_IC1F_Pos) | (3 << TIM_CCMR1_IC2F_Pos);

	// slave configuration (triggered by external encoder)
	timer->SMCR |= 3 << TIM_SMCR_SMS_Pos;

	timer->CNT = 0;
	timer->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
	timer->CR1 |= TIM_CR1_CEN;
}

uint32_t encoder_count(TIM_TypeDef *timer) {
	return (uint32_t)timer->CNT;
}

void encoder_zero(TIM_TypeDef *timer) {
	timer->CNT = 0;
}

int32_t encoder_delta(TIM_TypeDef *timer) {
	// this would break if 32767 counts in 1 tick, but thats unrealistic

	uint16_t cur_count = encoder_count(timer);
	int32_t delta = (int32_t)(int16_t)(cur_count - prev_count); // int16_t cast first to ensure 16-bit wrap

	prev_count = cur_count;

	return delta;
}

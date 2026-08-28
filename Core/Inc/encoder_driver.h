#ifndef INC_ENCODER_DRIVER_H_
#define INC_ENCODER_DRIVER_H_

#include "stm32f4xx.h"
#include <stdint.h>

// 16 MHz Clock
#define ENCODER_MAXVAL		4294967295
#define ENCODER_AUTORELOAD	ENCODER_MAXVAL
#define ENCODER_PRESCALER	0

#define ENCODER_CPR			2797.0f	// per datasheet

#define ENCODER_TIMER		TIM2

#define EXTEN_RISING		1
#define EXTSEL_PWM			3

void encoder_config(TIM_TypeDef *timer);
uint32_t encoder_count(TIM_TypeDef *timer);
void encoder_zero(TIM_TypeDef *timer);
int32_t encoder_delta(TIM_TypeDef *timer);

#endif /* INC_ENCODER_DRIVER_H_ */

#ifndef INC_PWM_DRIVER_H_
#define INC_PWM_DRIVER_H_

#include "stm32f4xx.h"
#include <stdint.h>

// 16 MHz Clock
#define PWM_MAXVAL		65535
#define PWM_AUTORELOAD	799	// 20 kHz PWM
#define PWM_PRESCALER	0	// maximize resolution

#define PWM_TIMER		TIM3

void pwm_config(TIM_TypeDef *timer);
void pwm_duty1(TIM_TypeDef *timer, uint8_t duty);
void pwm_duty2(TIM_TypeDef *timer, uint8_t duty);

#endif /* INC_PWM_DRIVER_H_ */

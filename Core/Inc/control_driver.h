#ifndef INC_CONTROL_DRIVER_H_
#define INC_CONTROL_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "encoder_driver.h"
#include "adc_dma_driver.h"
#include "motor.h"
#include "pid.h"

// 16 MHz Clock
#define CONTROL_MAXVAL		65535
#define CONTROL_AUTORELOAD	15999	// 1 kHz
#define CONTROL_PRESCALER	0

#define CONTROL_TIMER		TIM4

extern volatile uint8_t control_flag;
extern volatile float target_rpm;

void control_config(TIM_TypeDef *timer);
void control_cfg_all(void);
void control_irq(TIM_TypeDef *timer);
void control_loop(void);

#endif /* INC_CONTROL_DRIVER_H_ */

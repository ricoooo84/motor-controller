#ifndef INC_CONTROL_LOOP_H_
#define INC_CONTROL_LOOP_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "encoder_driver.h"
#include "motor.h"
#include "pid.h"
#include "uart_driver.h"

// 16 MHz Clock
#define CONTROL_MAXVAL		65535
#define CONTROL_AUTORELOAD	15999	// 1kHz
#define CONTROL_PRESCALER	0
#define CONTROL_TIMER		TIM4
#define COUNTS_FOR_RPM		20
#define COUNTS_FOR_UART		500

extern volatile uint8_t control_flag;
extern uart_t u;

void control_config(TIM_TypeDef *timer);
void control_cfg_all(void);
void control_irq(TIM_TypeDef *timer);
void control_loop(void);
void control_task(void *parameters);


#endif /* INC_CONTROL_LOOP_H_ */

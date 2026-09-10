#include "control_loop.h"
#include "stm32f4xx_hal.h"

float pid_output;
static int32_t delta;
static float actual_rpm;
static uint32_t loop_count = 0;
static int16_t target_rpm;
static uint8_t input_line[RX_BUF_SIZE];
static char output_line[TX_BUF_SIZE];

static float delta_to_rpm(int32_t delta) {
	float revs_per_loop = (float)delta / ENCODER_CPR;
	return revs_per_loop*(60000.0f/COUNTS_FOR_RPM);
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


	loop_count++;

	// read encoder
	delta += encoder_delta(ENCODER_TIMER);

	// pass into pid
	pid_output = pid_update(target_rpm, actual_rpm);

	// drive motor

	//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
	motor_move(pid_output);

	if (loop_count % COUNTS_FOR_RPM == 0) {
		actual_rpm = delta_to_rpm(delta);
		delta = 0;
	}
	if (loop_count % COUNTS_FOR_UART == 0) {
		uart_read_line(&u, input_line);
		uart_parse_int(input_line, &target_rpm);
		sprintf(output_line, "target rpm: %d\tactual rpm: %f\tpid output: %f\r\n", target_rpm, actual_rpm, pid_output);
		uart_write(&u, output_line);
	}
}

void control_task(void *parameters) {
	(void)parameters;

	for (;;) {
		//xSemaphoreTake(control_sem, portMAX_DELAY);
		control_loop();
	}
}

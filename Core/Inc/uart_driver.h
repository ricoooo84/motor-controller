#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>
#include "stm32f4xx.h"

#define RX_BUF_SIZE	16
#define TX_BUF_SIZE 64

typedef struct {
	USART_TypeDef *uartTD;

	volatile uint8_t rx_buf[RX_BUF_SIZE];
	volatile uint8_t rx_head;
	volatile uint8_t rx_tail;


	volatile uint8_t tx_buf[TX_BUF_SIZE];
	volatile uint8_t tx_head;
	volatile uint8_t tx_tail;
} uart_t;

void uart_cfg(uart_t *uart);
void uart_irq_handler(uart_t *uart);
uint8_t uart_read_line(uart_t *uart, uint8_t *buf);
uint8_t uart_write(uart_t *uart, const char *str);
uint8_t uart_parse_int(const uint8_t *buf, int16_t *out);

#endif

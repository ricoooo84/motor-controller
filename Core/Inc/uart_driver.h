#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>
#include "stm32f4xx.h"

#define UART_TX_SIZE	64
#define UART_RX_SIZE	64



typedef enum {
	UART_OK,
	UART_INVALID_ARG,
	UART_NOT_READY
} uart_status_t;

typedef enum {
	UART_READY,
	UART_BUSY,
	UART_COMPLETE,
	UART_ERROR
} uart_state_t;

typedef enum {
	UART_NO_ERROR,
	UART_OVERRUN,
	UART_NOISE,
	UART_FRAMING,
	UART_PARITY
} uart_error_t;

typedef struct {
	USART_TypeDef *uart_typedef;

	uint8_t tx_buf[UART_TX_SIZE];
	volatile uint16_t tx_len;
	volatile uint16_t tx_idx;
	volatile uart_state_t tx_state;

	uint8_t rx_buf[UART_RX_SIZE];
	volatile uint16_t rx_len;
	volatile uint16_t rx_idx;
	volatile uart_state_t rx_state;

	volatile uart_error_t rx_error;
} uart_t;

uart_status_t uart_write(uart_t *uart, uint8_t *buf, uint16_t len);
uart_status_t uart_read(uart_t *uart, uint16_t len);
uart_status_t uart_take(uart_t *uart, uint8_t *buf, uint16_t len);
void uart_irq(uart_t *uart);




#endif

#include "uart_driver.h"

void uart_cfg(uart_t *uart) {
	USART_TypeDef *uartTD = uart->uartTD;

	uartTD->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn); // warning: hardcoded to usart2
}

void uart_irq_handler(uart_t *uart) {
	// snapshot of status register
	uint32_t sr = uart->uartTD->SR;

	if (sr & USART_SR_RXNE) {
		uint8_t byte = (uint8_t)uart->uartTD->DR;
		uint8_t next = (uint8_t)((uart->rx_head+1) & (RX_BUF_SIZE-1));
		if (next != uart->rx_tail) {
			uart->rx_buf[uart->rx_head] = byte;
			uart->rx_head = next; // cannot simply increment due to ring edge case
		}
	}

	if (sr & USART_SR_TXE) {
		if (uart->tx_tail != uart->tx_head) {
			uart->uartTD->DR = uart->tx_buf[uart->tx_tail];
			uart->tx_tail = (uint8_t)((uart->tx_tail+1) & (TX_BUF_SIZE - 1));
		} else {
			// string done sending, stop triggering txe
			uart->uartTD->CR1 &= ~USART_CR1_TXEIE;
		}
	}
}

uint8_t uart_read_line(uart_t *uart, uint8_t *buf) {

	// find \r\n ending
	uint8_t idx = uart->rx_tail;
	uint8_t len = 0;

	while (idx != uart->rx_head) {
		char cur = uart->rx_buf[idx];

		if (cur == '\r' || cur == '\n') {
			break;
		} else {
			len++;
			idx = (uint8_t)((idx+1) & (RX_BUF_SIZE - 1));
		}
	}

	if (idx == uart->rx_head) {
		return 2;
	}

	// DOESNT HANDLE TRUNCATION
	// could break if wraps around, shouldnt happen though
	for (uint8_t i = 0; i < len; i++) {
		buf[i] = uart->rx_buf[uart->rx_tail];
		uart->rx_tail = (uint8_t)((uart->rx_tail + 1) & (RX_BUF_SIZE - 1));
	}
	buf[len] = '\0';

	// move tail past terminator
	while (uart->rx_tail != uart->rx_head) {
		uint8_t c = uart->rx_buf[uart->rx_tail];

		if (c != '\r' && c != '\n') {
			break;
		}

		uart->rx_tail = (uint8_t)((uart->rx_tail + 1) & (RX_BUF_SIZE-1));
	}

	return 0;

}

uint8_t uart_write(uart_t *uart, const char *str) {

	// read string into tx_buf
	while (*str) {
		uint8_t next = (uint8_t)((uart->tx_head+1) & (TX_BUF_SIZE-1));

		if (next == uart->tx_tail) {
			return 1;
		} else {
			uart->tx_buf[uart->tx_head] = (uint8_t)*str++;
			uart->tx_head = next;
		}
	}

	uart->uartTD->CR1 |= USART_CR1_TXEIE;
	return 0;
}

uint8_t uart_parse_int(const uint8_t *buf, int16_t *out) {
	// check valid string
	if (buf == NULL || *buf == '\0') {
		return 1;
	}

	// check negative
	uint8_t negative = 0;
	if (*buf == '-') {
		negative = 1;
		buf++;
	}

	// magnitude (WARNING: only works with valid input)
	int16_t mag = 0;
	while (*buf) {
		uint8_t digit = *buf - '0';
		mag = mag*10 + digit;

		buf++;
	}

	if (negative) {
		mag *= -1;
	}
	*out = mag;

	return 0;
}

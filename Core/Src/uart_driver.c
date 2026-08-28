#include "uart_driver.h"
#include "string.h"

uart_status_t uart_write(uart_t *uart, uint8_t *buf, uint16_t len) {
	if ((buf == NULL) || (len > UART_TX_SIZE) || (len == 0)) {
		return UART_INVALID_ARG;
	}
	if (uart->tx_state == UART_BUSY) {
		return UART_NOT_READY;
	}

	memcpy(uart->tx_buf, buf, len);
	uart->tx_idx = 0;
	uart->tx_len = len;
	uart->tx_state = UART_BUSY;

	// load first byte, turn on interrupt to trigger chain
	uart->uart_typedef->DR = uart->tx_buf[uart->tx_idx];
	uart->tx_idx++;
	uart->uart_typedef->CR1 |= USART_CR1_TXEIE;

	return UART_OK;
}

uart_status_t uart_read(uart_t *uart, uint16_t len) {
	if ((len > UART_RX_SIZE) || (len == 0)) {
		return UART_INVALID_ARG;
	}
	if (uart->rx_state == UART_BUSY) {
		return UART_NOT_READY;
	}

	uart->rx_idx = 0;
	uart->rx_len = len;
	uart->rx_state = UART_BUSY;
	uart->rx_error = UART_NO_ERROR;

	uart->uart_typedef->CR1 |= USART_CR1_RXNEIE;

	return UART_OK;
}

uart_status_t uart_take(uart_t *uart, uint8_t *buf, uint16_t len) {
	if (uart->rx_state != UART_COMPLETE) {
		return UART_NOT_READY;
	}

	if ((buf == NULL)|| (len > uart->rx_len)) {
		return UART_INVALID_ARG;
	}

	memcpy(buf, uart->rx_buf, len);
	uart->rx_state = UART_READY;

	return UART_OK;
}

void uart_irq(uart_t *uart) {
	// read once so flags so correct flag is handled at time of irq trigger
	uint32_t sr = uart->uart_typedef->SR;

	// error
	if ((sr) & (USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE)) {
		// dummy reads to clear error flag
		(void)uart->uart_typedef->DR;

		if (sr & USART_SR_ORE) {
			uart->rx_error = UART_OVERRUN;
		} else if (sr & USART_SR_NE) {
			uart->rx_error = UART_NOISE;
		} else if (sr & USART_SR_FE) {
			uart->rx_error = UART_FRAMING;
		} else if (sr & USART_SR_PE) {
			uart->rx_error = UART_PARITY;
		}
	}

	// rx
	else if ((sr & USART_SR_RXNE) && (uart->uart_typedef->CR1 & USART_CR1_RXNEIE)) {
		// reading DR clears RXNE
		uint8_t byte = uart->uart_typedef->DR;

		if (uart->rx_idx < uart->rx_len) {
			uart->rx_buf[uart->rx_idx] = byte;
			uart->rx_idx++;

			if (uart->rx_idx == uart->rx_len) {
				uart->uart_typedef->CR1 &= ~(USART_CR1_RXNEIE);
				uart->rx_state = UART_COMPLETE;
			}
		}
	}

	// tx
	else if ((sr & USART_SR_TXE) && (uart->uart_typedef->CR1 & USART_CR1_TXEIE)) {
		if (uart->tx_idx == uart->tx_len) {
			uart->uart_typedef->CR1 &= ~(USART_CR1_TXEIE);
			uart->tx_state = UART_READY;
		} else {
			uart->uart_typedef->DR = uart->tx_buf[uart->tx_idx];
			uart->tx_idx++;
		}
	}

}

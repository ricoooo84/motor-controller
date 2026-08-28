#include "adc_dma_driver.h"

void adc_start(void) {
	// dma
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream0->CR & DMA_SxCR_EN);


	DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
	DMA2_Stream0->M0AR = (uint32_t)&adc_buf;
	DMA2_Stream0->NDTR = 1;

	// CR's chsel: channel 0 (ADC1), already implied
	DMA2_Stream0->CR |= (DMA_SxCR_CIRC | (1 << DMA_SxCR_PSIZE_Pos) | (1 << DMA_SxCR_MSIZE_Pos) );

	DMA2_Stream0->CR |= DMA_SxCR_EN;


	// adc
	ADC1->SQR3 |= (ADC_CHANNEL << 0);
	ADC1->SMPR2 |= (2 << ADC_CHANNEL*3); // 28 cycles
	ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS; // dma config
	ADC1->CR2 |= ((1 << ADC_CR2_EXTEN_Pos) | (PWM_TRGO << ADC_CR2_EXTSEL_Pos)); // external trigger config
	ADC1->CR2 |= ADC_CR2_EOCS; // trivial but recommended with any DMA

	ADC1->CR2 |= ADC_CR2_ADON;

	// no SWSTART because externally triggered by PWM TRGO
}

void adc_amps(void) {

}

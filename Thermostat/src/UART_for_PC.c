/*
 * UART_for_PC.c
 *
 *  Created on: 13 ���. 2023 �.
 *      Author: denlo
 */

#include "UART_for_PC.h"

void DMA1_Channel4_5_IRQHandler(void) {
	if ((DMA1->ISR & DMA_ISR_TCIF4) == DMA_ISR_TCIF4) {
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
		DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	}
	if ((DMA1->ISR & DMA_ISR_TCIF5) == DMA_ISR_TCIF5) {
		DMA1->IFCR |= DMA_IFCR_CTCIF5;
	}
}

void init_GPIO_for_USART() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//IO - AF
	PORT_USART->MODER |= GPIO_USART_TX | GPIO_USART_RX;
	//AF1 on PA9(UART_TX) and PA10(UART_RX
	PORT_USART->AFR[1] |= (1 << GPIO_AFRH_AFRH1_Pos) | (1 << GPIO_AFRH_AFRH2_Pos);
}

void init_USART() {
	init_GPIO_for_USART();
	init_DMA_for_USART();

	RCC->APB2ENR |= RCC_USART;
	USART->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART->BRR = SystemCoreClock / 115200;
	USART->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
	USART->CR1 |= USART_CR1_UE;
}


void init_DMA_for_USART() {
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	//remap USART1 DMA on channel 4 and 5
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1RX_DMA_RMP | SYSCFG_CFGR1_USART1TX_DMA_RMP;

	//USART TX channel - 4
	DMA1_Channel4->CCR |= DMA_CCR_DIR | DMA_CCR_MINC;
	DMA1_Channel4->CMAR = (uint32_t)(&temperature);
	DMA1_Channel4->CPAR = (uint32_t)(&(USART->TDR));
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;
	DMA1_Channel4->CNDTR = 0;

	//USART RX channel - 5
	DMA1_Channel5->CCR &= ~DMA_CCR_DIR;
	DMA1_Channel5->CCR |= DMA_CCR_MINC | DMA_CCR_CIRC;
	DMA1_Channel5->CMAR = (uint32_t)(&(UART_rx_buf[0]));
	DMA1_Channel5->CPAR = (uint32_t)(&(USART->RDR));
	DMA1_Channel5->CNDTR = 2;
	DMA1_Channel5->CCR |= DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
	NVIC_SetPriority(DMA1_Channel4_5_IRQn, 3);

	DMA1_Channel5->CCR |= DMA_CCR_EN;

}

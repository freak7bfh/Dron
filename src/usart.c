/*
 * usart.c
 *
 *  Created on: 23-02-2015
 *      Author: Jarek
 */

#include "stm32f0xx_conf.h"
#include "string.h"
#include "main.h"

void USARTInit(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// piny							tx		   rx
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6|GPIO_PinSource7, GPIO_AF_0);
	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);

	// usart
	USART_InitStructure.USART_BaudRate 			  = 57600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode				  = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity			  = USART_Parity_No;
	USART_InitStructure.USART_StopBits			  = USART_StopBits_1;
	USART_InitStructure.USART_WordLength		  = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);

	// NVIC dla USART
	NVIC_InitTypeDef NVIC_UART1_InitStructure;
	NVIC_UART1_InitStructure.NVIC_IRQChannel 				= USART1_IRQn;
	NVIC_UART1_InitStructure.NVIC_IRQChannelPriority 		= 0;
	NVIC_UART1_InitStructure.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_Init(&NVIC_UART1_InitStructure);

	NVIC_EnableIRQ(USART1_IRQn);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

void USARTSendNow(char *s){
	uint16_t len = strlen(s);
	for (uint16_t i = 0; i < len; i++){
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, s[i]);
	}
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}

void USARTSendFrame(uint8_t *frame, uint16_t frame_len){
	for (uint16_t i = 0; i < frame_len; i++){
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, frame[i]);
	}
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}


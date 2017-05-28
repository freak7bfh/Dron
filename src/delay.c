/*

 * delay.c
 *
 *  Created on: 23-02-2015
 *      Author: Jarek
 */

#include "stm32f0xx_conf.h"
#include "delay.h"
#include "main.h"

void DelayInit(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);

	TIM_TimeBaseInitTypeDef TIM14_InitStruct;
	TIM_TimeBaseStructInit(&TIM14_InitStruct);

	TIM14_InitStruct.TIM_ClockDivision		= TIM_CKD_DIV1;
	TIM14_InitStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM14_InitStruct.TIM_Period				= 0xffff;
	TIM14_InitStruct.TIM_Prescaler			= SYS_CLOCK-1;
	TIM14_InitStruct.TIM_RepetitionCounter	= 0x0000;

	TIM_TimeBaseInit(TIM14, &TIM14_InitStruct);

	TIM_Cmd(TIM14,ENABLE);

//	USARTSendNow("DelayInit...\n\r");
}


void Delay_us(uint16_t us) {
	TIM14->CNT = 0;
	while((uint16_t)(TIM14->CNT) <= us);
}

void Delay_ms(uint16_t ms) {
	int i;
	for (i=0; i < ms; i++){
		Delay_us(1000);
	}
}

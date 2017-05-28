/*
 * motors.c
 *
 *  Created on: May 7, 2017
 *      Author: Jarek
 */

#include "stm32f0xx_conf.h"
#include "main.h"
#include "motors.h"

void MotorsPWMInit(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef MOTORS_InitStruct;
	MOTORS_InitStruct.GPIO_Pin		= MOTOR1|MOTOR2|MOTOR3|MOTOR4;
	MOTORS_InitStruct.GPIO_Mode		= GPIO_Mode_AF;
	MOTORS_InitStruct.GPIO_OType	= GPIO_OType_PP;
	MOTORS_InitStruct.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	MOTORS_InitStruct.GPIO_Speed	= GPIO_Speed_10MHz;
	GPIO_Init(MOTORS_GPIO,&MOTORS_InitStruct);

	GPIO_PinAFConfig(MOTORS_GPIO,GPIO_PinSource8,GPIO_AF_2);
	GPIO_PinAFConfig(MOTORS_GPIO,GPIO_PinSource9,GPIO_AF_2);
	GPIO_PinAFConfig(MOTORS_GPIO,GPIO_PinSource10,GPIO_AF_2);
	GPIO_PinAFConfig(MOTORS_GPIO,GPIO_PinSource11,GPIO_AF_2);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef TIM1_InitStruct;
	TIM1_InitStruct.TIM_ClockDivision		= TIM_CKD_DIV1;
	TIM1_InitStruct.TIM_CounterMode			= TIM_CounterMode_Up;
	TIM1_InitStruct.TIM_Period				= 100;			// freq
	TIM1_InitStruct.TIM_Prescaler			= (SYS_CLOCK-1)*10;
	TIM1_InitStruct.TIM_RepetitionCounter	= 0;

	TIM_TimeBaseInit(TIM1, &TIM1_InitStruct);
	TIM_Cmd(TIM1,ENABLE);

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode 		= TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState	= TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse 			= 2;				// %
	TIM_OCInitStruct.TIM_OCPolarity 	= TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCIdleState 	= TIM_OCIdleState_Set;

	TIM_OC1Init(TIM1, &TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_Pulse = 2;
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_Pulse = 2;
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_Pulse = 2;
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);



	TIM_CtrlPWMOutputs(TIM1, ENABLE);


//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	uint16_t TimerPeriod = 0;
//	uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;
//
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	/* GPIOA, GPIOB and GPIOE Clocks enable */
//	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
//
//	/* GPIOA Configuration: Channel 1, 2, 3, 4 and Channel 1N as alternate function push-pull */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);
//
//	TimerPeriod = (SystemCoreClock / 17570 ) - 1;
//	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
//	Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
//	/* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
//	Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
//	/* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
//	Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
//	/* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
//	Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);
//
//	/* TIM1 clock enable */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
//
//	/* Time Base configuration */
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//
//	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
//
//	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
//
//	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
//
//	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
//	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
//
//	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
//	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
//
//	TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
//	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
//
//	/* TIM1 counter enable */
//	TIM_Cmd(TIM1, ENABLE);
//
//	/* TIM1 Main Output Enable */
//	TIM_CtrlPWMOutputs(TIM1, ENABLE);


}

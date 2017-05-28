/*
 * main.h
 *
 *  Created on: 12 pa� 2016
 *      Author: Jarek
 */
#include "string.h"

#ifndef MAIN_H_
#define MAIN_H_

#define SYSTICK_CLK_FREQ 48000000
#define SYS_CLOCK 48	// MHz

#define LEDS_PORT	GPIOA
#define LEDG_PIN	GPIO_Pin_3
#define LEDR_PIN	GPIO_Pin_4
#define LEDB_PIN	GPIO_Pin_5

#define B_ON  LEDS_PORT->BSRR=LEDB_PIN
#define B_OFF LEDS_PORT->BRR=LEDB_PIN
#define G_ON  LEDS_PORT->BSRR=LEDG_PIN
#define G_OFF LEDS_PORT->BRR=LEDG_PIN


#define WAIT for(volatile uint16_t i=0; i<0x7f; i++)

#define IS_BIT_SET(m,n)     ((m) &   (0x01 << (n)))
#define SET_BIT(m,n)        ((m) |=  (0x01 << (n)))
#define RESET_BIT(m,n)      ((m) &= ~(0x01 << (n)))
#define XOR_BIT(m,n)		((m) ^=  (0x01 << (n)))


#endif /* MAIN_H_ */

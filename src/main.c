/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "main.h"
#include "rcc.h"
#include "usart.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "motors.h"
#include "adc.h"


volatile uint8_t RxIndeksRS=0;
volatile uint8_t RxBufRS[RS_BUFF];
volatile uint8_t systick_leds=0x00;
volatile uint16_t adc_val=0;
volatile uint8_t bat_led_flags=0;
volatile uint8_t led_flags=0;

char str_buf[8]={0};
uint8_t duty=0;
uint16_t voltage=10000;


int main(){

	RCCInit();
	USARTInit();
//
	_puts("\x1b[2J\x1b[H");  // clear terminal
	_puts("\x1b[?25l");	// cursor invisible
//
	_putsn("QUADRO-COPTER MOTOR DRIVER v0.1");
	_putsn("(C) 2017\r\n");

	_putsn("UATRT Init   OK");

	//SystemInit(); // ac6 external clock init
	SysTick_Config(SYSTICK_CLK_FREQ/100);	// systick period 10ms
	_putsn("SysTick Init OK");



	// leds
	GPIO_InitTypeDef LEDS_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	LEDS_InitStruct.GPIO_Pin	= LEDG_PIN|LEDR_PIN|LEDB_PIN;
	LEDS_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	LEDS_InitStruct.GPIO_OType	= GPIO_OType_PP;
	LEDS_InitStruct.GPIO_Speed	= GPIO_Speed_10MHz;
	GPIO_Init(LEDS_PORT,&LEDS_InitStruct);

	_putsn("LEDS Init    OK");

	B_ON;

	DelayInit();
	_putsn("DELAY Init   OK");

	MotorsPWMInit();
	_putsn("MOTORS Init  OK");

	ADCInit();
	_putsn("ADC Init     OK\r\n");

	_putsn("\r\nChange duty: '+' / '-'\r\n");



	B_OFF;

	while(1){
		SET_BIT(led_flags,0);



		adc_val=GetVoltage();
		if(voltage>adc_val || duty!=TIM1->CCR1){
			_puts("\x1b[15;0H");	// move to line 15
			_puts("\x1b[2K\x1b[G");	// clear line
			_puts("Battery voltage: ");
			itoa(adc_val,str_buf,10);
			_puts(str_buf);
			_puts(" mV ");

			_puts("\x1b[16;0H");	// move to line 16
			_puts("\x1b[2K\x1b[G");	// clear line
			_puts("Battery condition: ")

			if(adc_val>=4000){
				_puts("FULL");
				bat_led_flags=0;
			}
			if(adc_val>=3700 && adc_val<4000){
				_puts("GOOD");
				bat_led_flags=0;
			}
			if(adc_val>=3550 && adc_val<3700){
				_puts("LOW WARNING");
				bat_led_flags=1;
			}
			if(adc_val<3550){
				_puts("CRITICAL LOW - TURN THE DEVICE OFF IMMEDIATELY");
				bat_led_flags=2;
			}
			voltage=adc_val;
		}
		//_puts("\x1b[3A"); // move cursor 2 lines up

		if(duty!=TIM1->CCR1){
			_puts("\x1b[14;0H");	// move to line 14
			_puts("\x1b[2K\x1b[G");	// clear line
			_puts("Motor power: ");
			itoa(TIM1->CCR1,str_buf,10);
			_puts(str_buf);
			_puts("%");
			duty=TIM1->CCR1;
		}


		Delay_ms(500);
	}


	return 0;
}

// ====== IRQ Handlers ======

// USART1
void USART1_IRQHandler(){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){
		RxBufRS[RxIndeksRS++]=USART_ReceiveData(USART1);

		if(RxBufRS[0]=='+' || RxBufRS[0]=='='){
			if(TIM1->CCR1 < 100){
				TIM1->CCR1++;
				TIM1->CCR2++;
				TIM1->CCR3++;
				TIM1->CCR4++;
			}
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='-'){
			if(TIM1->CCR1 > 0){
				TIM1->CCR1--;
				TIM1->CCR2--;
				TIM1->CCR3--;
				TIM1->CCR4--;
			}
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='0'){
			TIM1->CCR1=0;
			TIM1->CCR2=0;
			TIM1->CCR3=0;
			TIM1->CCR4=0;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='1'){
			TIM1->CCR1=10;
			TIM1->CCR2=10;
			TIM1->CCR3=10;
			TIM1->CCR4=10;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='2'){
			TIM1->CCR1=20;
			TIM1->CCR2=20;
			TIM1->CCR3=20;
			TIM1->CCR4=20;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='3'){
			TIM1->CCR1=30;
			TIM1->CCR2=30;
			TIM1->CCR3=30;
			TIM1->CCR4=30;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='4'){
			TIM1->CCR1=40;
			TIM1->CCR2=40;
			TIM1->CCR3=40;
			TIM1->CCR4=40;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='5'){
			TIM1->CCR1=50;
			TIM1->CCR2=50;
			TIM1->CCR3=50;
			TIM1->CCR4=50;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='6'){
			TIM1->CCR1=60;
			TIM1->CCR2=60;
			TIM1->CCR3=60;
			TIM1->CCR4=60;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='7'){
			TIM1->CCR1=70;
			TIM1->CCR2=70;
			TIM1->CCR3=70;
			TIM1->CCR4=70;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='8'){
			TIM1->CCR1=80;
			TIM1->CCR2=80;
			TIM1->CCR3=80;
			TIM1->CCR4=80;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='9'){
			TIM1->CCR1=90;
			TIM1->CCR2=90;
			TIM1->CCR3=90;
			TIM1->CCR4=90;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='*'){
			TIM1->CCR1=100;
			TIM1->CCR2=100;
			TIM1->CCR3=100;
			TIM1->CCR4=100;
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}

		if(RxBufRS[0]=='/'){
			NVIC_SystemReset();
		}


		else{
			RxIndeksRS=0;
			RxBufRS[0]=0;
		}


		if(RxIndeksRS>=RS_BUFF){
			RxIndeksRS=0;
			for(uint8_t i=0; i<RS_BUFF; i++) RxBufRS[i]=0;
		}
	}
	USART_ClearFlag(USART1, USART_IT_RXNE);
}

// SysTick
void SysTick_Handler(){
	if(++systick_leds==100) systick_leds=0;

	if(bat_led_flags==0) LEDS_PORT->BRR=LEDR_PIN;
	if(bat_led_flags==1 && systick_leds==0)  LEDS_PORT->ODR^=LEDR_PIN;
	if(bat_led_flags==2 && (systick_leds==0  || systick_leds==10 || systick_leds==20 || systick_leds==30 ||
							systick_leds==40 || systick_leds==50 || systick_leds==60 || systick_leds==70 ||
							systick_leds==80 || systick_leds==90))  LEDS_PORT->ODR^=LEDR_PIN;

	if(IS_BIT_SET(led_flags,0) && systick_leds==0){
		LEDS_PORT->BSRR=LEDG_PIN;
	}
	if(IS_BIT_SET(led_flags,0) && systick_leds==2){
			LEDS_PORT->BRR=LEDG_PIN;
			RESET_BIT(led_flags,0);
		}

	//if(systick_leds==70) LEDS_PORT->ODR^=LEDR_PIN;
	//if(systick_leds==70) LEDS_PORT->BSRR=LEDR_PIN;

	//if(systick_leds==0) LEDS_PORT->BSRR=LEDB_PIN; if(systick_leds==20) LEDS_PORT->BRR=LEDB_PIN;
	//if(systick_leds==20) LEDS_PORT->BSRR=LEDR_PIN; if(systick_leds==40) LEDS_PORT->BRR=LEDR_PIN;
	//if(systick_leds==40) LEDS_PORT->BSRR=LEDG_PIN; if(systick_leds==60) LEDS_PORT->BRR=LEDG_PIN;
}



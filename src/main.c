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
#include "lis3dh.h"
#include "math.h"


volatile uint8_t RxIndeksRS=0;
volatile uint8_t RxBufRS[RS_BUFF];
volatile uint8_t systick_leds=0;
volatile uint16_t adc_val=0;
volatile uint8_t bat_led_flags=0;
volatile uint8_t led_flags=0;

char str_buf[8]={0};
uint8_t duty=1;
uint16_t voltage=10000;

// lis *****************
uint8_t test_frame[8]={100,200,123,24,115,166,217,80};

uint8_t MSB, LSB;
int16_t Xg, Zg;                                 // 16-bit values from accelerometer
int16_t x_array[100];                           // 100 samples for X-axis
int16_t z_array[100];                           // 100 samples for Z-axis
float x_average;                                // x average of samples
float z_average;                                // z average of samples
float zx_theta;                                 // degrees between Z and X planes

uint8_t test_val=0;

// *********************

int main(){

	//RCCInit();
	SystemInit();

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
	_putsn("ADC Init     OK");

	Lis3dhInit();
	_putsn("LIS3DH Init  TEST\r\n");


	_putsn("\r\nChange duty: '+' / '-'\r\n");



	B_OFF;




// lis ********************
//
//	/* ---------------- Local Macros ----------------- */
//
//	/* set read single command. Attention: command must be 0x3F at most */
//	#define SET_READ_SINGLE_CMD(x)			(x | 0x80)
//	/* set read multiple command. Attention: command must be 0x3F at most */
//	#define SET_READ_MULTI_CMD(x)			(x | 0xC0)
//	/* set write single command. Attention: command must be 0x3F at most */
//	#define SET_WRITE_SINGLE_CMD(x)			(x & (~(0xC0)))
//	/* set write multiple command. Attention: command must be 0x3F at most */
//	#define SET_WRITE_MULTI_CMD(x)			(x & (~(0x80)) x |= 0x40)

//	while(1){
//		SPI_SendData8(SPI1, 0x0f);
//		Delay_ms(10);
//		test_val=SPI_ReceiveData8(SPI1);
//		Delay_ms(10);
//		USART_SendData(USART1,test_val);
//		Delay_ms(500);
//
//	}


	SPI_SendData(0x23, 0xc9);                         // resetting the accelerometer internal circuit
	SPI_SendData(0x20, 0x67);                         // 100Hz data update rate, block data update disable, x/y/z enabled
	SPI_SendData(0x24, 0x20);                         // Anti aliasing filter bandwidth 800Hz, 16G (very sensitive), no self-test, 4-wire interface
	SPI_SendData(0x10, 0x00);                         // Output(X) = Measurement(X) - OFFSET(X) * 32;
	SPI_SendData(0x11, 0x00);                         // Output(Y) = Measurement(Y) - OFFSET(Y) * 32;
	SPI_SendData(0x12, 0x00);                         // Output(Z) = Measurement(Z) - OFFSET(Z) * 32;

// ************************

	while(1){
		SET_BIT(led_flags,0);



		adc_val=GetVoltage();

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

		// lis *******************
/*
		_puts("Motor power: ");
		itoa(TIM1->CCR1,str_buf,10);
		_puts(str_buf);
		_putsn("%");

		_puts("Battery voltage: ");
		itoa(adc_val,str_buf,10);
		_puts(str_buf);
		_putsn(" mV ");
*/

//		for(uint8_t i = 0; i < 100; i++) {             // getting 100 samples
//
//			MSB = SPI_ReceiveData(0x29);              // X-axis MSB
//			LSB = SPI_ReceiveData(0x28);              // X-axis LSB
//			Xg = (MSB << 8) | (LSB);                  // Merging
//			x_array[i] = Xg;
//
//			MSB = SPI_ReceiveData(0x2d);              // Z-axis MSB
//			LSB = SPI_ReceiveData(0x2c);              // Z-axis LSB
//			Zg = (MSB << 8) | (LSB);                  // Merging
//			z_array[i] = Zg;
//		}
//
//
//	    Sort_Signed(x_array, 100);                  // Sorting min to max
//	    Sort_Signed(z_array, 100);                  // Sorting min to max
//
//	    x_average /= 80;                            // dividing by the number of samples used
//	    x_average /= -141;                          // converting to meters per second squared
//
//	    z_average /= 80;                            // dividing by the number of samples used
//	    z_average /= -141;                          // converting to meters per second squared
//
//	    zx_theta = gToDegrees(z_average, x_average);                // getting the degrees between Z and X planes
//
//	    _puts("X: ");
//	    itoa(x_average,str_buf,10);
//	    _putsn(str_buf);
//
//	    _puts("Z: ");
//	    itoa(z_average,str_buf,10);
//		_putsn(str_buf);
//
//		_puts("XZt: ");
//	   	 itoa(zx_theta,str_buf,10);
//		_putsn(str_buf);
//
//		_puts("\x1b[3A");

		// ***********************
	}


	return 0;
}

// lis ******************
void Sort_Signed(int16_t A[], uint8_t L) {
  uint8_t i = 0;
  uint8_t status = 1;

  while(status == 1){
    status = 0;
    for(i = 0; i < L-1; i++){
      if (A[i] > A[i+1]){
        A[i]^=A[i+1];
        A[i+1]^=A[i];
        A[i]^=A[i+1];
        status = 1;
      }
    }
  }
}


float gToDegrees(float V, float H)               // refer to the orientation pic above
{
  float retval;
  uint16_t orientation;

  if (H == 0) H = 0.001;                         // preventing division by zero
  if (V == 0) V = 0.001;                         // preventing division by zero

  if ((H > 0) && (V > 0)) orientation = 0;
  if ((H < 0) && (V > 0)) orientation = 90;
  if ((H < 0) && (V < 0)) orientation = 180;
  if ((H > 0) && (V < 0)) orientation = 270;

  retval = ((atan(V/H)/3.14159)*180);
  if (retval < 0) retval += 90;
  retval = abs(retval) + orientation;
  return retval;
}

// **********************

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



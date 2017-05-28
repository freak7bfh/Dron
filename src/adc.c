/*
 * adc.c
 *
 *  Created on: May 24, 2017
 *      Author: Jarek
 */

#include "stm32f0xx.h"
#include "main.h"

void ADCInit(){

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure_forADC;
	GPIO_InitStructure_forADC.GPIO_Pin   	= GPIO_Pin_2;
	GPIO_InitStructure_forADC.GPIO_Mode  	= GPIO_Mode_AN;
	GPIO_InitStructure_forADC.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_InitStructure_forADC.GPIO_Speed 	= GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure_forADC);


	ADC_InitTypeDef ADC_InitStruct;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);


	/* Configure the ADC1 in continous mode withe a resolutuion equal to 10 bits  */
	ADC_InitStruct.ADC_Resolution 			= ADC_Resolution_10b;
	ADC_InitStruct.ADC_ContinuousConvMode 	= ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_ExternalTrigConv 	= ADC_ExternalTrigConv_T1_TRGO;
	ADC_InitStruct.ADC_DataAlign 			= ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ScanDirection 		= ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStruct);

	/* Convert the ADC_Channel_2 sensor  with 55.5 Cycles as sampling time */
	ADC_ChannelConfig(ADC1,ADC_Channel_2,ADC_SampleTime_239_5Cycles);

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait the ADCEN falg */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

	/* ADC1 regular Software Start Conv */
	ADC_StartOfConversion(ADC1);

}

uint16_t GetVoltage(){
	return ADC1->DR*6.475;
}

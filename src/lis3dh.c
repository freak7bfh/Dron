#include "stm32f0xx.h"
#include "lis3dh.h"
#include "usart.h"

void Lis3dhInit(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef SPI_CS_GPIO_InitStruct;
	SPI_CS_GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_12 | GPIO_Pin_15;
	SPI_CS_GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_OUT;
	SPI_CS_GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_10MHz;
	SPI_CS_GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
	SPI_CS_GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;

	GPIO_Init(GPIOA,&SPI_CS_GPIO_InitStruct);

	GPIOA->BSRR=GPIO_Pin_12;	// gyro
	GPIOA->BSRR=GPIO_Pin_15;	// acce



	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitTypeDef SPI_GPIO_InitStruct;
	SPI_GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	SPI_GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF;
	SPI_GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_10MHz;
	SPI_GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
	SPI_GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_0);

	GPIO_Init(GPIOB, &SPI_GPIO_InitStruct);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_InitTypeDef SPI1_InitStruct;

	SPI1_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI1_InitStruct.SPI_Mode              = SPI_Mode_Master;
	SPI1_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
	SPI1_InitStruct.SPI_CPOL              = SPI_CPOL_Low;
	SPI1_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;
//	SPI1_InitStruct.SPI_NSS               = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI1_InitStruct.SPI_NSS               = SPI_NSS_Soft;
	SPI1_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI1_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI1_InitStruct.SPI_CRCPolynomial	  = 7;

	SPI_Init(SPI1, &SPI1_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}



uint8_t WriteSPI(uint8_t cbData){
    uint8_t cbResult = 0x00;

    while(!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = cbData;
	while(!(SPI1->SR & SPI_SR_RXNE));
	cbResult = SPI1->DR;

    return cbResult;
}


uint8_t SetReg(uint8_t cbAddress, uint8_t cbData){


	uint8_t cbResult = 0x00;
	ACCE_ENABLE;

	WriteSPI(cbAddress);
	cbResult = WriteSPI(cbData);


	ACCE_DISABLE;

	return cbResult;

}

uint8_t GetReg(uint8_t cbAddress){


    ACCE_ENABLE;
    uint8_t cbResult = 0x00;


	WriteSPI(cbAddress|0x80);
	cbResult = WriteSPI(0x00);

	ACCE_DISABLE;
	return cbResult;
}




//uint8_t ReadReg(uint8_t Reg){
//	uint16_t Dane = 0x0080;
//	uint8_t Data=0;
//
//	Dane |= Reg;
//	Dane <<=8;
//
//	ACCE_ENABLE;
//	while((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET);
//
////	SPI_I2S_SendData(ACC_SPI, Dane);
//	SPI1->DR = Dane;
//
//
////	while(SPI_I2S_GetFlagStatus(ACC_SPI,SPI_I2S_FLAG_RXNE) == RESET);
//	while((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET);
//
//	Data = (uint8_t) (SPI1->DR);
//
//	ACCE_DISABLE;
//
//	return (Data);
//}
//
//void WriteReg(uint8_t Reg, uint8_t Data){
//	uint16_t Dane = 0;
//
//	Dane = Reg;
//	Dane <<=8;
//	Dane |= Data;
//
//	ACCE_ENABLE;
//	while((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET);
//	SPI1->DR = Dane;
//	while((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET);
//	//odczytuje dane aby skasowac flage
//	Dane = SPI1->DR;
//	ACCE_DISABLE;
//
//}
//
//void SPI_SendData(uint8_t address, uint8_t data){
//	ACCE_ENABLE;
//
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	SPI_I2S_SendData16(SPI1, address);
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//	SPI_I2S_ReceiveData16(SPI1);
//
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	SPI_I2S_SendData16(SPI1, data);
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//	SPI_I2S_ReceiveData16(SPI1);
//	ACCE_DISABLE;
//}
//
//uint8_t SPI_ReceiveData(uint8_t address){
//
//	ACCE_ENABLE;
//
//	address = 0x80 | address;                         // 0b10 - reading and clearing status
//	//address <<= 8;
//
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	SPI_I2S_SendData16(SPI1, address);
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//	SPI_I2S_ReceiveData16(SPI1);
//
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	SPI_I2S_SendData16(SPI1, 0x00);
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//
//	ACCE_DISABLE;
//	return SPI_I2S_ReceiveData16(SPI1);
//}





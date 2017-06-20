#include "stm32f0xx.h"

void Lis3dhInit(){
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
	SPI1_InitStruct.SPI_CPOL              = SPI_CPOL_High;
	SPI1_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI1_InitStruct.SPI_NSS               = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI1_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI1_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;

	SPI_Init(SPI1, &SPI1_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}


void SPI_SendData(uint8_t address, uint8_t data){
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData16(SPI1, address);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData16(SPI1);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData16(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData16(SPI1);
}

uint8_t SPI_ReceiveData(uint8_t address){
	address = 0x80 | address;                         // 0b10 - reading and clearing status

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData16(SPI1, address);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData16(SPI1);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData16(SPI1, 0x00);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	return SPI_I2S_ReceiveData16(SPI1);
}





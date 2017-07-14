/*
 * lis3dh.h
 *
 *  Created on: May 25, 2017
 *      Author: Jarek
 */

#ifndef LIS3DH_H_
#define LIS3DH_H_


#define ACCE_ENABLE GPIOA->BRR=GPIO_Pin_15;
#define ACCE_DISABLE GPIOA->BSRR=GPIO_Pin_15;
#define GYRO_ENABLE GPIOA->BRR=GPIO_Pin_12;
#define GYRO_DISABLE GPIOA->BSRR=GPIO_Pin_12;

void Lis3dhInit();

uint8_t SetReg(uint8_t cbAddress, uint8_t cbData);
uint8_t GetReg(uint8_t cbAddress);
uint8_t WriteSPI(uint8_t cbData);

//void SPI_SendData(uint8_t address, uint8_t data);
//uint8_t SPI_ReceiveData(uint8_t address);
//void SPI_SendFrame(uint8_t *data, uint8_t frame_len);

//uint8_t ReadReg(uint8_t Reg);
//void WriteReg(uint8_t Reg, uint8_t Data);

#endif /* LIS3DH_H_ */

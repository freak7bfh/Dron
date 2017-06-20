/*
 * lis3dh.h
 *
 *  Created on: May 25, 2017
 *      Author: Jarek
 */

#ifndef LIS3DH_H_
#define LIS3DH_H_

void Lis3dhInit();
void SPI_SendData(uint8_t address, uint8_t data);
uint8_t SPI_ReceiveData(uint8_t address);
//void SPI_SendFrame(uint8_t *data, uint8_t frame_len);

#endif /* LIS3DH_H_ */

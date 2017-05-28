/*
 * usart.h
 *
 *  Created on: 23-02-2015
 *      Author: Jarek
 */

#ifndef USART_H_
#define USART_H_

#define RS_BUFF 8

char buffer[64];	// string buffer
#define _printf(...) sprintf(buffer, __VA_ARGS__); USARTSendNow(buffer);
#define _printfn(...) sprintf(buffer, __VA_ARGS__); sprintf(buffer + strlen(buffer), "\r\n"); USARTSendNow(buffer);

#define _puts(...) USARTSendNow(__VA_ARGS__);
#define _putsn(...) USARTSendNow(__VA_ARGS__); USARTSendNow("\r\n");


void USARTInit();
void USARTSendNow(char *s);
void USARTSendFrame(uint8_t *frame, uint16_t frame_len);

#endif /* USART_H_ */

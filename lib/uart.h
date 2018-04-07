/*
 * uart.h
 *
 *  Created on: Jun 3, 2016
 *      Author: Administrator
 */


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include "global.h"
#include <stdio.h>

#define BAUD 	9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

void uartInit();
void sendByte(unsigned char c);
static FILE uart_ = FDEV_SETUP_STREAM(sendByte, NULL, _FDEV_SETUP_WRITE);

#endif /* UART_H_ */

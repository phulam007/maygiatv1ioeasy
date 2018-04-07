/*
 * board.h
 *
 *  Created on: Mar 3, 2016
 *      Author: Administrator
 *
 *      KHAI BÁO CHÂN CẲNG CHO BOARD MÁY GIẶT 8 OUTPUT
 *
 */

#ifndef BOARD_H_
#define BOARD_H_
#include <avr/io.h>
#include "global.h"

// output relay
#define	RELAY1	PD5
#define RELAY2 	PD6
#define RELAY3	PB0
#define RELAY4	PB1
#define RELAY5	PD4
#define RELAY6	PD7
#define	RELAY7	PD3
#define RELAY8	PD2
#define LED	PB4
// port relay
#define pRELAY1	PORTD
#define pRELAY2	PORTD
#define pRELAY3	PORTB
#define pRELAY4	PORTB
#define pRELAY5	PORTD
#define pRELAY6	PORTD
#define pRELAY8	PORTD
#define pRELAY7	PORTD
#define pLED	PORTB
// DDR RELAY
#define dRELAY1	DDRD
#define dRELAY2	DDRD
#define dRELAY3	DDRB
#define dRELAY4	DDRB
#define dRELAY5	DDRD
#define dRELAY6	DDRD
#define dRELAY7	DDRD
#define dRELAY8	DDRD
#define dLED	DDRB
// DIR 75176
#define DIR PB2
#define pDIR PORTB
#define dDIR DDRB

// INPUT
#define BUT_SCAN	PB5
#define pBUT_SCAN	PORTB
#define dBUT_SCAN	DDRB
#define	BUT_SCAN_ST	(inb(PINB) & (1<<BUT_SCAN))
// ON OFF RELAY
#define RELAY1_ON	sbi(pRELAY1, RELAY1)
#define RELAY2_ON	sbi(pRELAY2, RELAY2)
#define RELAY3_ON	sbi(pRELAY3, RELAY3)
#define RELAY4_ON	sbi(pRELAY4, RELAY4)
#define RELAY5_ON	sbi(pRELAY5, RELAY5)
#define RELAY6_ON	sbi(pRELAY6, RELAY6)
#define RELAY7_ON	sbi(pRELAY7, RELAY7)
#define RELAY8_ON	sbi(pRELAY8, RELAY8)
#define LED_OFF		cbi(pLED,LED)

#define RELAY1_OFF	cbi(pRELAY1, RELAY1)
#define RELAY2_OFF	cbi(pRELAY2, RELAY2)
#define RELAY3_OFF	cbi(pRELAY3, RELAY3)
#define RELAY4_OFF	cbi(pRELAY4, RELAY4)
#define RELAY5_OFF	cbi(pRELAY5, RELAY5)
#define RELAY6_OFF	cbi(pRELAY6, RELAY6)
#define RELAY7_OFF	cbi(pRELAY7, RELAY7)
#define RELAY8_OFF	cbi(pRELAY8, RELAY8)
#define LED_ON		sbi(pLED,LED)

#define RELAY1_REV	outb(pRELAY1,pRELAY1^(1<<RELAY1))
#define RELAY2_REV	outb(pRELAY2,pRELAY2^(1<<RELAY2))
#define RELAY3_REV	outb(pRELAY3,pRELAY3^(1<<RELAY3))
#define RELAY4_REV	outb(pRELAY4,pRELAY4^(1<<RELAY4))
#define RELAY5_REV	outb(pRELAY5,pRELAY5^(1<<RELAY5))
#define RELAY6_REV	outb(pRELAY6,pRELAY6^(1<<RELAY6))
#define RELAY7_REV	outb(pRELAY7,pRELAY7^(1<<RELAY7))
#define RELAY8_REV	outb(pRELAY8,pRELAY8^(1<<RELAY8))
#define LED_REV		outb(pLED,pLED^(1<<LED))

#define RELAY1_ST	(pRELAY1 & (1<<RELAY1))
#define RELAY2_ST	(pRELAY2 & (1<<RELAY2))
#define RELAY3_ST	(pRELAY3 & (1<<RELAY3))
#define RELAY4_ST	(pRELAY4 & (1<<RELAY4))
#define RELAY5_ST	(pRELAY5 & (1<<RELAY5))
#define RELAY6_ST	(pRELAY6 & (1<<RELAY6))
#define RELAY7_ST	(pRELAY7 & (1<<RELAY7))
#define RELAY8_ST	(pRELAY8 & (1<<RELAY8))

#define REV		cbi(pDIR, DIR)
#define SEND		sbi(pDIR, DIR)
#define _ON		"T"
#define _OFF		"F"
#define _REV		"R"


struct device_S
{
	u08 ip;
	unsigned char Mac[20];
	u08 state;
	u08 type;
	u08 lenMac;
	u08 sta[5];
};

void gpioInit()	;
void relayControl_ON(u08 relay);
void relayControl_OFF(u08 relay);

#endif /* BOARD_H_ */

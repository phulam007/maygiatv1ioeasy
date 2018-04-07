/*
 * main.c
 *
 *  Created on: Feb 16, 2016
 *      Author: Administrator
 *      MAY GIAT - ATMEGA8A --- 11059200hz--- dieu khien 8 relay
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "board.h"
#include "pt.h"
#include "uart.h"
#include "ioeasy.h"


volatile struct mainControl_S mG;
char str[2] = {'V','S'};
//
static struct pt resetMacAdd;
static struct pt blinkLed;
static struct pt dataProcess;
//
u16 time_tick = 0;
u08 timeout = 0;
u16 time;
u08 fTimeout = 0;
u08 ledBlink = 0;
//
static PT_THREAD(resetMacDev(struct pt* pt))
	    {
		static u08 staBut;
		static u16 ct;
		PT_BEGIN(pt);
		while(1){
		    if(BUT_SCAN_ST == 0)
			{
			    switch(staBut)
				{
				    case 0 : if(mG.enableSearch)
					    {
					    LED_OFF;
					    //mG.enableSearch = 0;
					    }
					    else
					    {
					    LED_ON;
					   // mG.enableSearch = 1;
					    }
					    staBut = 1;
					    pt_delay(500);
					    break;
				   case 1 : ct++;
					   pt_delay(1);
					    if(ct > RESET_TIME)
						{
						    resetMac(&mG);
						    ct = 0 ;
						    ledBlink = 1;
						    staBut = 2;
						}
					    break;
				    case 2: break;
				}
			}
		    else { staBut = 0; ct = 0; }
		    PT_YIELD(pt);
		}
		PT_END(pt);
	    }
static PT_THREAD(blinkLed_(struct pt* pt))
	    {
		PT_BEGIN(pt);
		    while(1)
			{
			PT_WAIT_UNTIL(pt, (ledBlink == 1));
			    LED_ON;
			    pt_delay(800);
			    LED_OFF;
			    pt_delay(800);
			    LED_ON;
			    pt_delay(800);
			    LED_OFF;
			    pt_delay(800);
			    LED_ON;
			    pt_delay(800);
			    LED_OFF;
			    ledBlink = 0;
			 PT_YIELD(pt);
			}
		PT_END(pt);
	    }
static PT_THREAD(dataProcess_(struct pt* pt))
	    {
		PT_BEGIN(pt);
		while(1)
		    {
		    if(mG.nextStt == END)
		    		{
		    			cmdProcess(&mG);
		    			_delay_ms(2);
		    			REV;
		    			mG.nextStt = SYN;
		    			mG.cmd = NONE;
		    		}
		    if(fTimeout)
		    		{
					_delay_ms(100);
					REV;
		    			mG.nextStt = SYN;
		    			mG.cmd = NONE;
		    			mG.curTrans = 0xff; // lưu ý
		    			timeout = 0;
		    			fTimeout = 0;
		    			time = 0;
		    		}
		    PT_YIELD(pt);
		    }
		PT_END(pt);
	    }
//
int main()
{
    PT_INIT(&resetMacAdd);
    PT_INIT(&blinkLed);
    PT_INIT(&dataProcess);
    _delay_ms(500);
	getInfo(&mG);	// khai bao eeprom
	gpioInit();		// khai bao I/O
	uartInit();		// uart
	deviceInit(&mG);
	TCNT0 = 213;
	TCCR0B |= (1<<CS02)  ; // 1/256
	TIMSK0 |= (1 << TOIE0);
	sei();
	mG.enableSearch = 1;
	_delay_ms(10);
	REV;
	_delay_ms(10);
	while(1)
	{
	    resetMacDev(&resetMacAdd);
	    blinkLed_(&blinkLed);
	    dataProcess_(&dataProcess);
	}
return 0;
}

// CHƯƠNG TRÌNH NGẮT UART XU LY DU LIEU GUI DEN THUC THI CMD
ISR(USART_RX_vect)
{
	unsigned char c;
	c = UDR0;
	checkSyn(c,&mG,str);
	if(mG.nextStt != END)
				{
					timeout = 1;
					time = 0;
				}
			else
				{
					timeout = 0;
					time = 0;
				}
}

/////////////////////////////////////////////////////////////////////////////////////
//
//                                       NGAT TIMER0
//	                       Dung de kiem tra timeout du lieu gui toi
////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{
	 time_tick+=1;
	 if(timeout)
		 time++;
	 if(time > 200)
		 {
		 	 fTimeout = 1;
		 	 timeout =0;
		 	 time = 0;
		 }
	 outb(TCNT0, 213);
}


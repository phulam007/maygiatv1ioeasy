/*
 * board.c
 *
 *  Created on: Mar 3, 2016
 *      Author: Administrator
 *      Thiết lập thông số cho các ngõ vào ra
 *
 */

#include "board.h"
#include "ioeasy.h"
#include <avr/delay.h>
#include <inttypes.h>
#include <string.h>
#include "uart.h"

void gpioInit()
{
    //SET INPUT
	cbi(dBUT_SCAN,BUT_SCAN);
	sbi(pBUT_SCAN,BUT_SCAN);	// tro keo len

	// set output relay
	sbi(dRELAY1, RELAY1);		// RELAY1
	sbi(dRELAY2, RELAY2);		// RELAY2
	sbi(dRELAY3, RELAY3);		// RELAY3
	sbi(dRELAY4, RELAY4);		// RELAY4
	sbi(dRELAY5, RELAY5);		// RELAY5
	sbi(dRELAY6, RELAY6);		// RELAY6
	sbi(dRELAY7, RELAY7);		// RELAY7
	sbi(dRELAY8, RELAY8);		// RELAY8
	// set output dir 75176
	sbi(dDIR, DIR);		// RELAY1
	// LED
	sbi(dLED, LED);		// RELAY1

	LED_OFF;
	// OFF RELAY
	RELAY1_OFF;
	RELAY2_OFF;
	RELAY3_OFF;
	RELAY4_OFF;
	RELAY5_OFF;
	RELAY6_OFF;
	RELAY7_OFF;
	RELAY8_OFF;
}


void deviceInit(struct mainControl_S *a)
{
    u08 i;
    for(i=0;i<NUM_DEV;i++)
	a->device[i].state = 0;
}
// xoa thiet bi
// callback function infomation form device
void cbInfoDev(struct mainControl_S *a)
{
    if(a->device[a->dev].state)
	{out485("VSr%c%c%c",a->trans,1,1);}
    else
	{out485("VSr%c%c%c",a->trans,1,0);}
}
// RESET THIET BI
int cbResetDev(struct mainControl_S *a)
{

	return 1;
}
// SET DIA CHI IP
int cbSetIpDev(struct mainControl_S *a)
{

	return 1;
}
// XOA THIET BI
int cbDelDev(struct mainControl_S *a)
{

	return 1;
}
// CHUONG TRINH DIEU KHIEN
int cbControl(struct mainControl_S *a)
{
    u08 tmp[8];
    u08 vol;
    	if(a->dev != 0xff)
    	{	// bat thiet bi
    		if((strncmp(a->data,_ON,strlen(_ON)) == 0)&&(strlen(a->data)==1))
    				{
    					//if(a->curTrans != a->trans)
    					{
    					    relayControl_ON(a->dev);
    					    out485("VSr%c%c%c",a->trans,1,1);
    					    a->device[a->dev].state = 1;
    					}
    					//else
    					//    out485("VSr%c%c%c",a->trans,1,a->device[a->dev].state);
    					return 1;
    				}
    		// tat thiet bi
    		if((strncmp(a->data,_OFF,strlen(_OFF)) == 0)&&(strlen(a->data)==1))
    				{
    					//if(a->curTrans != a->trans)
    		    			{
    					    relayControl_OFF(a->dev);
    					    out485("VSr%c%c%c",a->trans,1,0);
    					    a->device[a->dev].state = 0;
    		    			}
    					//else
    					//out485("VSr%c%c%c",a->trans,1,a->device[a->dev].state);
    					return 1;
    				}
    		// dao thiet bi
    		if((strncmp(a->data,_REV,strlen(_REV)) == 0)&&(strlen(a->data)==1))
    				{
    				    //if(a->curTrans != a->trans)
    		    		    //{
    				      if(a->device[a->dev].state)
    					{
    					    relayControl_OFF(a->dev);
    					    out485("VSr%c%c%c",a->trans,1,0);
    					    a->device[a->dev].state = 0;
    					    return 1;
    					}
    				     else
    					{
    					    relayControl_ON(a->dev);
    					    out485("VSr%c%c%c",a->trans,1,1);
    					    a->device[a->dev].state = 1;
    					    return 1;
    					}
    				     // a->curTrans = a->trans;

    		    		    //}
    				    //else
    					//out485("VSr%c%c%c",a->trans,1,a->device[a->dev].state);
    				    return 1;
    				}
    	}
	return 0;
}

void relayControl_ON(u08 relay)
{
    switch(relay)
	{
	    case 0: RELAY1_ON; break;
	    case 1: RELAY2_ON; break;
	    case 2: RELAY3_ON; break;
	    case 3: RELAY4_ON; break;
	    case 4: RELAY5_ON; break;
	    case 5: RELAY6_ON; break;
	    case 6: RELAY7_ON; break;
	    case 7: RELAY8_ON; break;
	}
}


void relayControl_OFF(u08 relay)
{
    switch(relay)
	{
	    case 0: RELAY1_OFF; break;
	    case 1: RELAY2_OFF; break;
	    case 2: RELAY3_OFF; break;
	    case 3: RELAY4_OFF; break;
	    case 4: RELAY5_OFF; break;
	    case 5: RELAY6_OFF; break;
	    case 6: RELAY7_OFF; break;
	    case 7: RELAY8_OFF; break;
	}
}

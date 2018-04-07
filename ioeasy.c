/*
 * ioeasy.c
 *
 *  Created on: Mar 3, 2016
 *      Author: Administrator
 */

#include "ioeasy.h"
#include <avr/delay.h>
#include <inttypes.h>
#include <string.h>
#include "uart.h"
//#include "fstring.h"
#define EEPSAVE 0
const unsigned char macDefault[NUM_DEV][20] = {"mR4700svc01","mR4700svc02","mR4700svc03","mR4700svc04","mR4700svc05","mR4700svc06","mR4700svc07","mR4700svc08"};
const unsigned char deviceType[NUM_DEV]     = {SET_OUTPUT,SET_OUTPUT,SET_OUTPUT,SET_OUTPUT,SET_OUTPUT,SET_OUTPUT,SET_OUTPUT,SET_OUTPUT};
const unsigned char ipDefault[NUM_DEV]	    = {8,9,10,11,12,13,14,15};
/***********************************************************
 *
 * XOA MA MAC THIET BI
 *
 ***********************************************************/
void resetMac(struct mainControl_S* a)
    {
	u08 tmp;
	for(tmp = 0; tmp < NUM_DEV; tmp++)
	    {
		a->device[tmp].ip = 0xff;
		eeprom_write_byte((u08*)MYIP+tmp,0xff);
	    }
	a->nextStt = SYN;
	a->cmd = NONE;
    }
/************************************************************
 * 			KHAI BAO MAC DINH
 */
void getInfo(struct mainControl_S* a)
    {
	u08 i, tmp;
	for(i=0;i < NUM_DEV;i++)
	{
	    a->device[i].type = deviceType[i];
	    a->device[i].state = 0;
	    if(EEPSAVE)
		a->device[i].ip = eeprom_read_byte((u08*)MYIP+i);
	    else
		a->device[i].ip = ipDefault[i];
	    a->device[i].lenMac = strlen(macDefault[i]);
	    for(tmp = 0; tmp < a->device[i].lenMac; tmp++)
		a->device[i].Mac[tmp] = macDefault[i][tmp];
	}
	a->nextStt = SYN;
	a->cmd = NONE;
    }


/************************************************************
 * 			HAM NGAT NHAN TIN HIEU TU BO IO EASY
 *
 ***********************************************************/
void checkSyn(char c,  struct mainControl_S* a, char cmd[])
{
	static u08 ct, i;
	u08 j;
	static u08 _tmp[NUM_DEV];
	switch(a->nextStt)
	{
	case SYN: // Dong bo ki tu dau
				if(c == cmd[ct]) ct++;			 // kiem tra 2 ki tu dau co phai la VS kg?
					else ct = 0;
				if(ct == 2)
					{
						a->nextStt = GET_CMD;
						i=0;
					}	// neu du 2 ki tu dau -> lay lenh dieu khien tiep theo M I O a
				break;
	case GET_CMD:
				ct = 0;
				switch(c)
				  {
					case 'M':  a->nextStt = GET_TRANS;		// lay ma transsaction
						   a->cmd = SET_MAC;			// che do set mac thiet bi->
						   break;
					case 'O': a->nextStt = GET_TRANS;
						  a->cmd = SET_OUTPUT;		// set output
						   break;
				  	default:  a->nextStt = SYN;
				  	  	  a->cmd = NONE;
				  	  	  	  break;
				  }
				  break;
	case GET_TRANS : // lay transsaction
					a->nextStt = GET_LEN1;
					a->trans = c;
					break;
	case GET_LEN1:  // lay chieu dai mac host.. kt khi da do tb
				  a->lenMac = c;
				  if(a->lenMac >= HOST_LEN_MIN)
					  {
					  	  a->nextStt = GET_MAC;
					  	  i = 0;
					  }
				  else
					  a->nextStt = SYN;
				  break;
	case GET_MAC:	// lay ve mac MAC cua bo IO
			 //if(a->lenMac-- > 0)
			  //   i++ ;
			 a->lenMac--;
			 if(a->lenMac == 0)
			     {
				if(a->cmd == SET_MAC)
				    a->nextStt = GET_LEN3;
				else
				    a->nextStt = GET_IP;
			     }
				break;
	case GET_LEN3 :		// lay chieu dai ma MAC cua IO gui den tb
					a->lenMac1 = c;
					if(a->lenMac1 >= HOST_LEN_MIN)
						a->nextStt = GET_MAC1;
					else
						a->nextStt = SYN;
					break;
	case GET_MAC1 : 	// lay ma MAC tu IO gui den tb... neu
				if(a->lenMac1-- > 0)
					{
						for(j = 0; j < NUM_DEV; j++)
						_tmp[j]+=(a->device[j].Mac[i] == c);
							i++;
					}
				if(a->lenMac1 == 0)
					{
						a->dev = 0xff;
						for(j = 0; j< NUM_DEV; j++)
						    if(_tmp[j]==a->device[j].lenMac)
							a->dev = j;
						if(a->dev != 0xff)
							{
								a->nextStt = GET_LEN2;
								a->lenMac1 = i;
								i=0;
								memset(_tmp,0,NUM_DEV);
							}
						else
							{
								a->nextStt = SYN;
								a->cmd = NONE;
							}
						}
					break;
	case GET_IP :		// lay dia chi IP cua tb
				a->dev = 0xff;
				for(j = 0; j < NUM_DEV; j++)
					{
						if((a->device[j].ip == c)&&(a->device[j].type==a->cmd))
							{
								a->dev = j;
								a->nextStt = GET_LEN2;
								break;
							}
					}
				if((a->dev==0xff))
					{
					    a->nextStt = SYN;
					    a->cmd = NONE;
					}
					break;
	case GET_LEN2:		// lay do dai data
				a->lenData = c;
				a->nextStt = GET_DATA;
				break;
	case GET_DATA:		// lay data gui den
				if(a->lenData-- > 0)
				{
					a->data[i] = c;
					i++;
					a->data[i] = 0;
				}
				if(a->lenData == 0)
				{
					a->nextStt = END;
					a->lenData = i;
					i = 0;
				}
				break;
	}
}
int sendInfodevice(struct mainControl_S* a)
{
 u08 __tmp[3];
 if(a->dev != 0xff)
	  	  if((strncmp(a->data,_INFO_DEV,strlen(_INFO_DEV)) == 0)&&(strlen(a->data)==1))
			{
	  		    cbInfoDev(a);
				return 1;
			}
  return 0;
}
int sendMacdevice(struct mainControl_S* a)
{
	if((strncmp(a->data,_ADD_DEV,strlen(_ADD_DEV)) == 0)&&(strlen(a->data)==1))
		{
			out485("VSr%c%c%s",a->trans,strlen(a->device[a->dev].Mac),a->device[a->dev].Mac);
			return 1;
		}
	return 0;
}
int setIPdevice(struct mainControl_S* a)
{
	u08 i,j;
	if((strncmp(a->data,_SET_IP,strlen(_SET_IP)) == 0)&&(strlen(a->data)>3)&&(strlen(a->data)<=6))
		{
		j = strlen(a->data)-strlen(_SET_IP);
			if(a->cmd == SET_MAC)
			{
						if(j==1) { i = a->data[3]-'0';}
						if(j==2) { i = (a->data[3]-'0')*10 + (a->data[4] - '0');}
						if(j==3) { i = (a->data[3]-'0')*100 + (a->data[4]-'0')*10 + (a->data[5]-'0');}
						a->device[a->dev].ip = i;
						eeprom_write_byte((u08)MYIP+a->dev,a->device[a->dev].ip);
						out485("VSa%c",a->trans);
						return 1;
			}
		}
	return 0;
}
int deleteIpdevice(struct mainControl_S* a)
{
	u08 i;
	if(a->dev != 0xff)
	{
		if((strncmp(a->data,_DELETE_DEV,strlen(_DELETE_DEV)) == 0)&&(strlen(a->data)==1))
		{
			 a->device[a->dev].ip = 0xff;
			 eeprom_write_byte((u08)MYIP+a->dev,0xFF);
			 if(a->device[a->dev].state)
			 //eeprom_write_byte((u08)_VOL_EEP+a->dev,100);
			 cbDelDev(a);
			 out485("VSa%c",a->trans);					// RETURN ACK -> OK
			 for(i=0;i<NUM_DEV;i++)
				 if(a->device[i].ip == 0xff)
					 continue;
				 else
			 	 	 return 1;
			 resetMac(a);
		}
	}
	return 0;
}

/*************************************************************
 * 			HAM XU LY DU LIEU GUI LEN
 * 			- xu ly cac lenh T (ON), F(OFF), R(DAO), D(delete)
 * 			- IP= (gan IP), MAC=(gan ma mac)
 *
 *************************************************************/
int cmdProcess(struct mainControl_S* a)
{
	// tim thiet bi
	if(sendMacdevice(a)) return 0;
	// gan dia chi ip
	if(setIPdevice(a)) return 0;
	// xoa thiet bi
	if(deleteIpdevice(a)) return 0;
	// gui thong tin thiet bi
	if(sendInfodevice(a)) return 0;
	// dieu khien thiet bi
	if(cbControl(a)) return 0;
	return 0;
}

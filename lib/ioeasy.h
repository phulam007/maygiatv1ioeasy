/*
 * ioeasy.h
 *
 *  Created on: Mar 3, 2016
 *      Author: Administrator
 */

#ifndef IOEASY_H_
#define IOEASY_H_
#include <avr/io.h>
#include <avr/eeprom.h>
#include "avrlibdefs.h"
#include "avrlibtypes.h"
#include "board.h"
// DEFINE
#define MAC 	'M'
#define OUTPUT 	'O'
#define INPUT 	'I'
#define ACK 	'a'
#define NACK 	'n'
#define _DELETE_DEV	"D"
#define _INFO_DEV	"s"
#define _ADD_DEV		"m"
#define _SET_MAC		"MAC="
#define _SET_IP			"IP="


#define NUM_DEV	8
#define HOST_LEN_MIN	7
#define RESET_TIME	60000
#define TIMEOUT_MS	400

// EEPROM
#define EEPROM_WRITE	0	// ghi vao eeprom lan dau
#define HAVE_A_HOST	1	// TRANG THAI TB CO DC DO` HAY CHUA?
#define MAC_NUM		2	// SO RELAY DA TIM MA MAC
#define SAVE_STATUS 4	// KT XEM CO LUU TRANG THAI RELAY KG?
#define HOST_LEN 	5
#define TIMEOUT_	6
#define MYIP		20
#define MAC_LEN		30
#define STATUS_		10	// TRANG THAI RELAY
#define MYMAC		100
#define MYHOST		40
#define MAC_TYPE	60
#define out485(...)		SEND;\
						_delay_ms(15);\
						fprintf(&uart_,__VA_ARGS__);\
						_delay_ms(15);\
						REV;\

enum state_S{
				SYN, 	   // CHO DONG BO DU LIEU
				GET_CMD,   // LAY LENH GUI XUONG
				GET_TRANS, // LAY MA TRANSSACTION
				GET_LEN1,  // LAY DO DAI MAC
				GET_MAC,   // LAY MA MAC
				GET_MAC1,  // LAY MA MAC CUA TB
				GET_IP,    // LAY IP CUA TB
				GET_LEN2,  // LAY DO DAI DU LIEU
				GET_LEN3,  // LAY DO DAI MA MAC TB
				GET_DATA,  // LAY DU LIEU
				END,	   // KET THUC QUA TRINH NHAN DU LIEU --> XU LY
				TIMEOUT
			};
enum cmd_S{
			NONE,			// KG CO LENH NAO
			SET_MAC, 		// SET DIA CHI MAC CHO TB
			SET_STT, 		// SET TRANG THAI
			SET_OUTPUT,		// OUTPUT
			SET_INPUT,		// INPUT
			GET_INFO,		// INFO
			GET_ACK			// GUI TRA ACK
			};
enum out_S{
			OFF,
			ON,
};

struct host_S
{
	u08 hostLen;
	u08 hostMac[20];
};



struct mainControl_S
{
	enum cmd_S cmd;                 // lenh gui den tu bo IO
	u08 haveaHost;					// trang thai tim tb
	u08 lenMac;						// chieu dai ma Mac
	u08 lenMac1;
	u08 lenData;					// chieu dai du lieu
	u08 macNum;						// trang thai tb
	u08 dev;
	enum state_S waitStt;			// chá»� tráº¡ng thÃ¡i
	u08 enableSearch;
	enum state_S nextStt;			// trang thai cho lenh tiep theo cua thiet bi
	unsigned char trans; 			// ma transsaction
	unsigned char curTrans;
	unsigned char data[100];		// du lieu gui den
	struct device_S device[NUM_DEV];
	struct host_S	host;
};
void cbInfo(struct mainControl_S *a);
void cbDelete(struct mainControl_S *a);
int cbControl(struct mainControl_S *a);

void eepromInit();
void checkSyn(char c, struct mainControl_S* a, char cmd[]);
int cmdProcess(struct mainControl_S* a);
void resetMac(struct mainControl_S* a);


#endif /* IOEASY_H_ */

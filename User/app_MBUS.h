/*
*******************************************************************************
*
*                           the app_MBUS file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_MBUS.h
* Version       : V1.0.0
* Programmer(s) : yu
*******************************************************************************
*/
#include <stdint.h>

#ifndef  APP_MBUS_H_
#define APP_MBUS_H_
typedef struct{
	uint8_t	flowdata[50];
	uint8_t	flowaddr[70];
	uint8_t	flownumber;
	uint8_t	flowbuf;
	uint8_t  flowcnt;
	uint8_t  nummax;
	
}MBUSDEF;

extern  volatile	MBUSDEF	MbusFlow;

#define CONFIGING			((uint8_t)0x03)
#define READING				((uint8_t)0x00)
#define COMPLETE			((uint8_t)0x01)
#define IDLING		     	((uint8_t)0x02)
#define SLEEP				((uint8_t)0x04)
#define RETRANSMISSION		((uint8_t)0x05)

void flow_read(uint8_t flownum);




#endif

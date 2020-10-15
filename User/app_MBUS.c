/*
*******************************************************************************
*
*                           the app_MBUS file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_MBUS.c
* Version       : V1.0.0
* Programmer(s) : yu
*******************************************************************************
*/
#include "includes.h"
#define	TIMEFLOW	200

volatile	MBUSDEF	MbusFlow;

uint8_t 	FlowRec(uint8_t num)
{
	uint8_t	recvbuf[520]={0};
	uint8_t	i, j, n, sum = 0;
		
	comMemCpy(COM3 ,(uint8_t *)recvbuf); 
	for(i = 22; i < 50; i++)
	{
		if((recvbuf[i] == 0x68) && (recvbuf[i + 34] == 0x16))
		{
			for(n = 0; n < 33; n++){
				sum += recvbuf[i + n];
			}
			if(sum == recvbuf[i+33])
			{
				for(j = 0; j < 2; j ++)
				{
					MbusFlow.flowdata[j + num*5] = recvbuf[19 - j + i ];
				}
				for(j = 2; j < 5; j ++)
				{
					MbusFlow.flowdata[j + num*5] = recvbuf[13 + j + i ];
				}
			}
			MbusFlow.flowcnt = 0;
			return 1;
		}
	}
	
	MbusFlow.flowdata[0 + num*5] = 0x65;
	for(j = 1; j < 5; j ++)
	{
		MbusFlow.flowdata[j + num*5] = 0;
	}
	
	return 0;
}

void flow_read(uint8_t  flownum)
{
	USART_InitTypeDef USART_InitStructure;
	uint8_t  i, j, x, cnt, sum;
	uint8_t	num	= flownum;
	uint8_t	sendbuf[22]	= {0xFE,0xFE,0xFE,0xFE,0xFE,0xFE
												,0X68,0X10,0x10,0x01,0x03,0x71,0x01,0x20,0x58,0X01,0x03,0x1F,0X90,0X00,0X58,0X16};
	for( i = 0;  i < 7;  i++)
	{
	 	sendbuf[i + 8] = addrcache.addrFlow[(num + 1)*7 -1 - i];
	}
	for( i = 0; i < 14; i++)
	{
	 	sum += sendbuf[ i +6] ;
	}
	sendbuf[20] = sum;
	
#if DBGMODE
	for(j = 0; j < 22; j ++)
	{
		printf("%X ",sendbuf[j]);
	}
	printf("\r\n");  
#endif
		
	for(cnt = 0;cnt < 3; cnt ++)
	{
		comSendBuf(COM3,(uint8_t *)sendbuf,22);  
		MbusFlow.flowcnt += 1;
		Delay_ms(500);
			
		x = FlowRec(num);
		
		if(x == 1){
			return;
		}
		
		else
		{
			USART_InitStructure.USART_BaudRate = 2500 - (cnt*100);	/* ²¨ÌØÂÊ */
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			USART_InitStructure.USART_StopBits = USART_StopBits_2;
			USART_InitStructure.USART_Parity = USART_Parity_Even ;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART3, &USART_InitStructure);
		}
	}	
	
	MbusFlow.flowdata[0 + num*5] = 0x65;
	for(i = 1; i < 5; i ++)
	{
		MbusFlow.flowdata[i + num*5] = 0;
	}
	
	return ;	
}

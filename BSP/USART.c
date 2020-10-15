/*
*******************************************************************************
*
*                           the app_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : main.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include <stdio.h>
#include <string.h>


#include "USART.h"
#include "delay.h"



//---------------------------------------------------//
uint8_t usart1_rfBuf[USART1_RXMAX_SIZE] = {0};
uint8_t usart1_rBuf[USART1_RXBUF_SIZE] = {0};
__IO uint8_t usart1_rOk = 0;
__IO uint16_t usart1_rNum = 0;

uint8_t usart1_tBuf[USART1_TXBUF_SIZE] = {0};
__IO uint8_t usart1_tBusy = 0;
__IO uint16_t usart1_tNum = 0;
//---------------------------------------------------//

uint8_t usart2_rfBuf[USART2_RXMAX_SIZE] = {0};
uint8_t usart2_rBuf[USART2_RXBUF_SIZE] = {0};
__IO uint8_t usart2_rOk = 0;
__IO uint16_t usart2_rNum = 0;

uint8_t usart2_tBuf[USART2_TXBUF_SIZE] = {0};
__IO uint8_t usart2_tBusy = 0;
__IO uint16_t usart2_tNum = 0;
//---------------------------------------------------//
uint8_t usart3_rfBuf[USART3_RXMAX_SIZE] = {0};
uint8_t usart3_rBuf[USART3_RXBUF_SIZE] = {0};
__IO uint8_t usart3_rOk = 0;
__IO uint16_t usart3_rNum = 0;

uint8_t usart3_tBuf[USART3_TXBUF_SIZE] = {0};
__IO uint8_t usart3_tBusy = 0;
__IO uint16_t usart3_tNum = 0;
//---------------------------------------------------//


// 必须包含<stdio.h>
int fputc(int ch, FILE *f)              
{
    USART_SendData(USART1, (uint8_t) ch);   // uart3发送
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);            /* 等待发送结束 */
        
#if USART3_ENABLE == 1
    USART_SendData(USART3, (uint8_t) ch);   // uart3发送
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);            /* 等待发送结束 */
#endif        
    return ch;
}


//------------------------------------------------------------------------------//
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    //----------------------------------------------------------------//         
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //----------------------------------------------------------------//   
    /* Enable the USART1 Interrupt*/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);                 

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_GetFlagStatus(USART1, USART_FLAG_TC);     
    //----------------------------------------------------------------//                
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_DeInit(DMA1_Channel4);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart1_tBuf;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    DMA_ClearFlag(DMA1_FLAG_GL4);
    DMA_Cmd(DMA1_Channel4, DISABLE);

    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA_DeInit(DMA1_Channel5);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart1_rfBuf;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_BufferSize = USART1_RXMAX_SIZE;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    DMA_ClearFlag(DMA1_FLAG_GL5);
    DMA_Cmd(DMA1_Channel5, ENABLE);


    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    //----------------------------------------------------------------//        
    USART_Cmd(USART1, ENABLE);                        
}



void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);//PA2-TX 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);//PA3-RX

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Enable the USART2 Interrupt*/
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_GetFlagStatus(USART2, USART_FLAG_TC);


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA1_Channel7, DISABLE);
    DMA_DeInit(DMA1_Channel7);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_tBuf;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
    DMA_ClearFlag(DMA1_FLAG_GL7);
    DMA_Cmd(DMA1_Channel7, DISABLE);

    DMA_Cmd(DMA1_Channel6, DISABLE);
    DMA_DeInit(DMA1_Channel6);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_rfBuf;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_BufferSize = USART2_RXMAX_SIZE;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);

    DMA_ClearFlag(DMA1_FLAG_GL6);
    DMA_Cmd(DMA1_Channel6, ENABLE);


    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);


    USART_Cmd(USART2, ENABLE);
}

void USART2_Start_DMA_TX(uint8_t *buf, uint16_t num)
{
	if(!usart2_tBusy)
	{
		usart2_tBusy = 1;
		
		DMA_Cmd(DMA1_Channel7, DISABLE);
		DMA1_Channel7->CMAR = (uint32_t)buf;
		DMA1_Channel7->CNDTR = (uint32_t)num;
		DMA_ClearFlag(DMA1_FLAG_GL7);
		DMA_Cmd(DMA1_Channel7, ENABLE);
	}
}


void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);	//PB10-TX 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);	//PB11-RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/* Enable the USART3 Interrupt*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_GetFlagStatus(USART3, USART_FLAG_TC);
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_DeInit(DMA1_Channel2);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart3_tBuf;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	DMA_ClearFlag(DMA1_FLAG_GL2);
	DMA_Cmd(DMA1_Channel2, DISABLE);
	
	DMA_Cmd(DMA1_Channel3, DISABLE);
	DMA_DeInit(DMA1_Channel3);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart3_rfBuf;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_BufferSize = USART3_RXMAX_SIZE;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	DMA_ClearFlag(DMA1_FLAG_GL3);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
	
	USART_Cmd(USART3, ENABLE);
}

void USART3_Start_DMA_TX(uint8_t *buf, uint16_t num)
{
	if(!usart3_tBusy)
	{
		usart3_tBusy = 1;
		
		DMA_Cmd(DMA1_Channel2, DISABLE);
		DMA1_Channel2->CMAR = (uint32_t)buf;
		DMA1_Channel2->CNDTR = (uint32_t)num;
		DMA_ClearFlag(DMA1_FLAG_GL2);
		DMA_Cmd(DMA1_Channel2, ENABLE);
	}
}


void USART2_Send_String(char *str)
{
//	while(*str)
//	{
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//		
//		USART_SendData(USART2, *str++);
//	}
	
	strcpy((char *)usart2_tBuf, str);
	
	USART2_Start_DMA_TX(usart2_tBuf, strlen(str));
}

void USART2_Send_Bytes(uint8_t *dat, uint16_t len)
{
//	while(len--)
//	{
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//		
//		USART_SendData(USART2, *dat++);
//	}
	
	memcpy(usart2_tBuf, dat, len);
	
	USART2_Start_DMA_TX(usart2_tBuf, len);
}

uint8_t Wait_USART2_TX_Finish(uint16_t timeout)
{
	while(timeout)
	{
		Delay_ms(1);
		if(!usart2_tBusy)
		{
			break;
		}
		timeout--;
	}
	if(timeout > 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void USART2_Send_Byte(uint8_t data)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART2, data);
}

void USART2_rBuf_Clear(void)
{
	memset(usart2_rBuf, 0, USART2_RXBUF_SIZE);
	usart2_rNum = 0;
}


void USART3_Send_String(char *str)
{
//	while(*str)
//	{
//		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//		
//		USART_SendData(USART3, *str++);
//	}
	
	strcpy((char *)usart3_tBuf, str);
	
	USART3_Start_DMA_TX(usart3_tBuf, strlen(str));
}

void USART3_Send_Bytes(uint8_t *dat, uint16_t len)
{
//	while(len--)
//	{
//		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//		
//		USART_SendData(USART3, *dat++);
//	}
	
	memcpy(usart3_tBuf, dat, len);
	
	USART3_Start_DMA_TX(usart3_tBuf, len);
}

uint8_t Wait_USART3_TX_Finish(uint16_t timeout)
{
	while(timeout)
	{
		Delay_ms(1);
		if(!usart3_tBusy)
		{
			break;
		}
		timeout--;
	}
	if(timeout > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void USART3_Send_Byte(uint8_t data)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART3, data);
}

void USART3_rBuf_Clear(void)
{
	memset(usart3_rBuf, 0, USART3_RXBUF_SIZE);
	usart3_rNum = 0;
}



void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7))
	{
		DMA_Cmd(DMA1_Channel7, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL7);
		
		USART_ClearFlag(USART2, USART_IT_TC);
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	}
}

void USART2_IRQHandler(void)
{
	uint16_t newFrameLen;
	
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		usart2_tBusy = 0;
		
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
        {	
		DMA_Cmd(DMA1_Channel6, DISABLE);
		
		newFrameLen = USART2_RXMAX_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);
		
		if((usart2_rNum + newFrameLen) > USART2_RXBUF_SIZE)
		{
			usart2_rNum = 0;
		}		
		
		memcpy(usart2_rBuf + usart2_rNum, usart2_rfBuf, newFrameLen);
		
		usart2_rNum += newFrameLen;
		usart2_rOk = 1;
		
		DMA_SetCurrDataCounter(DMA1_Channel6, USART2_RXMAX_SIZE);
		
		DMA_ClearFlag(DMA1_FLAG_GL6);
		DMA_Cmd(DMA1_Channel6, ENABLE);
		
		USART_ReceiveData(USART2);
        }
	
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
	{
		USART_ReceiveData(USART2);
	}
}

void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC2))
	{
		DMA_Cmd(DMA1_Channel2, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL2);
		
		USART_ClearFlag(USART3, USART_IT_TC);
		USART_ITConfig(USART3, USART_IT_TC, ENABLE);
	}
}

void USART3_IRQHandler(void)
{
	uint16_t newFrameLen;
	
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		usart3_tBusy = 0;
		
		USART_ITConfig(USART3, USART_IT_TC, DISABLE);
		USART_ClearITPendingBit(USART3, USART_IT_TC);
	}
	
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
        {	
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		newFrameLen = USART3_RXMAX_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3);
		
		if((usart3_rNum + newFrameLen) > USART3_RXBUF_SIZE)
		{
			usart3_rNum = 0;
		}		
		
		memcpy(usart3_rBuf + usart3_rNum, usart3_rfBuf, newFrameLen);
		
		usart3_rNum += newFrameLen;
		usart3_rOk = 1;

		DMA_SetCurrDataCounter(DMA1_Channel3, USART3_RXMAX_SIZE);
		
		DMA_ClearFlag(DMA1_FLAG_GL3);
		DMA_Cmd(DMA1_Channel3, ENABLE);
		
		USART_ReceiveData(USART3);
        }
	
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET)
	{
		USART_ReceiveData(USART3);
	}
}


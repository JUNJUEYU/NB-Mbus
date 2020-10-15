#ifndef __USART_H
#define __USART_H

#include "stm32l1xx.h"


#define USART3_ENABLE                                           0

#define USART1_TXBUF_SIZE					10
#define USART1_RXBUF_SIZE					60
#define USART1_RXMAX_SIZE					30

#define USART2_TXBUF_SIZE					10
#define USART2_RXBUF_SIZE					60
#define USART2_RXMAX_SIZE					30

#define USART3_TXBUF_SIZE					100
#define USART3_RXBUF_SIZE					600
#define USART3_RXMAX_SIZE					300



//-------------------------------------------------//
extern uint8_t usart1_rfBuf[USART1_RXMAX_SIZE];

extern uint8_t usart1_rBuf[USART1_RXBUF_SIZE];
extern __IO uint8_t usart1_rOk;
extern __IO uint16_t usart1_rNum;

extern uint8_t usart1_tBuf[USART1_TXBUF_SIZE];
extern __IO uint8_t usart1_tBusy;
extern __IO uint16_t usart1_tNum;
//-------------------------------------------------//
extern uint8_t usart2_rBuf[USART2_RXBUF_SIZE];
extern __IO	uint8_t usart2_rOk;
extern __IO	uint16_t usart2_rNum;

extern uint8_t usart2_tBuf[USART2_TXBUF_SIZE];
extern __IO	uint8_t usart2_tBusy;
extern __IO uint16_t usart2_tNum;
//-------------------------------------------------//
extern uint8_t usart3_rBuf[USART3_RXBUF_SIZE];
extern __IO uint8_t usart3_rOk;
extern __IO uint16_t usart3_rNum;

extern uint8_t usart3_tBuf[USART3_TXBUF_SIZE];
extern __IO uint8_t usart3_tBusy;
extern __IO uint16_t usart3_tNum;
//-------------------------------------------------//


void USART1_Config(void);
void USART2_Config(void);
void USART3_Config(void);

void USART2_Start_DMA_TX(uint8_t *buf, uint16_t num);
void USART3_Start_DMA_TX(uint8_t *buf, uint16_t num);


void USART2_Send_String(char *str);
void USART2_Send_Bytes(uint8_t *dat, uint16_t len);
void USART2_Send_Byte(uint8_t data);
void USART2_rBuf_Clear(void);

uint8_t Wait_USART2_TX_Finish(uint16_t timeout);

void USART3_Send_String(char *str);
void USART3_Send_Bytes(uint8_t *dat, uint16_t len);
void USART3_Send_Byte(uint8_t data);
void USART3_rBuf_Clear(void);

uint8_t Wait_USART3_TX_Finish(uint16_t timeout);


#endif

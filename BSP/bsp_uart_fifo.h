/*
*********************************************************************************************************
*
*	妯″潡鍚嶇О : 涓插彛涓柇+FIFO椹卞姩妯″潡
*	鏂囦欢鍚嶇О : bsp_uart_fifo.h
*	璇�    鏄� : 澶存枃浠�
*
*	Copyright (C), 2015-2020, 瀹夊瘜鑾辩數瀛� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

/*
	濡傛灉闇€瑕佹洿鏀逛覆鍙ｅ搴旂殑绠¤剼锛岃鑷淇敼 bsp_uart_fifo.c鏂囦欢涓殑 static void InitHardUart(void)鍑芥暟
*/


/*
    瀹夊瘜鑾盨TM32-V5 涓插彛鍒嗛厤锛�
    銆愪覆鍙�1銆� RS232 鑺墖绗�1璺€�
        PB6/USART1_TX	  --- 鎵撳嵃璋冭瘯鍙�
        PB7/USART1_RX

    銆愪覆鍙�2銆� PA2 绠¤剼鐢ㄤ簬浠ュお缃戯紱 RX绠¤剼鐢ㄤ簬鎺ユ敹GPS淇″彿
        PA2/USART2_TX   
        PA3/USART2_RX	;鎺PS妯″潡杈撳嚭

    銆愪覆鍙�3銆� RS485 閫氫俊 - TTL 璺崇嚎 鍜� 鎺掗拡
        PB10/USART3_TX
        PB11/USART3_RX
*/

#define	UART1_FIFO_EN	1
#define	UART2_FIFO_EN	1
#define	UART3_FIFO_EN	1
#define	UART4_FIFO_EN	0
#define	UART5_FIFO_EN	0
#define	UART6_FIFO_EN	0

/* RS485鑺墖鍙戦€佷娇鑳紾PIO, PB2 */
#define RCC_RS485_TXEN      RCC_AHBPeriph_GPIOB
#define PORT_RS485_TXEN     GPIOB
#define PIN_RS485_TXEN	    GPIO_Pin_0

#define RS485_RX_EN()	    PORT_RS485_TXEN->BSRRH = PIN_RS485_TXEN
#define RS485_TX_EN()	    PORT_RS485_TXEN->BSRRL = PIN_RS485_TXEN


/* 瀹氫箟绔彛鍙� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 鎴�  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 鎴� PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5	/* USART6, PC6, PC7 */
}COM_PORT_E;

/* 瀹氫箟涓插彛娉㈢壒鐜囧拰FIFO缂撳啿鍖哄ぇ灏忥紝鍒嗕负鍙戦€佺紦鍐插尯鍜屾帴鏀剁紦鍐插尯, 鏀寔鍏ㄥ弻宸� */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			2400
	#define UART1_TX_BUF_SIZE	1*512
	#define UART1_RX_BUF_SIZE	1*512
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			9600
	#define UART2_TX_BUF_SIZE	1*512
	#define UART2_RX_BUF_SIZE	1*512
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			2500
	#define UART3_TX_BUF_SIZE	1*512
	#define UART3_RX_BUF_SIZE	1*512
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif

#if UART6_FIFO_EN == 1
	#define UART6_BAUD			115200
	#define UART6_TX_BUF_SIZE	1*1024
	#define UART6_RX_BUF_SIZE	1*1024
#endif

/* 涓插彛璁惧缁撴瀯浣� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32鍐呴儴涓插彛璁惧鎸囬拡 */
	uint8_t *pTxBuf;			/* 鍙戦€佺紦鍐插尯 */
	uint8_t *pRxBuf;			/* 鎺ユ敹缂撳啿鍖� */
	uint16_t usTxBufSize;		/* 鍙戦€佺紦鍐插尯澶у皬 */
	uint16_t usRxBufSize;		/* 鎺ユ敹缂撳啿鍖哄ぇ灏� */
	__IO uint16_t usTxWrite;	/* 鍙戦€佺紦鍐插尯鍐欐寚閽� */
	__IO uint16_t usTxRead;		/* 鍙戦€佺紦鍐插尯璇绘寚閽� */
	__IO uint16_t usTxCount;	/* 绛夊緟鍙戦€佺殑鏁版嵁涓暟 */

	__IO uint16_t usRxWrite;	/* 鎺ユ敹缂撳啿鍖哄啓鎸囬拡 */
	__IO uint16_t usRxRead;		/* 鎺ユ敹缂撳啿鍖鸿鎸囬拡 */
	__IO uint16_t usRxCount;	/* 杩樻湭璇诲彇鐨勬柊鏁版嵁涓暟 */

	void (*SendBefor)(void); 	/* 寮€濮嬪彂閫佷箣鍓嶇殑鍥炶皟鍑芥暟鎸囬拡锛堜富瑕佺敤浜嶳S485鍒囨崲鍒板彂閫佹ā寮忥級 */
	void (*SendOver)(void); 	/* 鍙戦€佸畬姣曠殑鍥炶皟鍑芥暟鎸囬拡锛堜富瑕佺敤浜嶳S485灏嗗彂閫佹ā寮忓垏鎹负鎺ユ敹妯″紡锛� */
	void (*ReciveNew)(uint8_t _byte);	/* 涓插彛鏀跺埌鏁版嵁鐨勫洖璋冨嚱鏁版寚閽� */
}UART_T;

void bsp_InitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);
void comSetBaud(COM_PORT_E _ucPort, uint32_t _BaudRate);

void USART_SetBaudRate(USART_TypeDef* USARTx, uint32_t BaudRate);

void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);
void RS485_SendStr(char *_pBuf);
void RS485_SetBaud(uint32_t _baud);

//--------------------------------------------------//
/*
wubo:鍚庡姞鐨�3涓嚱鏁�
*/
void UartTxd(uint8_t *pch,uint16_t n);

uint16_t GetRxCount(COM_PORT_E _ucPort);
uint16_t myMemCpy(COM_PORT_E _ucPort,uint8_t *_pByte);
uint16_t comMemCpy(COM_PORT_E _ucPort,uint8_t *_pByte);
//--------------------------------------------------//
#endif

/***************************** 瀹夊瘜鑾辩數瀛� www.armfly.com (END OF FILE) *********************************/

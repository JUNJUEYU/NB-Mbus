
#ifndef _BSP_H_
#define _BSP_H


#define  USE_FreeRTOS                   0

#if USE_FreeRTOS == 1
	#include "FreeRTOS.h"
	#include "task.h"
	#define DISABLE_INT()           taskENTER_CRITICAL()
	#define ENABLE_INT()            taskEXIT_CRITICAL()

    #define  USER_PRI               configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#else
	/* 开关全局中断的宏 */
	#define ENABLE_INT()            __set_PRIMASK(0)	/* 使能全局中断 */
	#define DISABLE_INT()           __set_PRIMASK(1)	/* 禁止全局中断 */
    #define  USER_PRI               5
#endif


/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf                      printf
//#define BSP_Printf(...)

#include "stm32l1xx.h"  //#include "stm32f10x.h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
#define TRUE                            1
#endif

#ifndef FALSE
#define FALSE                           0
#endif

#ifndef OK
#define OK                              1    /* No error, everything OK. */
#endif

#ifndef ERROR
#define ERROR                           0    /* Out of memory error.     */
#endif

#ifndef NULL
#define NULL                            ((void *)0)
#endif


#if 1
#define     DBGCOM                      COM1    // 调试口
#endif

#define     BL_COM                      COM1    // ??UART
#define     NB_COM                      COM2    // NBIOT接口

//==============================================================================//

#define     POWER_PIN                   GPIO_Pin_7
#define     POWER_PORT                  GPIOB
#define     POWER_CLOCK                 RCC_AHBPeriph_GPIOB
#define     POWER_ON()                  GPIO_WriteBit(POWER_PORT,POWER_PIN,Bit_SET)    
#define     POWER_OFF()                 GPIO_WriteBit(POWER_PORT,POWER_PIN,Bit_RESET)

#define     NBRESET_PIN                 GPIO_Pin_8
#define     NBRESET_PORT                GPIOB
#define     NBRESET_CLOCK               RCC_AHBPeriph_GPIOB
#define     NBRESET_ON()                GPIO_WriteBit(NBRESET_PORT,NBRESET_PIN,Bit_SET)    
#define     NBRESET_OFF()               GPIO_WriteBit(NBRESET_PORT,NBRESET_PIN,Bit_RESET)


#define     NBPOWER_PIN                 GPIO_Pin_9
#define     NBPOWER_PORT                GPIOB
#define     NBPOWER_CLOCK               RCC_AHBPeriph_GPIOB
#define     NBPOWER_ON()                GPIO_WriteBit(NBPOWER_PORT,NBPOWER_PIN,Bit_SET)    
#define     NBPOWER_OFF()               GPIO_WriteBit(NBPOWER_PORT,NBPOWER_PIN,Bit_RESET)

#define     RESET_PIN                   GPIO_Pin_8 
#define     RESET_IOPORT                GPIOA
#define     RESET_CLOCK                 RCC_AHBPeriph_GPIOA

#define     LRRESET_ON()                GPIO_WriteBit(RESET_IOPORT,RESET_PIN,Bit_SET)    
#define     LRRESET_OFF()               GPIO_WriteBit(RESET_IOPORT,RESET_PIN,Bit_RESET)

//==============================================================================//

#define     MBUS_COM                      COM3    // MBUS接口

//==============================================================================//

#define     MBUS_PIN                   GPIO_Pin_7
#define     MBUS_PORT                  GPIOA
#define     MBUS_CLOCK                 RCC_AHBPeriph_GPIOA
#define     MBUS_OFF()                  GPIO_WriteBit(MBUS_PORT,MBUS_PIN,Bit_SET)    
#define     MBUS_ON()                 GPIO_WriteBit(MBUS_PORT,MBUS_PIN,Bit_RESET) 
//==============================================================================//

#define     LED_PIN 						GPIO_Pin_12         
#define     LED_PORT                  	GPIOA
#define     LED_CLOCK                  RCC_AHBPeriph_GPIOA
#define     LED_OFF()                  	GPIO_WriteBit(LED_PORT,LED_PIN,Bit_SET)    
#define     LED_ON()                	GPIO_WriteBit(LED_PORT,LED_PIN,Bit_RESET) 
#define	    LED_TOG()                   GPIO_ToggleBits(LED_PORT,LED_PIN)

//==============================================================================//
#define     E2WP_PIN                    GPIO_Pin_5
#define     E2WP_PORT                   GPIOB
#define     E2WP_CLOCK                  RCC_AHBPeriph_GPIOB
#define     E2WP_LOW()                  GPIO_WriteBit(LED_PORT,LED_PIN,Bit_RESET)      
#define     E2WP_HIGH()                 GPIO_WriteBit(LED_PORT,LED_PIN,Bit_SET)  
//==============================================================================//
#define     EX_KEY_PIN                  GPIO_Pin_8
#define     EX_KEY_PORT                 GPIOA
#define     EX_KEY_CLOCK                RCC_AHBPeriph_GPIOA

#define     BL_STA_PIN                  GPIO_Pin_11
#define     BL_STA_PORT                 GPIOA
#define     BL_STA_CLOCK                RCC_AHBPeriph_GPIOA

#define     SUBPOW_PIN                  GPIO_Pin_7
#define     SUBPOW_PORT                 GPIOA
#define     SUBPOW_CLOCK                RCC_AHBPeriph_GPIOA
#define     SUBPOW_ON()                 GPIO_WriteBit(SUBPOW_PORT,SUBPOW_PIN,Bit_SET)    
#define     SUBPOW_OFF()                GPIO_WriteBit(SUBPOW_PORT,SUBPOW_PIN,Bit_RESET)
typedef struct 
{
      uint32_t          mbCadF:         1;          // lora cad
      uint32_t          mbTxdF:         1;          // lora发送
      uint32_t          mbRxdF:         1;          // lora接收
      
      uint32_t          mbPulseF1:      1;       
      uint32_t          mbPulseF2:      1; 
      
      uint32_t          mbAlmF:         1;          // 闹钟唤醒标志                        
      uint32_t          mbTimF:         1;          // 25ms时序标志        
      uint32_t          mbTrgF:         1;          // 干簧管触发有效
	  uint32_t        mbHndF:         1;          // 按键触发
	  uint32_t        mBlueF:         1;          // 蓝牙触发
      
      uint32_t          mbNbEn:         1;          // NB发送使能   
	  uint32_t	        mbNbcmp;
      
      uint32_t          mbRec:          23;               
	uint32_t        mbWorkF:           3; 	
}STRFLAG;

//==============================================================================//


/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */

#include "delay.h"

#include "TIM.h"
#include "RTC.h"

#include "EEPROM.h"
#include "bsp_I2C_GPIO.h"
#include "bsp_EEPROM_24xx.h"

#include "bsp_Adc.h"
#include "bsp_uart_fifo.h"

/*
#include "hal_interface.h"
#include "bsp_SX1278.h"
#include "radio.h"
#include "sx1276.h"
#include "sx1276-board.h"
*/

extern  __IO   STRFLAG    gstuFlag;
//==============================================================================//


void bsp_Init(void);

void To_Enter_Stop(void);
void To_Enter_Wait(uint32_t n);
void mbus_shutdown(void);

void Exit_Ctrl(uint8_t En);

extern	uint16_t 	timeout;


#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

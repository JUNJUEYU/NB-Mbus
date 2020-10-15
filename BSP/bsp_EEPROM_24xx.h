/*
*********************************************************************************************************
*                                     STM32F4 Driver Platform
*
*                             (c) Copyright 2015; 莱克科技发展有限公司
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     the BSP of I2C by GPIO
*                             ST Microelectronics STM32 with the STM32F407IGT6
*
* Filename      : bsp_EEPROM_24xx.h
* Version       : V1.0.0
* Programmer(s) : from development board of armfly
*********************************************************************************************************
*/

#ifndef _BSP_EEPROM_24XX_H
#define _BSP_EEPROM_24XX_H

#include <stdint.h>

//#define AT24C02
//#define AT24C04
//#define AT24C08
//#define AT24C16
//#define FM24CL16BG

//#define AT24C32
#define     M24C64
//#define AT24C128
//#define	M24M02


#ifdef AT24C02
	#define EE_MODEL_NAME		"AT24C02"
	#define EE_DEV_ADDR1		0xA0		/* 设备地址 */
	#define EE_DEV_ADDR2		0xA2		/* 设备地址 */
	#define EE_PAGE_SIZE		8			/* 页面大小(字节) */
	#define EE_SIZE				256			/* 总容量(字节) */	//total page :32
	#define EE_ADDR_BYTES		1			/* 地址字节个数 */
#endif

#ifdef AT24C04
	#define EE_MODEL_NAME		"AT24C04"
	#define EE_DEV_ADDR1		0xA0		/* 设备地址 */
	#define EE_DEV_ADDR2		0xA2		/* 设备地址 */
	#define EE_PAGE_SIZE		16			/* 页面大小(字节) */
	#define EE_SIZE				512			/* 总容量(字节) */	//total page :32
	#define EE_ADDR_BYTES		1			/* 地址字节个数 */
#endif

#ifdef AT24C08
	#define EE_MODEL_NAME		"AT24C08"
	#define EE_DEV_ADDR1		0xA0		/* 设备地址 */
	#define EE_DEV_ADDR2		0xA2		/* 设备地址 */
	#define EE_PAGE_SIZE		16			/* 页面大小(字节) */  //total page :64
	#define EE_SIZE				(1*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		1			/* 地址字节个数 */
#endif

/*
ds:写24C16分区
第1区256b,ds=0xa0;第2区256b,ds=0xa2
第3区256b,ds=0xa4;第4区256b,ds=0xa6
第5区256b,ds=0xa8;第6区256b,ds=0xaa
第7区256b,ds=0xac;第8区256b,ds=0xae
*/
//------------------------------------------------------------------------//
#ifdef AT24C16
	#define EE_MODEL_NAME		"AT24C16"
	#define EE_DEV_ADDR1		0xA0		/* 设备地址 */
	#define EE_DEV_ADDR2		0xA8		/* 设备地址 */
	#define EE_PAGE_SIZE		16			/* 页面大小(字节) */	//total page :128
	#define EE_SIZE				(2*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		1			/* 地址字节个数 */
#endif

#ifdef FM24CL16BG
	#define EE_MODEL_NAME		"FM24CL16BG"
	#define EE_DEV_ADDR1		0xA0		/* 1#区地址 */
	#define EE_DEV_ADDR2		0xA2		/* 2#区地址 */
    #define EE_DEV_ADDR3		0xA4		/* 3#区地址 */
    #define EE_DEV_ADDR4		0xA6		/* 4#区地址 */
    #define EE_DEV_ADDR5		0xA8		/* 5#区地址 */
    #define EE_DEV_ADDR6		0xAA		/* 6#区地址 */
    #define EE_DEV_ADDR7		0xAC		/* 7#区地址 */
    #define EE_DEV_ADDR8		0xAE		/* 8#区地址 */
	#define EE_PAGE_SIZE		16	        /* 页面大小(字节) */ //total page :128
	#define EE_SIZE				(2*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		1			/* 地址字节个数 */
#endif
//------------------------------------------------------------------------//
#ifdef AT24C32
	#define EE_MODEL_NAME		"AT24C32"
	#define EE_DEV_ADDR			0xA0		/* 设备地址 */    
	#define EE_PAGE_SIZE		32			/* 页面大小(字节) */ //total page :128   
	#define EE_SIZE				(4*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		2			/* 地址字节个数 */
#endif

#ifdef M24C64
	#define EE_MODEL_NAME		"M24C64"
	#define EE_DEV_ADDR			0xA0		/* 设备地址 */
	#define EE_PAGE_SIZE		32			/* 页面大小(字节) */ //total page :256    
	#define EE_SIZE				(8*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		2			/* 地址字节个数 */
#endif
//------------------------------------------------------------------------//
#ifdef AT24C128
	#define EE_MODEL_NAME		"AT24C128"
	#define EE_DEV_ADDR			0xA0		/* 设备地址 */    
	#define EE_PAGE_SIZE		64			/* 页面大小(字节) */ //total page :256 
	#define EE_SIZE				(16*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		2			/* 地址字节个数 */
#endif

#ifdef M24M02
	#define EE_MODEL_NAME		"M24M02"
	#define EE_DEV_ADDR1		0xA0		/* 设备地址 */
	#define EE_DEV_ADDR2		0xA8		/* 设备地址 */
	#define EE_PAGE_SIZE		(256)	    /* 页面大小(字节) */
	#define EE_SIZE				(256*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		2			/* 地址字节个数 */
#endif
//------------------------------------------------------------------------//
uint32_t BSP_EE_CheckOk(uint8_t DevAddr);
uint32_t BSP_EE_ReadBytes(uint8_t DevAddr, uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint32_t BSP_EE_WriteBytes(uint8_t DevAddr, uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

/*
*********************************************************************************************************
*                                    STM32F4 Driver Platform
*
*                             (c) Copyright 2015; 鑾卞厠绉戞妧鍙戝睍鏈夐檺鍏徃
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
* Filename      : bsp_I2C_GPIO.h
* Version       : V1.0.0
* Programmer(s) : from development board of armfly
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include <stdint.h>

#define I2C_WR	0		/* 鍐欐帶鍒禸it */
#define I2C_RD	1		/* 璇绘帶鍒禸it */

void bsp_InitI2C(void);
void i2C_Reset(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif

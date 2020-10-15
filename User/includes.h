/*
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#define DBGMODE 1

/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                           宏定义
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                           	OS
*********************************************************************************************************
*/
#if 0

  #include "FreeRTOS.h"
  #include "task.h"
  #include "queue.h"
  #include "croutine.h"
  #include "semphr.h"
  #include "event_groups.h"

#endif

/*
*********************************************************************************************************
*                                         		BSP
*********************************************************************************************************
*/
#include "bsp.h"

/*
*********************************************************************************************************
*                                               APP
*********************************************************************************************************
*/
#include "main.h"

#include "app_UART1.h"
#include "app_NBIOT.h"
#include "app_NBRSV.h"
#include "app_RS485.h"      
#include "APP_EEPROM.h"
#include "app_MBUS.h"
#include "APP_Record.h"

//#include "APP_WATER.h"

/*
*********************************************************************************************************
*                                         	其它库
*********************************************************************************************************
*/



#endif

/********************************************************************************************************/

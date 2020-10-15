#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <string.h>

#include "stm32l1xx.h"



#pragma pack (1)			                        // 字节对齐		
typedef struct 
{
      uint8_t 	        mu1Head1;                   // 0x68
      uint8_t 	        mu1Head2; 			        // 0x10
#if 0
      uint8_t           mu1WaterNum[5];             // 水表序号
      uint16_t          mu2ZoneNum;                 // 地区码
#else      
      uint32_t          mu4WaterNum;
      uint32_t          mu4DtuId; 
#endif         
      uint8_t 	        mu1ComCmd;			        // 控制码			
      uint8_t 	        mu1Lenth;			        // lenth 	
      uint16_t 	        mu2Func; 			        // 功能码        
      uint8_t           mu1Ser;                     // 序列号					        
      //---------------------------------------------------------// 14bytes
      uint32_t 	        mu4TotFlow;                 // 水表指针流量  
      uint8_t           mu1Unit0;                   // 单位

      uint32_t 	        mu4CurFlow;                 // 当前流量（每次上报清零后的流量）
      uint8_t           mu1Unit1;                   // 单位

      uint8_t           mu1Rtc[6];        
      uint8_t 	        mu1State[3];                // 状态 						
      //---------------------------------------------------------//  33bytes		
      uint8_t 	        mu1Check; 			        // sum 	
      uint8_t 	        mu1Tail; 			        // 尾
      //---------------------------------------------------------//  35bytes
}STRWATER;
#pragma pack ()


extern  uint32_t  Timer;
extern  uint32_t  SleepTimer;

#define     NB_INIT_CNT             3
#endif




#ifndef __RTC_H
#define __RTC_H

#include "stm32l1xx.h"
#include "stdio.h"

/*
鏃堕棿璁剧疆
*/
typedef struct {
  uint8_t year, month, day, hours, minutes, seconds;
} TTime;



#pragma pack (1)			                // 瀛楄妭瀵归綈		
typedef struct 
{
      uint8_t 	mu1Head1;                               // 0x68
      uint8_t 	mu1Head2; 			                    // 0x10

      TTime     mRtcTime;                               // 瀹炴椂鏃堕挓
      //---------------------------------------------------------//  33bytes		
      uint8_t 	mu1Check; 			                    // sum 	
      uint8_t 	mu1Tail; 			                    // 灏�
      //---------------------------------------------------------//  35bytes
}STRWMACK;
#pragma pack ()




uint8_t RtcInit(void);
void RTC_TimeShow(void);
void RTC_DateShow(void);
void RTC_AlartSet(void);      

void RtcWakeUp_ms(uint32_t n);
void RtcWakeUpConfig(uint32_t m);

void Set_Time(TTime Rtc);
uint32_t ReadRtcTime(TTime   *pRtc);

#endif

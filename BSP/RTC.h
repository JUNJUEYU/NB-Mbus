
#ifndef __RTC_H
#define __RTC_H

#include "stm32l1xx.h"
#include "stdio.h"

/*
ʱ������
*/
typedef struct {
  uint8_t year, month, day, hours, minutes, seconds;
} TTime;



#pragma pack (1)			                // �ֽڶ���		
typedef struct 
{
      uint8_t 	mu1Head1;                               // 0x68
      uint8_t 	mu1Head2; 			                    // 0x10

      TTime     mRtcTime;                               // ʵʱʱ��
      //---------------------------------------------------------//  33bytes		
      uint8_t 	mu1Check; 			                    // sum 	
      uint8_t 	mu1Tail; 			                    // β
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

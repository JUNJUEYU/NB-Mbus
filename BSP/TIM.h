#ifndef __TIM_H
#define __TIM_H

#include <stdio.h>
#include "stm32l1xx.h"


#define TIM2_ClockPrescaler     21		    //得到100KHz时钟，即每 10us 计数器加1
#define TIM2_Period	            375	


/*
#define TIM2_ClockPrescaler 3200		//得到10KHz时钟，即每 100us 计数器加1
#define TIM2_Period		  		10000		//计数器所能计数的最大值,与预分频后得到的时钟共同决定了TIM2定时的长度为 10000*100us = 1s

#define TIM6_ClockPrescaler 3200		//得到10KHz时钟，即每 100us 计数器加1
#define TIM6_Period		  		50000		//计数器所能计数的最大值,与预分频后得到的时钟共同决定了TIM6定时的长度为 50000*100us = 5s

#define TIM7_ClockPrescaler 3200		//得到10KHz时钟，即每 100us 计数器加1
#define TIM7_Period		  		50000		//计数器所能计数的最大值,与预分频后得到的时钟共同决定了TIM7定时的长度为 50000*100us = 5s
*/


void TIM_Config(void);


#endif

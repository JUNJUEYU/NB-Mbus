#ifndef __TIM_H
#define __TIM_H

#include <stdio.h>
#include "stm32l1xx.h"


#define TIM2_ClockPrescaler     21		    //�õ�100KHzʱ�ӣ���ÿ 10us ��������1
#define TIM2_Period	            375	


/*
#define TIM2_ClockPrescaler 3200		//�õ�10KHzʱ�ӣ���ÿ 100us ��������1
#define TIM2_Period		  		10000		//���������ܼ��������ֵ,��Ԥ��Ƶ��õ���ʱ�ӹ�ͬ������TIM2��ʱ�ĳ���Ϊ 10000*100us = 1s

#define TIM6_ClockPrescaler 3200		//�õ�10KHzʱ�ӣ���ÿ 100us ��������1
#define TIM6_Period		  		50000		//���������ܼ��������ֵ,��Ԥ��Ƶ��õ���ʱ�ӹ�ͬ������TIM6��ʱ�ĳ���Ϊ 50000*100us = 5s

#define TIM7_ClockPrescaler 3200		//�õ�10KHzʱ�ӣ���ÿ 100us ��������1
#define TIM7_Period		  		50000		//���������ܼ��������ֵ,��Ԥ��Ƶ��õ���ʱ�ӹ�ͬ������TIM7��ʱ�ĳ���Ϊ 50000*100us = 5s
*/


void TIM_Config(void);


#endif

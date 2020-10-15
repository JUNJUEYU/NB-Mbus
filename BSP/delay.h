#ifndef __DELAY_H
#define __DELAY_H

#include "stm32l1xx.h"

#define SysTick_Enable()	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk
#define SysTick_Disable()	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk


void delay(uint32_t ms);
uint32_t millis(void);


void Delay_us(uint32_t nTime);
void Delay_ms(uint32_t nTime);

#endif


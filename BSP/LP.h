
#ifndef __LP_H
#define __LP_H

#include "stm32l1xx.h"
#include "stdio.h"

#define Start_IAP_Flag()				GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)
#define Feed_IWDG()							IWDG_ReloadCounter()


void Enter_StandbyMode(void);
void IWDG_Config(void);
void IAP_GPIO_Config(void);


#endif

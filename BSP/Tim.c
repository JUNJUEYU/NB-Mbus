/*
*******************************************************************************
*
*                           the app_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : main.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "bsp.h"
#include "app_MBUS.h"

uint32_t  Timer = 0 ;
uint32_t  SleepTimer = 0 ;
uint32_t  ledtimer = 0 ;
uint16_t  timeout = 0;	 
uint16_t	sleeping = 0;

void TIM2_IRQHandler(void) 
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        gstuFlag.mbTimF = 1;
		Timer += 1;
		ledtimer += 1;
		if(gstuFlag.mbWorkF == CONFIG)
		{
			if(ledtimer >= 100)
			{
				LED_TOG();
				ledtimer = 0;
			}
		}
		else if(gstuFlag.mbWorkF == TRANS)
		{
//			SleepTimer ++;
			if(sleeping != 1)
			{
				if(ledtimer >= 48)
				{
					LED_ON();
					if(ledtimer >= 50)
					{
						ledtimer = 0;
						LED_OFF();
					}
				}
			}
			else
			{
				if(ledtimer >= 4)
				{
					LED_TOG();
					ledtimer = 0;
				}
			}
		}
		else 			
		{
			if(ledtimer >= 148)
			{	
				LED_ON();
				if(ledtimer >= 150)
				{
					ledtimer = 0;
					LED_OFF();
				}
			}
		}

    }
}

void TIM_Config(void)
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/**************************** TIM2 Config ***********************************/
	/* TIM2 TIM6 TIM7 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
	/* TIM2 Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler     = TIM2_ClockPrescaler - 1;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period        = TIM2_Period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* TIM2 IRQChannel enable */
	NVIC_InitStructure.NVIC_IRQChannel  = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* Enable the TIM2 counter */
	TIM_Cmd(TIM2, ENABLE);
#if 0	
	/* TIM6 Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = TIM6_ClockPrescaler - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM6_Period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
	/* TIM6 IRQChannel enable */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	/* Enable the TIM6 counter */
	TIM_Cmd(TIM6, DISABLE);
#endif

#if 0	
	/* TIM7 Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = TIM7_ClockPrescaler - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM7_Period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	/* TIM7 IRQChannel enable */
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	/* Enable the TIM7 counter */
	TIM_Cmd(TIM7, ENABLE);
#endif        
}


/*******************************(END OF FILE)**********************************/


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
#include "LP.h"

uint16_t batVoltage = 0;		//µ¥Î»mV




void Enter_StandbyMode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	RCC_AHBPeriphResetCmd(0x0000003F, DISABLE);

//	RCC_APB2PeriphResetCmd(0x00005A1D, DISABLE);
//	RCC_APB1PeriphResetCmd(0xA0E64A37, DISABLE);
	
	PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);
	
	PWR_FastWakeUpCmd(ENABLE);
	PWR_UltraLowPowerCmd(ENABLE);

	PWR_ClearFlag(PWR_FLAG_WU);
	PWR_ClearFlag(PWR_FLAG_SB);
	PWR_EnterSTANDBYMode();
        
//	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

void IWDG_Config(void)
{
        /* Enable the LSI oscillator ************************************************/
        RCC_LSICmd(ENABLE);

        /* Wait till LSI is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
        {}

        /* Enable write access to IWDG_PR and IWDG_RLR registers */
        IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

        /* IWDG counter clock: LSI(37 KHz)/256 */
        IWDG_SetPrescaler(IWDG_Prescaler_256);

        /* Set counter reload value to obtain 10s IWDG TimeOut */
        IWDG_SetReload(1445);

        /* Reload IWDG counter */
        IWDG_ReloadCounter();

        /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
        IWDG_Enable();
}

void IAP_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        //  EXTI_InitTypeDef EXTI_InitStructure;
        //  NVIC_InitTypeDef NVIC_InitStructure;

        /* Enable the GPIO Clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
        //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Configure Button pin as input */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
}



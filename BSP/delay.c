
#include "delay.h"

//volatile uint32_t TimingDelay;

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
//void TimingDelay_Decrement(void)
//{
//  if(TimingDelay != 0)
//  { 
//    TimingDelay--;
//  }
//}

//-----------------------------------------------------//

// 实测有3us
void Delay_us(uint32_t nTime)
{
    while(nTime--)
    {
        __asm volatile( "NOP" );
    }    
}

/*
ms延时函数
*/

void Delay_ms(uint32_t nTime)
{
    uint32_t i;
    
    while(nTime--)
    {
        for(i=350; i>0; i--)
        {
          __asm volatile( "NOP" );
        }   
    }
}

//-----------------------------------------------------//
void Soft_delay_us(uint32_t time)
{    
    Delay_us(time);
}

void Soft_delay_ms(uint32_t time)
{    
    Delay_ms(time);
}

//-----------------------------------------------------//

void delay(uint32_t ms)
{
    Delay_ms(ms);
}

uint32_t millis(void)
{    
    //return xTaskGetTickCount();
    return 0;
}
//-----------------------------------------------------//


/*******************************(END OF FILE)**********************************/


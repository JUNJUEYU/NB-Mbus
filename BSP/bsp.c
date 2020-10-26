/*
*******************************************************************************
*
*                           the app_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : bsp.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "bsp.h"

     
void Gpio_Cfg(void)
{   
    GPIO_InitTypeDef   GPIO_InitStructure;     
 
#if 1   // 对功耗很关键,不要遗漏(省电50ua)    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE); 
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;   
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOH, &GPIO_InitStructure);      
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, DISABLE);        
#endif  
    
#if 1   // 对功耗很关键,不要遗漏(省电40ua)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); 
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;          
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_13;          
    GPIO_Init(GPIOC,&GPIO_InitStructure);               
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, DISABLE);        
#endif    
//--------------------------------------------------------------//      
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);    
    
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;      
    GPIO_InitStructure.GPIO_Pin         = 0x9fff;               // PA13.PA14为SWD接口
    GPIO_Init(GPIOA,&GPIO_InitStructure);  
#if 1
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_UP;      
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_8;               
    GPIO_Init(GPIOA,&GPIO_InitStructure);              
#endif    
//--------------------------------------------------------------//         
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
    
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;   
    GPIO_InitStructure.GPIO_Pin         = 0xffff;          
    GPIO_Init(GPIOB,&GPIO_InitStructure);  
    
    // 电路板上有上拉电阻，所以要用CPU的内部上拉、或不拉;
    // 因为pb10/pb11外接485芯片，因此，不要拉电阻，以防额外费电。
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;                     
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;          
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB,&GPIO_InitStructure);      
//--------------------------------------------------------------//      
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;                  
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;      

    NBRESET_OFF();      // 置低
    GPIO_InitStructure.GPIO_Pin         = NBRESET_PIN;
    GPIO_Init(NBRESET_PORT, &GPIO_InitStructure);               // PA1:nbiot_rst
    
#if 0    
    LRRESET_ON();       // 置高
    GPIO_InitStructure.GPIO_Pin         = RESET_PIN;
    GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );             // PA8:lora_rst    
#endif    
                 
    POWER_OFF();        // 置低(不给电)
    GPIO_InitStructure.GPIO_Pin         = POWER_PIN;        
    GPIO_Init(POWER_PORT,&GPIO_InitStructure);                  //PB7:nbiot_pwr      
	
	LED_OFF();        // 置低(不给电)
    GPIO_InitStructure.GPIO_Pin         = LED_PIN;        
    GPIO_Init(LED_PORT,&GPIO_InitStructure);                  //PB7:nbiot_pwr    
	
	MBUS_OFF() ;        // 置低(不给电)
    GPIO_InitStructure.GPIO_Pin         = MBUS_PIN;        
    GPIO_Init(MBUS_PORT,&GPIO_InitStructure);                  //PB7:nbiot_pwr   
//--------------------------------------------------------------//    
}    

void Gpio_WkCfg(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;    
//--------------------------------------------------------------//       
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);   
    
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;       
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_UP;     
    GPIO_InitStructure.GPIO_Pin         = EX_KEY_PIN;           // PA8:?????
    GPIO_Init(EX_KEY_PORT, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin         = BL_STA_PIN;           // pa11:????
    GPIO_Init(BL_STA_PORT,&GPIO_InitStructure); 
//--------------------------------------------------------------//  
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;        
          
    LED_OFF();          
    GPIO_InitStructure.GPIO_Pin         = LED_PIN;              // pa12:led??
    GPIO_Init(LED_PORT,&GPIO_InitStructure);   
        
    SUBPOW_ON();
    GPIO_InitStructure.GPIO_Pin         = SUBPOW_PIN;           // pa7:????
    GPIO_Init(SUBPOW_PORT,&GPIO_InitStructure);  
//--------------------------------------------------------------//  
#if 0    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);   
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;      
    GPIO_InitStructure.GPIO_Pin         = PULSE1_PIN;           // pb15:????1
    GPIO_Init(PULSE1_PORT,&GPIO_InitStructure);                      
         
    GPIO_InitStructure.GPIO_Pin         = PULSE2_PIN;           // pb13:????2       
    GPIO_Init(PULSE2_PORT,&GPIO_InitStructure);       
#endif    
//--------------------------------------------------------------//          
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;      

    E2WP_HIGH();
    GPIO_InitStructure.GPIO_Pin         = E2WP_PIN;             // pb5:e2pw??
    GPIO_Init(E2WP_PORT,&GPIO_InitStructure);          
//--------------------------------------------------------------//     
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);   
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;                 
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;  
  
    NBRESET_OFF();    
    GPIO_InitStructure.GPIO_Pin         = NBRESET_PIN;          // PB8:????
    GPIO_Init(NBRESET_PORT, &GPIO_InitStructure);      
    
    NBPOWER_OFF();     
    GPIO_InitStructure.GPIO_Pin         = NBPOWER_PIN;          // PB9:????     
    GPIO_Init(NBPOWER_PORT,&GPIO_InitStructure);                 
//--------------------------------------------------------------//  
}
/*
低功耗管脚配置
*/
void Gpio_LpCfg(void)
{  
    GPIO_InitTypeDef   GPIO_InitStructure;           
//--------------------------------------------------------------//     
#if 1    	
    RCC_HSEConfig(RCC_HSE_OFF);
        
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE); 
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;   
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOH, &GPIO_InitStructure);      
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, DISABLE);        
#endif      
	

//--------------------------------------------------------------//  
#if 1   
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); 
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;          
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_13;          
    GPIO_Init(GPIOC,&GPIO_InitStructure);               
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, DISABLE);        
#endif 
//--------------------------------------------------------------//          
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);           
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;      
    GPIO_InitStructure.GPIO_Pin         = 0x9fff;               // PA13.PA14 SWD
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
    
    
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_8;           // PA8:按键
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_UP;     
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
 
       
//--------------------------------------------------------------//            
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;   
    GPIO_InitStructure.GPIO_Pin         = 0xffff;          
    GPIO_Init(GPIOB,&GPIO_InitStructure);       
    //---------------------------------------------------------//            
    // Eeprom?????????
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;           
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_UP;          
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB,&GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;           
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;          
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_9;
    GPIO_Init(GPIOB,&GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;          
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_1;
    GPIO_Init(GPIOB,&GPIO_InitStructure);   

    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_12;              // LED
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_UP;     
    GPIO_Init(GPIOA, &GPIO_InitStructure);       

    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_7;              // 子电源
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;     
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
}

void mbus_shutdown(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
	GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_7;              // 子电源
	GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;     
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
}
void LowPowerSet(void)
{

    //----------------------------------//                                    
    /*
    1.电源管理器为range1:1.8v; 
    2.电源管理器为range2:1.5v; 
    3.电源管理器为range3:1.2v                     
    */
    PWR_VoltageScalingConfig(PWR_VoltageScaling_Range3);       
    while(PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET) {
    }
    //----------------------------------//   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);       
    //----------------------------------//   
    PWR_PVDCmd(DISABLE);                // 禁用PVD
    PWR_UltraLowPowerCmd(ENABLE);       // 关闭Vref
    PWR_FastWakeUpCmd(ENABLE);          // 加快启动速度
    //----------------------------------//    
    DBGMCU_Config(DBGMCU_STOP,DISABLE); // 禁用调试接口                 
}


void To_Enter_Stop(void)
{
    Gpio_LpCfg();
//    LowPowerSet();
    //----------------------------------------------------------//
    RTC_AlartSet();                     // 设置闹钟唤醒
    //----------------------------------------------------------//       
	DBGMCU_Config(DBGMCU_STOP, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);    
    /* Enter Stop Mode */     
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);           
    //----------------------------------------------------------//       
    bsp_Init();
    //----------------------------------------------------------//      
}


void To_Enter_Wait(uint32_t n)
{
#if 1
    Delay_ms(n); 
#else
    RtcWakeUp_ms(n);                   // RTC定时唤醒
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);   
#endif  
}    
    


void Exit_Ctrl(uint8_t En)
{  
    if(En == 0)         // 屏蔽PB10/PB11中断
    {   
        EXTI->IMR &= (uint32_t)(~EXTI_Line10);      // 屏蔽#10中断
        EXTI->IMR &= (uint32_t)(~EXTI_Line11);      // 屏蔽#11中断
        
        EXTI->EMR &= (uint32_t)(~EXTI_Line10);      // 屏蔽#10事件
        EXTI->EMR &= (uint32_t)(~EXTI_Line11);      // 屏蔽#11事件
    }
    else                // 取消屏蔽位
    { 
        EXTI->IMR |= (uint32_t)(EXTI_Line10);    
        EXTI->EMR |= (uint32_t)(EXTI_Line10);    
        
        EXTI->IMR |= (uint32_t)(EXTI_Line11);              
        EXTI->EMR |= (uint32_t)(EXTI_Line11); 
    }
}    


void EXTI_Cfg(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    	    
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);              // 外部中断需要使能
               
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);               //??????  
    
    EXTI_InitStructure.EXTI_Line        = EXTI_Line8;                           //PA8:??  
    EXTI_InitStructure.EXTI_Mode        = EXTI_Mode_Interrupt;        
    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Falling;                 //?????
    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;      
    EXTI_Init(&EXTI_InitStructure);  
    
    NVIC_InitStructure.NVIC_IRQChannel  = EXTI9_5_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      
    NVIC_Init(&NVIC_InitStructure);  
	
	
	
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);      // 连接外部中断
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);      // 连接外部中断
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);      // 连接外部中断
//    /* Connect EXTI15 Line to Pb15 pin */    
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);      // DIO_0:连接外部中断   
//       
//#if UART3_FIFO_EN == 0
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line10;                  //中断线10      
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;      
//    EXTI_Init(&EXTI_InitStructure);                                     //初始化       
//    
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line11;                  //中断线11          
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;  
//    EXTI_Init(&EXTI_InitStructure);                                     //初始化    
//#endif  
//    
//	
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line12;                  //中断线12          
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;  
//    EXTI_Init(&EXTI_InitStructure);     
//    
//  
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line15;                  //中断线15
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Rising;          //上升沿触发 
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;                       
//    EXTI_Init(&EXTI_InitStructure);                                     //初始化   
//      
//    
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;    
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      
//    NVIC_Init(&NVIC_InitStructure);     
}


void RTC_EXIT(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    EXTI_InitTypeDef EXTI_InitStructure;
    
    //-----------------------------------------------------//    
    /* EXTI configuration */
    EXTI_ClearITPendingBit(EXTI_Line20);
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable the RTC Wakeup Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //-----------------------------------------------------//   
    /*Alarm必须配置EXTI_Line17中断*/
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
    EXTI_InitStructure.EXTI_Line = EXTI_Line17; 
    EXTI_Init(&EXTI_InitStructure); 
        
    /*使能 RTC Alarm Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;      // 要高于Wakeup中断
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //-----------------------------------------------------//       
}



void bsp_Init(void)
{ 
     /* 
    1.优先级分组设置为4，可配置0-15级抢占式优先级,0级子优先级，即不存在子优先级。    
    */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
    //-------------------------//
    Gpio_Cfg();     
	Gpio_WkCfg();
    //-------------------------//        
    RtcInit();          
    TIM_Config();           
    //-------------------------//  
	bsp_Adc_Init();
	//-------------------------// 
    bsp_InitUart();    
    //-------------------------//           
//  bsp_InitSpi();
	bsp_InitI2C();
    //-------------------------//          
	EXTI_Cfg();      
    RTC_EXIT();        
    //-------------------------//  
    /*
    1.放在初始化的最后面，
    2.可以防止没有考虑的情况（节约2ua）
    */
    LowPowerSet();   
    //-------------------------//     
}


/*******************************(END OF FILE)**********************************/


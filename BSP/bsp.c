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
 
#if 1   // �Թ��ĺܹؼ�,��Ҫ��©(ʡ��50ua)    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE); 
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;   
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOH, &GPIO_InitStructure);      
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, DISABLE);        
#endif  
    
#if 1   // �Թ��ĺܹؼ�,��Ҫ��©(ʡ��40ua)
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
    GPIO_InitStructure.GPIO_Pin         = 0x9fff;               // PA13.PA14ΪSWD�ӿ�
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
    
    // ��·�������������裬����Ҫ��CPU���ڲ�����������;
    // ��Ϊpb10/pb11���485оƬ����ˣ���Ҫ�����裬�Է�����ѵ硣
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;                     
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;          
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB,&GPIO_InitStructure);      
//--------------------------------------------------------------//      
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;                  
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;      

    NBRESET_OFF();      // �õ�
    GPIO_InitStructure.GPIO_Pin         = NBRESET_PIN;
    GPIO_Init(NBRESET_PORT, &GPIO_InitStructure);               // PA1:nbiot_rst
    
#if 0    
    LRRESET_ON();       // �ø�
    GPIO_InitStructure.GPIO_Pin         = RESET_PIN;
    GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );             // PA8:lora_rst    
#endif    
                 
    POWER_OFF();        // �õ�(������)
    GPIO_InitStructure.GPIO_Pin         = POWER_PIN;        
    GPIO_Init(POWER_PORT,&GPIO_InitStructure);                  //PB7:nbiot_pwr      
	
	LED_OFF();        // �õ�(������)
    GPIO_InitStructure.GPIO_Pin         = LED_PIN;        
    GPIO_Init(LED_PORT,&GPIO_InitStructure);                  //PB7:nbiot_pwr    
	
	MBUS_OFF() ;        // �õ�(������)
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
�͹��Ĺܽ�����
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
    
    
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_8;           // PA8:����
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

    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_7;              // �ӵ�Դ
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;     
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
}

void mbus_shutdown(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
	GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_7;              // �ӵ�Դ
	GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;     
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
}
void LowPowerSet(void)
{

    //----------------------------------//                                    
    /*
    1.��Դ������Ϊrange1:1.8v; 
    2.��Դ������Ϊrange2:1.5v; 
    3.��Դ������Ϊrange3:1.2v                     
    */
    PWR_VoltageScalingConfig(PWR_VoltageScaling_Range3);       
    while(PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET) {
    }
    //----------------------------------//   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);       
    //----------------------------------//   
    PWR_PVDCmd(DISABLE);                // ����PVD
    PWR_UltraLowPowerCmd(ENABLE);       // �ر�Vref
    PWR_FastWakeUpCmd(ENABLE);          // �ӿ������ٶ�
    //----------------------------------//    
    DBGMCU_Config(DBGMCU_STOP,DISABLE); // ���õ��Խӿ�                 
}


void To_Enter_Stop(void)
{
    Gpio_LpCfg();
//    LowPowerSet();
    //----------------------------------------------------------//
    RTC_AlartSet();                     // �������ӻ���
    //----------------------------------------------------------//       
//	DBGMCU_Config(DBGMCU_STOP, ENABLE);
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
    RtcWakeUp_ms(n);                   // RTC��ʱ����
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);   
#endif  
}    
    


void Exit_Ctrl(uint8_t En)
{  
    if(En == 0)         // ����PB10/PB11�ж�
    {   
        EXTI->IMR &= (uint32_t)(~EXTI_Line10);      // ����#10�ж�
        EXTI->IMR &= (uint32_t)(~EXTI_Line11);      // ����#11�ж�
        
        EXTI->EMR &= (uint32_t)(~EXTI_Line10);      // ����#10�¼�
        EXTI->EMR &= (uint32_t)(~EXTI_Line11);      // ����#11�¼�
    }
    else                // ȡ������λ
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);              // �ⲿ�ж���Ҫʹ��
               
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
	
	
	
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);      // �����ⲿ�ж�
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);      // �����ⲿ�ж�
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);      // �����ⲿ�ж�
//    /* Connect EXTI15 Line to Pb15 pin */    
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);      // DIO_0:�����ⲿ�ж�   
//       
//#if UART3_FIFO_EN == 0
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line10;                  //�ж���10      
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;      
//    EXTI_Init(&EXTI_InitStructure);                                     //��ʼ��       
//    
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line11;                  //�ж���11          
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;  
//    EXTI_Init(&EXTI_InitStructure);                                     //��ʼ��    
//#endif  
//    
//	
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line12;                  //�ж���12          
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;  
//    EXTI_Init(&EXTI_InitStructure);     
//    
//  
//    EXTI_InitStructure.EXTI_Line        = EXTI_Line15;                  //�ж���15
//    EXTI_InitStructure.EXTI_Trigger     = EXTI_Trigger_Rising;          //�����ش��� 
//    EXTI_InitStructure.EXTI_LineCmd     = ENABLE;                       
//    EXTI_Init(&EXTI_InitStructure);                                     //��ʼ��   
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
    /*Alarm��������EXTI_Line17�ж�*/
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
    EXTI_InitStructure.EXTI_Line = EXTI_Line17; 
    EXTI_Init(&EXTI_InitStructure); 
        
    /*ʹ�� RTC Alarm Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;      // Ҫ����Wakeup�ж�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //-----------------------------------------------------//       
}



void bsp_Init(void)
{ 
     /* 
    1.���ȼ���������Ϊ4��������0-15����ռʽ���ȼ�,0�������ȼ����������������ȼ���    
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
    1.���ڳ�ʼ��������棬
    2.���Է�ֹû�п��ǵ��������Լ2ua��
    */
    LowPowerSet();   
    //-------------------------//     
}


/*******************************(END OF FILE)**********************************/


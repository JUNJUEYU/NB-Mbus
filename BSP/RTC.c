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
#include "APP_EEPROM.h"

TTime   gucRtcTime;
/*
*********************************************************************************************************
*	�� �� ��: RTC_Alarm_IRQHandler
*	����˵��: ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void RTC_Alarm_IRQHandler(void) 
{ 
    if(RTC_GetFlagStatus(RTC_FLAG_ALRAF) != RESET) 
    {                
        PWR_RTCAccessCmd(ENABLE);               //�����У�����RTC_ClearITPendingBit ����ɹ�(wubo:����û������������)
        RTC_ClearITPendingBit(RTC_IT_ALRA); 
        PWR_RTCAccessCmd(DISABLE);                
                      
        gstuFlag.mbAlmF = 1;                    // ���ӻ��ѱ�־ 
    } 
    EXTI_ClearITPendingBit(EXTI_Line17);        //ע������д˾䣬�����ж�һֱ����������ѭ������ִ���ˡ� 
       
    if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
    {
        // ������ѱ�־
        PWR_ClearFlag(PWR_FLAG_WU);
    }      
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_WKUP_IRQHandler
*	����˵��: ���߻����ж�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 

void RTC_WKUP_IRQHandler(void)
{
    //������RTC�Ķ�ʱ�����ж�ǰ��һ��Ҫ�����Ӧ���жϣ�������ֵ�����Ϊ��ʱ�ܽ���RTC�жϣ���ʱ���ܽ���
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_WUT);      
    } 
    EXTI_ClearITPendingBit(EXTI_Line20);

    //RTC�������жϽ�CPU��ͣ��ģʽ����ʱ��һ��Ҫ���Wake-Up��־λ������ֻ�ܽ�һ��RTC�жϣ����������ˡ� 

    // ʡ��2.5uA
    //--------------------------------------------//
    if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)   // ��黽�ѱ�־�Ƿ���λ
    {
        PWR_ClearFlag(PWR_FLAG_WU);                 
    }  
    //--------------------------------------------//
}

/*
*********************************************************************************************************
*	�� �� ��: RtcWakeUp_ms
*	����˵��: �����������1000ms
*	��    �Σ�n < 1000;
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void RtcWakeUp_ms(uint32_t n)
{  
    uint32_t x = 0;
    
    PWR_RTCAccessCmd(ENABLE);           // �����üĴ���Ȩ��
//-----------------------------------------------------//      
    // �Ƚ�ֹ����������
    RTC_WakeUpCmd(DISABLE);                                             
    RTC_ClearFlag(RTC_FLAG_WUTF);
    //ѡ��ʱ��Ϊ�ⲿ32.768KHz 8��Ƶ
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div8);     
    //----------------------------//
    x = (n * 41 + 5) / 10;    
    if(x > 4095) {
      x = 4095;                     
    }
    RTC_SetWakeUpCounter(x);                
    //----------------------------//
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line20); // �����ǧ������
    //��Ҫʹ���жϣ�����Ҫ�жϺ���    
    RTC_ITConfig(RTC_IT_WUT,ENABLE);                                    
    RTC_WakeUpCmd(ENABLE);  
//-----------------------------------------------------//    
    PWR_RTCAccessCmd(DISABLE);          // һ��Ҫ��ֹ�������޷�����͹���ģʽ
}

/*
*********************************************************************************************************
*	�� �� ��: RtcWakeUpConfig
*	����˵��: ����ʱ������(1s)
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void RtcWakeUpConfig(uint32_t m)
{    
    PWR_RTCAccessCmd(ENABLE);           // �����üĴ���Ȩ��
//-----------------------------------------------------//    
    RTC_WakeUpCmd(DISABLE);                                             // �Ƚ�ֹ����������
    RTC_ClearFlag(RTC_FLAG_WUTF);
    
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);              // 1hz
    RTC_SetWakeUpCounter(m);                                            // ���
    
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line20);                                // �����ǧ������                
    
    RTC_ITConfig(RTC_IT_WUT,ENABLE);                                    //��Ҫʹ���жϣ�����Ҫ�жϺ���    
    RTC_WakeUpCmd(ENABLE);
//-----------------------------------------------------//    
    PWR_RTCAccessCmd(DISABLE);          // һ��Ҫ��ֹ�������޷�����͹���ģʽ
}


void RTC_AlartSet(void)
{
    RTC_AlarmTypeDef    RTC_AlarmStructure; 
    PWR_RTCAccessCmd(ENABLE);           // �����üĴ���Ȩ��
    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);     
#if 1
    // ������������
    if(gstuWmPara.mu1Tr_Ho > 23) {
      gstuWmPara.mu1Tr_Ho = 23;
    }  
    if(gstuWmPara.mu1Tr_Mi > 59) {
      gstuWmPara.mu1Tr_Mi = 59;
    }    
    if(gstuWmPara.mu1Tr_Se > 59) {
      gstuWmPara.mu1Tr_Se = 59;
    }
    //���ݲ����������� 
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12            = RTC_H12_AM; 
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours          = gstuWmPara.mu1Tr_Ho; 
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes        = gstuWmPara.mu1Tr_Mi; 
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds        = gstuWmPara.mu1Tr_Se; 
#else    
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12            = RTC_H12_AM; 
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours          = 0; 
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes        = 0; 
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds        = 0;           
#endif    
   
    RTC_AlarmStructure.RTC_AlarmDateWeekDay     = 31;
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel  = RTC_AlarmDateWeekDaySel_Date; // ������    
    
    RTC_AlarmStructure.RTC_AlarmMask    = RTC_AlarmMask_DateWeekDay;            // ���ڲ�����ƥ��
 
     
#if 0
	RTC_AlarmStructure.RTC_AlarmMask    |= RTC_AlarmMask_Hours;                 // Сʱ������ƥ��
    RTC_AlarmStructure.RTC_AlarmMask    |= RTC_AlarmMask_Minutes;               // ���Ӳ�����ƥ��           
    RTC_AlarmStructure.RTC_AlarmMask    |= RTC_AlarmMask_Seconds;               // ���Ӳ�����ƥ�� 
#endif    
    RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure);               // ��bin��ʽ����
     
    //RTC_ClearFlag(RTC_FLAG_ALRAF);        // ���RTC���ӱ�־
    RTC_ClearITPendingBit(RTC_IT_ALRA);     // ���RTC����A���жϱ�־
    EXTI_ClearITPendingBit(EXTI_Line17);    // ���LINE17�ϵ��жϱ�־λ 
        
    RTC_ITConfig(RTC_IT_ALRA,ENABLE);       // ʹ���ж�
    RTC_AlarmCmd(RTC_Alarm_A,ENABLE);       // ʹ���豸
    
    PWR_RTCAccessCmd(DISABLE);              // һ��Ҫ��ֹ�������޷�����͹���ģʽ
}    


/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
uint8_t RtcConfig(void)
{
    RTC_InitTypeDef     RTC_InitStructure;
    RTC_TimeTypeDef      RTC_TimeStructure;
     uint32_t temp = 0;   
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_RTCAccessCmd(ENABLE);

    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */ 
 
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
        temp++;
        Delay_ms(10);
        if(temp >= 250) return 1;//��ʼ��ʱ��ʧ��,���������⣬32768����������ʱ��1-5S 
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
    //----------------------------------------------------------------//
    /* Configure the RTC data register and RTC prescaler */

    RTC_InitStructure.RTC_AsynchPrediv  = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv   = 0xFF;
    RTC_InitStructure.RTC_HourFormat    = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
    //----------------------------------------------------------------//     
    RTC_TimeStructure.RTC_H12           = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours         = 0x00;         // 0��
    RTC_TimeStructure.RTC_Minutes       = 0x00;         // 0��
    RTC_TimeStructure.RTC_Seconds       = 0x00;         // 0��
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
    //----------------------------------------------------------------//        
    return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: RtcInit
*	����˵��: ��ѯ�󱸼Ĵ���������RTC��ʼ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t RtcInit(void)
{
    //����ǲ��ǵ�һ������ʱ��
    uint8_t flag = 0;
    //��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)      
    {
        /* RTC configuration  */
        flag = RtcConfig();
        if(flag == 0)
        {
            RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
        }
        else
            return flag;
    }
    else
    {
        /* Enable the PWR clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

        /* Allow access to RTC */
        PWR_RTCAccessCmd(ENABLE);

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();
    }
    return 0;
}   

/*
*********************************************************************************************************
*	�� �� ��: CorrectRtc
*	����˵��: ��������
*	��    �Σ�����ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t Is_Leep_Year(uint16_t year)
{
    if(year % 4 == 0)
    {
        if(year % 100 == 0) 
        {        
            if(year % 400 == 0) {            
                return 1;
            }
            else {            
                return 0;
            }        
        }      
        else {        
            return 1;
        }                       
    }
    else {    
        return 0;
    }
}

const uint8_t LEEP_DAYCOUNT[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t COMM_DAYCOUNT[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
*********************************************************************************************************
*	�� �� ��: CorrectRtc
*	����˵��: ���ݸ�������ʱ�䣬��������ʱ��
*	��    �Σ�rtcʱ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
TTime CorrectRtc(TTime Rtc)
{
    TTime       mRtcTime;     
    uint8_t     *pch;       
    uint16_t    u2year  = 0;
    uint8_t     u1year  = 0;
    uint8_t     u1month = 0;
    uint8_t     u1day   = 0;
    uint8_t     u1hour  = 0;
    
    uint8_t     u1Carry = 0;    // ��λ��־
//----------------------------------------//      
    u1hour  = Rtc.hours;
    u1hour += 8;
    if(u1hour < 24) 
    {
        u1Carry = 0;
    }
    else
    {          
        u1hour -= 24;
        u1Carry = 1;            // ���ڽ�λ
    }
    
    mRtcTime.hours      = u1hour;
    mRtcTime.minutes    = Rtc.minutes;
    mRtcTime.seconds    = Rtc.seconds;        
      
    u1year  = Rtc.year;
    u1month = Rtc.month;
    u1day   = Rtc.day;
 //----------------------------------------//     
    if(u1Carry > 0)
    {      
        // �·ݱ���Ϊ1~12    
        if(u1month < 1) {
            u1month = 1;
        }
        else if(u1month > 12) {          
            u1month = 12;
        }
            
        u2year = (uint16_t)(2000 + u1year);     // �����        
        if(Is_Leep_Year(u2year)) {           
            pch = (uint8_t *)LEEP_DAYCOUNT;     // ����       
        }
        else {   
            pch = (uint8_t *)COMM_DAYCOUNT;     // ƽ��
        }
                                 
        if(u1day < pch[u1month - 1])
        {
            u1day += 1;
            u1Carry = 0;            
        }
        else              
        {
            u1day   = 1;        // ��1�ſ�ʼ                
            u1Carry = 1;        // �¶Ƚ�λ    
        }   
                
        if(u1Carry > 0)
        {                  
            if(u1month < 12)
            {
                u1month += 1;
                u1Carry = 0;
            }  
            else
            {
                u1month = 1;
                u1year += 1;    // ��Ƚ�λ            
            }        
        }   
    } 
    
    mRtcTime.year   = u1year;
    mRtcTime.month  = u1month;
    mRtcTime.day    = u1day;

    return mRtcTime;
}




/*
������̽����RTC��ʱ������RTC�����ں�ʱ�䣬��������һֱ����Ч��ֻ��ϵͳ����������Ч��
���������Ľ�������ǣ�ֻ��Ҫ����������ʱ���������ȡһ������ʱ�䣬�һ�ȡ���ڵĺ���һ��Ҫ���ڻ�ȡʱ�亯��������ܽ����
*/

static uint8_t aShowTime[50] = {0};
static uint8_t aShowDate[50] = {0};

/*
*********************************************************************************************************
*	�� �� ��: ReadRtcTime
*	����˵��: ��ʾʱ��
*	��    �Σ���
*	�� �� ֵ: ���ص���������ֵ
*********************************************************************************************************
*/ 

uint32_t ReadRtcTime(TTime   *pRtc)
{
    uint32_t    u4tmp = 0;
    
    RTC_DateTypeDef  RTC_DateStructure;
    RTC_TimeTypeDef  RTC_TimeStructure;                
  	     
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);            // �Ȼ�ȡʱ��
    pRtc->hours    =  RTC_TimeStructure.RTC_Hours;
    pRtc->minutes  =  RTC_TimeStructure.RTC_Minutes;
    pRtc->seconds  =  RTC_TimeStructure.RTC_Seconds;       
   
  
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);            // �ٻ�ȡ����    
    pRtc->year   =  RTC_DateStructure.RTC_Year;
    pRtc->month  =  RTC_DateStructure.RTC_Month;
    pRtc->day    =  RTC_DateStructure.RTC_Date;    

    u4tmp = 0;
    u4tmp += (uint32_t)pRtc->hours * 3600;
    u4tmp += (uint32_t)pRtc->minutes * 60;
    u4tmp += (uint32_t)pRtc->seconds;
    
    return u4tmp;
}


/*
********************************************************************************
*	�� �� ��: RTC_TimeShow
*	����˵��: ��ʾʱ��
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************
*/ 
void RTC_TimeShow(void)
{
    RTC_TimeTypeDef  RTC_TimeStructure;

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	/* ʱ����ʾ��ʽ : hh:mm:ss */	
	sprintf((char*)aShowTime,"%0.2d:%0.2d:%0.2d",	RTC_TimeStructure.RTC_Hours, 
													RTC_TimeStructure.RTC_Minutes, 
													RTC_TimeStructure.RTC_Seconds);	
	printf("%s\r\n", aShowTime);	
}

/*
********************************************************************************
*	�� �� ��: RTC_DateShow
*	����˵��: ��ʾʱ��
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************
*/
void RTC_DateShow(void)
{
    RTC_DateTypeDef  RTC_DateStructure;

	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	sprintf((char*)aShowDate,"%0.2d-%0.2d-%0.2d-%0.1d",	RTC_DateStructure.RTC_Year, 
														RTC_DateStructure.RTC_Month, 
														RTC_DateStructure.RTC_Date,
														RTC_DateStructure.RTC_WeekDay);
	printf("%s\r\n", aShowDate);
}




/*
*********************************************************************************************************
*	�� �� ��: GetWeekDay
*	����˵��: �������ڼ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
static uint8_t GetWeekDay(TTime tTm)
{
    uint16_t uWeek = 0;
    uint16_t y,m,d=0;
    
    y = (uint16_t)(tTm.year) + 2000;                // 2019
    m = (uint16_t)(tTm.month);                      // 03
    d = (uint16_t)(tTm.day);                        // 14
    
    if (m >= 3)
    {
        m = m;        
    }
    else    /* ĳ���1��2��Ҫ������һ���13��14�������� */
    {
        m += 12;
        y -= 1;
    }
    
    // �õ�1 ~ 7����Ӧ��һ������
    uWeek = 1 + (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;   
          
    return (uint8_t)uWeek;
}

/*
*********************************************************************************************************
*	�� �� ��: Set_Time
*	����˵��: ����ʱ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void Set_Time(TTime Rtc)
{
    TTime       tTm;
                  
    RTC_DateTypeDef  RTC_DateStructure;
    RTC_TimeTypeDef  RTC_TimeStructure;
        
//-----------------------------------//            
    tTm = CorrectRtc(Rtc);                      // ���׶�ʱ����8Сʱ
//-----------------------------------//
    PWR_RTCAccessCmd(ENABLE);                   //ʹ�ܱ��ݼĴ�������                  
    /* ����ʱ���룬�Լ���ʾ��ʽ */
    //RTC_TimeStructure.RTC_H12       = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours     = tTm.hours;
    RTC_TimeStructure.RTC_Minutes   = tTm.minutes;
    RTC_TimeStructure.RTC_Seconds   = tTm.seconds; 
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);   
    
    /* ���������պ����� */
    RTC_DateStructure.RTC_Year      = tTm.year;
    RTC_DateStructure.RTC_Month     = tTm.month;
    RTC_DateStructure.RTC_Date 	    = tTm.day;
    RTC_DateStructure.RTC_WeekDay   = GetWeekDay(tTm);     // ������ȷ����WeekDay    
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);	
    
    PWR_RTCAccessCmd(DISABLE);     
 //-----------------------------------//    
    // ������ʱ�Ӻ��ٶ�һ��
    ReadRtcTime(&gucRtcTime);
}

/*******************************(END OF FILE)**********************************/

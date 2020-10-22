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
*	函 数 名: RTC_Alarm_IRQHandler
*	功能说明: 闹钟设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/ 
void RTC_Alarm_IRQHandler(void) 
{ 
    if(RTC_GetFlagStatus(RTC_FLAG_ALRAF) != RESET) 
    {                
        PWR_RTCAccessCmd(ENABLE);               //必须有，否则RTC_ClearITPendingBit 不会成功(wubo:好像并没有上述的问题)
        RTC_ClearITPendingBit(RTC_IT_ALRA); 
        PWR_RTCAccessCmd(DISABLE);                
                      
        gstuFlag.mbAlmF = 1;                    // 闹钟唤醒标志 
    } 
    EXTI_ClearITPendingBit(EXTI_Line17);        //注意必须有此句，否则中断一直被触发，主循环不被执行了。 
       
    if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
    {
        // 清除唤醒标志
        PWR_ClearFlag(PWR_FLAG_WU);
    }      
}

/*
*********************************************************************************************************
*	函 数 名: RTC_WKUP_IRQHandler
*	功能说明: 休眠唤醒中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/ 

void RTC_WKUP_IRQHandler(void)
{
    //在设置RTC的定时唤醒中断前，一定要清除相应的中断，否则表现的现象为有时能进入RTC中断，有时不能进。
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_WUT);      
    } 
    EXTI_ClearITPendingBit(EXTI_Line20);

    //RTC的周期中断将CPU从停机模式唤醒时，一定要清除Wake-Up标志位，否则只能进一次RTC中断，后续进不了。 

    // 省电2.5uA
    //--------------------------------------------//
    if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)   // 检查唤醒标志是否置位
    {
        PWR_ClearFlag(PWR_FLAG_WU);                 
    }  
    //--------------------------------------------//
}

/*
*********************************************************************************************************
*	函 数 名: RtcWakeUp_ms
*	功能说明: 最大休眠设置1000ms
*	形    参：n < 1000;
*	返 回 值: 无
*********************************************************************************************************
*/ 
void RtcWakeUp_ms(uint32_t n)
{  
    uint32_t x = 0;
    
    PWR_RTCAccessCmd(ENABLE);           // 打开配置寄存器权限
//-----------------------------------------------------//      
    // 先禁止，才能设置
    RTC_WakeUpCmd(DISABLE);                                             
    RTC_ClearFlag(RTC_FLAG_WUTF);
    //选择时钟为外部32.768KHz 8分频
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div8);     
    //----------------------------//
    x = (n * 41 + 5) / 10;    
    if(x > 4095) {
      x = 4095;                     
    }
    RTC_SetWakeUpCounter(x);                
    //----------------------------//
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line20); // 此语句千万不能少
    //需要使能中断，不需要中断函数    
    RTC_ITConfig(RTC_IT_WUT,ENABLE);                                    
    RTC_WakeUpCmd(ENABLE);  
//-----------------------------------------------------//    
    PWR_RTCAccessCmd(DISABLE);          // 一定要禁止，否则无法进入低功耗模式
}

/*
*********************************************************************************************************
*	函 数 名: RtcWakeUpConfig
*	功能说明: 休眠时间设置(1s)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/ 
void RtcWakeUpConfig(uint32_t m)
{    
    PWR_RTCAccessCmd(ENABLE);           // 打开配置寄存器权限
//-----------------------------------------------------//    
    RTC_WakeUpCmd(DISABLE);                                             // 先禁止，才能设置
    RTC_ClearFlag(RTC_FLAG_WUTF);
    
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);              // 1hz
    RTC_SetWakeUpCounter(m);                                            // 间隔
    
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line20);                                // 此语句千万不能少                
    
    RTC_ITConfig(RTC_IT_WUT,ENABLE);                                    //需要使能中断，不需要中断函数    
    RTC_WakeUpCmd(ENABLE);
//-----------------------------------------------------//    
    PWR_RTCAccessCmd(DISABLE);          // 一定要禁止，否则无法进入低功耗模式
}


void RTC_AlartSet(void)
{
    RTC_AlarmTypeDef    RTC_AlarmStructure; 
    PWR_RTCAccessCmd(ENABLE);           // 打开配置寄存器权限
    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);     
#if 1
    // 纠正闹钟设置
    if(gstuWmPara.mu1Tr_Ho > 23) {
      gstuWmPara.mu1Tr_Ho = 23;
    }  
    if(gstuWmPara.mu1Tr_Mi > 59) {
      gstuWmPara.mu1Tr_Mi = 59;
    }    
    if(gstuWmPara.mu1Tr_Se > 59) {
      gstuWmPara.mu1Tr_Se = 59;
    }
    //根据参数设置闹钟 
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
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel  = RTC_AlarmDateWeekDaySel_Date; // 按日期    
    
    RTC_AlarmStructure.RTC_AlarmMask    = RTC_AlarmMask_DateWeekDay;            // 日期不考虑匹配
#if 1
    RTC_AlarmStructure.RTC_AlarmMask    |= RTC_AlarmMask_Hours;                 // 小时不考虑匹配
#endif
     
#if 0
	RTC_AlarmStructure.RTC_AlarmMask    |= RTC_AlarmMask_Hours;                 // 小时不考虑匹配
    RTC_AlarmStructure.RTC_AlarmMask    |= RTC_AlarmMask_Minutes;               // 分钟不考虑匹配           
    RTC_AlarmStructure.RTC_AlarmMask    |= RTC_AlarmMask_Seconds;               // 秒钟不考虑匹配 
#endif    
    RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure);               // 按bin格式设置
     
    //RTC_ClearFlag(RTC_FLAG_ALRAF);        // 清除RTC闹钟标志
    RTC_ClearITPendingBit(RTC_IT_ALRA);     // 清除RTC闹钟A的中断标志
    EXTI_ClearITPendingBit(EXTI_Line17);    // 清除LINE17上的中断标志位 
        
    RTC_ITConfig(RTC_IT_ALRA,ENABLE);       // 使能中断
    RTC_AlarmCmd(RTC_Alarm_A,ENABLE);       // 使能设备
    
    PWR_RTCAccessCmd(DISABLE);              // 一定要禁止，否则无法进入低功耗模式
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
        if(temp >= 250) return 1;//初始化时钟失败,晶振有问题，32768晶振起振极限时间1-5S 
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
    RTC_TimeStructure.RTC_Hours         = 0x00;         // 0点
    RTC_TimeStructure.RTC_Minutes       = 0x00;         // 0分
    RTC_TimeStructure.RTC_Seconds       = 0x00;         // 0秒
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
    //----------------------------------------------------------------//        
    return 0;
}

/*
*********************************************************************************************************
*	函 数 名: RtcInit
*	功能说明: 查询后备寄存器，进行RTC初始化
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t RtcInit(void)
{
    //检查是不是第一次配置时钟
    uint8_t flag = 0;
    //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
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
*	函 数 名: CorrectRtc
*	功能说明: 计算闰年
*	形    参：年数值
*	返 回 值: 无
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
*	函 数 名: CorrectRtc
*	功能说明: 根据格林尼治时间，修正北京时间
*	形    参：rtc时钟
*	返 回 值: 无
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
    
    uint8_t     u1Carry = 0;    // 进位标志
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
        u1Carry = 1;            // 日期进位
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
        // 月份必须为1~12    
        if(u1month < 1) {
            u1month = 1;
        }
        else if(u1month > 12) {          
            u1month = 12;
        }
            
        u2year = (uint16_t)(2000 + u1year);     // 读年份        
        if(Is_Leep_Year(u2year)) {           
            pch = (uint8_t *)LEEP_DAYCOUNT;     // 闰年       
        }
        else {   
            pch = (uint8_t *)COMM_DAYCOUNT;     // 平年
        }
                                 
        if(u1day < pch[u1month - 1])
        {
            u1day += 1;
            u1Carry = 0;            
        }
        else              
        {
            u1day   = 1;        // 从1号开始                
            u1Carry = 1;        // 月度进位    
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
                u1year += 1;    // 年度进位            
            }        
        }   
    } 
    
    mRtcTime.year   = u1year;
    mRtcTime.month  = u1month;
    mRtcTime.day    = u1day;

    return mRtcTime;
}




/*
调试用探索板RTC的时候，设置RTC的日期和时间，发现日期一直不生效，只有系统重启才能生效。
摸索出来的解决方法是：只需要在设置日期时间后，立即读取一次日期时间，且获取日期的函数一定要放在获取时间函数后面就能解决。
*/

static uint8_t aShowTime[50] = {0};
static uint8_t aShowDate[50] = {0};

/*
*********************************************************************************************************
*	函 数 名: ReadRtcTime
*	功能说明: 显示时间
*	形    参：无
*	返 回 值: 返回当天的秒计数值
*********************************************************************************************************
*/ 

uint32_t ReadRtcTime(TTime   *pRtc)
{
    uint32_t    u4tmp = 0;
    
    RTC_DateTypeDef  RTC_DateStructure;
    RTC_TimeTypeDef  RTC_TimeStructure;                
  	     
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);            // 先获取时间
    pRtc->hours    =  RTC_TimeStructure.RTC_Hours;
    pRtc->minutes  =  RTC_TimeStructure.RTC_Minutes;
    pRtc->seconds  =  RTC_TimeStructure.RTC_Seconds;       
   
  
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);            // 再获取日期    
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
*	函 数 名: RTC_TimeShow
*	功能说明: 显示时间
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/ 
void RTC_TimeShow(void)
{
    RTC_TimeTypeDef  RTC_TimeStructure;

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	/* 时间显示格式 : hh:mm:ss */	
	sprintf((char*)aShowTime,"%0.2d:%0.2d:%0.2d",	RTC_TimeStructure.RTC_Hours, 
													RTC_TimeStructure.RTC_Minutes, 
													RTC_TimeStructure.RTC_Seconds);	
	printf("%s\r\n", aShowTime);	
}

/*
********************************************************************************
*	函 数 名: RTC_DateShow
*	功能说明: 显示时间
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: GetWeekDay
*	功能说明: 计算星期几
*	形    参：无
*	返 回 值: 无
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
    else    /* 某年的1、2月要看作上一年的13、14月来计算 */
    {
        m += 12;
        y -= 1;
    }
    
    // 得到1 ~ 7，对应周一到周日
    uWeek = 1 + (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;   
          
    return (uint8_t)uWeek;
}

/*
*********************************************************************************************************
*	函 数 名: Set_Time
*	功能说明: 设置时间
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/ 
void Set_Time(TTime Rtc)
{
    TTime       tTm;
                  
    RTC_DateTypeDef  RTC_DateStructure;
    RTC_TimeTypeDef  RTC_TimeStructure;
        
//-----------------------------------//            
    tTm = CorrectRtc(Rtc);                      // 比伦敦时间早8小时
//-----------------------------------//
    PWR_RTCAccessCmd(ENABLE);                   //使能备份寄存器操作                  
    /* 设置时分秒，以及显示格式 */
    //RTC_TimeStructure.RTC_H12       = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours     = tTm.hours;
    RTC_TimeStructure.RTC_Minutes   = tTm.minutes;
    RTC_TimeStructure.RTC_Seconds   = tTm.seconds; 
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);   
    
    /* 设置年月日和星期 */
    RTC_DateStructure.RTC_Year      = tTm.year;
    RTC_DateStructure.RTC_Month     = tTm.month;
    RTC_DateStructure.RTC_Date 	    = tTm.day;
    RTC_DateStructure.RTC_WeekDay   = GetWeekDay(tTm);     // 必须正确设置WeekDay    
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);	
    
    PWR_RTCAccessCmd(DISABLE);     
 //-----------------------------------//    
    // 设置完时钟后，再读一次
    ReadRtcTime(&gucRtcTime);
}

/*******************************(END OF FILE)**********************************/

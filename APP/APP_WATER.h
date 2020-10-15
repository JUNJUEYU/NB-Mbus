/*
*******************************************************************************
*
*                           the app_water file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_water.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "includes.h"


#define WMTRG_DLT           10UL            // 水表触发秒间隔


STRWATER                gstuLrDat;          // lora发送的水表数据结构 

__IO static uint32_t    m4RtcTrgBak1 = 0;
__IO static uint32_t    m4RtcTrgBak2 = 0;

/*
16进制转BCD码
*/
uint32_t HexToBcd(uint32_t v)
{
    uint32_t u4ret = 0;
    uint32_t u4tmp = 0;
      
    u4tmp = (v % 1000000);
    u4ret |= u4tmp / 100000;
    u4ret <<= 4;
    
    u4tmp %= 100000;
    u4ret |= u4tmp / 10000;
    u4ret <<= 4;      
    
    u4tmp %= 10000;
    u4ret |= u4tmp / 1000;
    u4ret <<= 4;   
    
    u4tmp %= 1000;
    u4ret |= u4tmp / 100;
    u4ret <<= 4;
    
    u4tmp %= 100;
    u4ret |= u4tmp / 10;
    u4ret <<= 4; 
    
    u4tmp %= 10;
    u4ret |= u4tmp;    
      
    return u4ret;
}

/*
BCD码转16进制
*/
uint32_t BcdToHex(uint32_t v)
{
    uint32_t u4ret = 0;
    uint32_t u4tmp = 0;
    
    u4tmp = v;    
    u4ret = u4tmp & (uint32_t)0x000f;
    
    u4tmp >>= 4;
    if(u4tmp > 0) {
      u4ret += (u4tmp & (uint32_t)0x000f) * 10;
    }
    
    u4tmp >>= 4;
    if(u4tmp > 0) {
      u4ret += (u4tmp & (uint32_t)0x000f) * 100;
    }
        
    u4tmp >>= 4;
    if(u4tmp > 0) {
      u4ret += (u4tmp & (uint32_t)0x000f) * 1000;
    }
        
    u4tmp >>= 4;
    if(u4tmp > 0) {
      u4ret += (u4tmp & (uint32_t)0x000f) * 10000;
    }
    
    u4tmp >>= 4;
    if(u4tmp > 0) {
      u4ret += (u4tmp & (uint32_t)0x000f) * 100000;
    }
        
    u4tmp >>= 4;
    if(u4tmp > 0) {
      u4ret += (u4tmp & (uint32_t)0x000f) * 1000000;
    }
        
    u4tmp >>= 4;
    if(u4tmp > 0) {
      u4ret += (u4tmp & (uint32_t)0x000f) * 10000000;
    }
    
    return u4ret;
}


/*
直读水表的累加和计算
*/
uint8_t LoraSum(uint8_t *pch,uint16_t n)
{
    uint16_t    i;
    uint8_t     sum = 0;
    
    for(i = 0; i < n; i++)
      sum += pch[i];
   
    return sum;  
}



/*
判断当前时间与上次触发时间差
*/
uint32_t AbsSecond(uint32_t val1,   uint32_t val2)
{                
    if(val1 < val2) 
    {    
        val1 += 86400;     // 1天内秒计数
    }               
    return (val1 - val2);            
}
/*
水表脉冲计数处理
*/
void WaterPulse(void)
{      
    TTime       mRtc;
    uint32_t    u4tmp  = 0;
    uint32_t    u4cnt1 = 0;
    uint32_t    u4cnt2 = 0;
    
    uint16_t    i;   
      
    To_Enter_Wait(100);                   // 100ms等待       
    
    for(i = 0; i < 6; i++) 
    {   
        To_Enter_Wait(25);                  // 25ms等待        
        
        if(gstuFlag.mbPulseF1)              // 脉冲中断标志1
        {
            //if(GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_10 ) == 0) 
            if(GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_10 ) > 0) 
            {            
                u4cnt1 = 0;     
            }
            else {              
                u4cnt1 += 1; 
            }              
        }              
        
        if(gstuFlag.mbPulseF2)              // 脉冲中断标志2
        {
            //if(GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_11 ) == 0) 
            if(GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_11 ) > 0) 
            {            
                u4cnt2 = 0;     
            }
            else {  
                u4cnt2 += 1;
            }
        }
    }     
    //----------------------------------------------// 
    u4tmp = ReadRtcTime(&mRtc);                 // 当天的秒计数
    
    if(u4cnt1 > 3) 
    {                                     
        // 脉冲间隔大于10s
        if(AbsSecond(u4tmp,m4RtcTrgBak1) > WMTRG_DLT) 
        {
            m4RtcTrgBak1 = u4tmp;               // 备份秒数
            gstuWmTot.mu4Pulse1 += 1;           // 1#脉冲计数  
        }     
    }  
    
    if(u4cnt2 > 3) 
    {
        // 脉冲间隔大于10s
        if(AbsSecond(u4tmp,m4RtcTrgBak2) > WMTRG_DLT) 
        {
            m4RtcTrgBak2 = u4tmp;               // 备份秒数
            gstuWmTot.mu4Pulse2 += 1;           // 2#脉冲计数                
        }
    }         
    //----------------------------------------------// 
    if(gstuWmTot.mu4Pulse1 <= gstuWmTot.mu4Pulse2) 
    {
        u4tmp = gstuWmTot.mu4Pulse1;
    }
    else 
    {
        u4tmp = gstuWmTot.mu4Pulse2;
    }     
    //----------------------------------------------// 
    if(u4tmp >= gstuWmTot.mu4TotVal + 100)      // 100个脉冲，即1吨
    {
          gstuWmTot.mu4TotVal = u4tmp;       
          SaveTotFlow(&gstuWmTot);              // 保存到EEPROM中          
    }
    //----------------------------------------------//                
}

/*
数据打包，通过LORA发送给集中器
*/
void FillLoraData(void)
{   
    TTime       mRtc;
    uint32_t    u4tmp,u4dlt; 
    //----------------------------------------------//                  
    gstuLrDat.mu1Head1      = LORA_HEAD1;               // 0x68
    gstuLrDat.mu1Head2      = LORA_HEAD2;               // 0x10
    //----------------------------------------------//  
    gstuLrDat.mu4WaterNum   = gstuWmPara.mu4WaterNum;
    gstuLrDat.mu4DtuId      = gstuWmPara.mu4DtuId;   
    //----------------------------------------------//
    gstuLrDat.mu1ComCmd     = 0x81;			            // 控制码			
    gstuLrDat.mu1Lenth      = 0x16;			            // lenth 	
    //----------------------------------------------// 
    gstuLrDat.mu2Func       = 0x901f; 		            // 功能码        
    //----------------------------------------------// 
    gstuLrDat.mu1Ser += 1;                              // 序列号					        
    //----------------------------------------------//        
    u4tmp = BcdToHex(gstuWmPara.mu4InitVal);            // 水表初值
    u4tmp += gstuWmTot.mu4TotVal;                       // 总的流量值     
    u4tmp /= 100;                                       // 取整数(小数点占2位)
    gstuLrDat.mu4TotFlow    = HexToBcd(u4tmp);          // 水表指针
    gstuLrDat.mu1Unit0      = 0x2c;                     // 单位
    //----------------------------------------------// 
    u4tmp = gstuWmTot.mu4TotVal;                        // 累计流量
    u4tmp /= 100;                                       // 取整数
    gstuLrDat.mu4CurFlow    = HexToBcd(u4tmp);          // 当前流量（每次上报清零后的流量）
    gstuLrDat.mu1Unit1      = 0x2c;                     // 单位
    //----------------------------------------------//    
    ReadRtcTime(&mRtc);
         
    gstuLrDat.mu1Rtc[0]     = mRtc.year;  
    gstuLrDat.mu1Rtc[1]     = mRtc.month;  
    gstuLrDat.mu1Rtc[2]     = mRtc.day;  
    gstuLrDat.mu1Rtc[3]     = mRtc.hours;  
    gstuLrDat.mu1Rtc[4]     = mRtc.minutes;  
    gstuLrDat.mu1Rtc[5]     = mRtc.seconds;  
    //----------------------------------------------//
    if(gu4Pulse1 >= gu4Pulse2) {
        u4dlt = (gu4Pulse1 - gu4Pulse2);
    }
    else {
        u4dlt = (gu4Pulse2 - gu4Pulse1); 
    }     
    u4tmp = HexToBcd(u4dlt);                    // 脉冲差值       
    gstuLrDat.mu1State[0]   = (uint8_t)(u4tmp & 0x00ff); 
    u4tmp >>= 8;
    gstuLrDat.mu1State[1]   = (uint8_t)(u4tmp & 0x00ff); 
    u4tmp >>= 8;
    gstuLrDat.mu1State[2]   = (uint8_t)(u4tmp & 0x00ff);
    //----------------------------------------------//		
    gstuLrDat.mu1Check      = LoraSum((uint8_t *)&gstuLrDat,sizeof(STRWATER)-2); 	// sum 	
    gstuLrDat.mu1Tail       = LORA_TAIL; 	        // 尾                            
    //----------------------------------------------//	           
}



/*******************************(END OF FILE)**********************************/
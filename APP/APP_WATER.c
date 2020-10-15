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


#define WMTRG_DLT           10UL            // ˮ��������


STRWATER                gstuLrDat;          // lora���͵�ˮ�����ݽṹ 

__IO static uint32_t    m4RtcTrgBak1 = 0;
__IO static uint32_t    m4RtcTrgBak2 = 0;

/*
16����תBCD��
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
BCD��ת16����
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
ֱ��ˮ����ۼӺͼ���
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
�жϵ�ǰʱ�����ϴδ���ʱ���
*/
uint32_t AbsSecond(uint32_t val1,   uint32_t val2)
{                
    if(val1 < val2) 
    {    
        val1 += 86400;     // 1���������
    }               
    return (val1 - val2);            
}
/*
ˮ�������������
*/
void WaterPulse(void)
{      
    TTime       mRtc;
    uint32_t    u4tmp  = 0;
    uint32_t    u4cnt1 = 0;
    uint32_t    u4cnt2 = 0;
    
    uint16_t    i;   
      
    To_Enter_Wait(100);                   // 100ms�ȴ�       
    
    for(i = 0; i < 6; i++) 
    {   
        To_Enter_Wait(25);                  // 25ms�ȴ�        
        
        if(gstuFlag.mbPulseF1)              // �����жϱ�־1
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
        
        if(gstuFlag.mbPulseF2)              // �����жϱ�־2
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
    u4tmp = ReadRtcTime(&mRtc);                 // ����������
    
    if(u4cnt1 > 3) 
    {                                     
        // ����������10s
        if(AbsSecond(u4tmp,m4RtcTrgBak1) > WMTRG_DLT) 
        {
            m4RtcTrgBak1 = u4tmp;               // ��������
            gstuWmTot.mu4Pulse1 += 1;           // 1#�������  
        }     
    }  
    
    if(u4cnt2 > 3) 
    {
        // ����������10s
        if(AbsSecond(u4tmp,m4RtcTrgBak2) > WMTRG_DLT) 
        {
            m4RtcTrgBak2 = u4tmp;               // ��������
            gstuWmTot.mu4Pulse2 += 1;           // 2#�������                
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
    if(u4tmp >= gstuWmTot.mu4TotVal + 100)      // 100�����壬��1��
    {
          gstuWmTot.mu4TotVal = u4tmp;       
          SaveTotFlow(&gstuWmTot);              // ���浽EEPROM��          
    }
    //----------------------------------------------//                
}

/*
���ݴ����ͨ��LORA���͸�������
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
    gstuLrDat.mu1ComCmd     = 0x81;			            // ������			
    gstuLrDat.mu1Lenth      = 0x16;			            // lenth 	
    //----------------------------------------------// 
    gstuLrDat.mu2Func       = 0x901f; 		            // ������        
    //----------------------------------------------// 
    gstuLrDat.mu1Ser += 1;                              // ���к�					        
    //----------------------------------------------//        
    u4tmp = BcdToHex(gstuWmPara.mu4InitVal);            // ˮ���ֵ
    u4tmp += gstuWmTot.mu4TotVal;                       // �ܵ�����ֵ     
    u4tmp /= 100;                                       // ȡ����(С����ռ2λ)
    gstuLrDat.mu4TotFlow    = HexToBcd(u4tmp);          // ˮ��ָ��
    gstuLrDat.mu1Unit0      = 0x2c;                     // ��λ
    //----------------------------------------------// 
    u4tmp = gstuWmTot.mu4TotVal;                        // �ۼ�����
    u4tmp /= 100;                                       // ȡ����
    gstuLrDat.mu4CurFlow    = HexToBcd(u4tmp);          // ��ǰ������ÿ���ϱ�������������
    gstuLrDat.mu1Unit1      = 0x2c;                     // ��λ
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
    u4tmp = HexToBcd(u4dlt);                    // �����ֵ       
    gstuLrDat.mu1State[0]   = (uint8_t)(u4tmp & 0x00ff); 
    u4tmp >>= 8;
    gstuLrDat.mu1State[1]   = (uint8_t)(u4tmp & 0x00ff); 
    u4tmp >>= 8;
    gstuLrDat.mu1State[2]   = (uint8_t)(u4tmp & 0x00ff);
    //----------------------------------------------//		
    gstuLrDat.mu1Check      = LoraSum((uint8_t *)&gstuLrDat,sizeof(STRWATER)-2); 	// sum 	
    gstuLrDat.mu1Tail       = LORA_TAIL; 	        // β                            
    //----------------------------------------------//	           
}



/*******************************(END OF FILE)**********************************/
/*
*******************************************************************************
*
*                           the bsp_adc file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : bsp_adc.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "bsp.h"

//uint16_t batVoltage = 0;		//单位mV
#define     ADCREFVAL               (3286) 
uint8_t     gucPowPer = 0;
/*
引脚初始化程序：
*/
void ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;  		//PB1:  ADC1_通道9
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/*
ADC初始化程序：
*/
void bsp_Adc_Init(void)
{
    ADC_InitTypeDef         ADC_InitStructure;
    ADC_CommonInitTypeDef   ADC_CommonInitStructure;
           
    //------------------------------//
    RCC_HSICmd(ENABLE);     // ADC必须使用HSI
    /* Check that HSI oscillator is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
    //------------------------------//         
    ADC_GPIO_Config();    
    //------------------------------//     
    /* ADC1 DeInit */  
    ADC_DeInit(ADC1);    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    //------------------------------//     
    ADC_CommonInitStructure.ADC_Prescaler       = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);
    //------------------------------//     
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode          = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode    = DISABLE;                      //非连续模式(单次)
    ADC_InitStructure.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None;//有软件控制转换
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;          //转换数据右对齐
    ADC_InitStructure.ADC_NbrOfConversion       = 1;                            //转换通道为通道1
    ADC_Init(ADC1, &ADC_InitStructure);         //初始化ADC
   
    ADC_RegularChannelConfig(ADC1,  ADC_Channel_9,    1,  ADC_SampleTime_384Cycles);
        
    /* Enables the ADC1 Power Down during Delay */ 
    ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
    /* Delay until the converted data has been read */
    ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);
    /* Enable ADC1 */
    ADC_Cmd(ADC1,ENABLE);
       
    /* Wait until the ADC1 is ready */
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
    {
    }
    
    //ADC_SoftwareStartConv(ADC1);      // 开始转换
}

#define              ADCBUFCNT              4
static  uint16_t    guAdcVal[ADCBUFCNT] = {0};

//uint16_t GetBatVal(void)
//{
//    uint16_t    i = 0;
//    uint32_t    u4AdcVal = 0;
//      
//    for(i = 0; i < ADCBUFCNT; i++) 
//    {
//        ADC_SoftwareStartConv(ADC1);    // 开始转换
//        Delay_ms(1);
//        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束            
//        guAdcVal[i] = ADC_GetConversionValue(ADC1);          
//        u4AdcVal += guAdcVal[i];
//    }   
//    
//    u4AdcVal *= (3000);
//    u4AdcVal += (1024*ADCBUFCNT);
//    u4AdcVal /= (2048*ADCBUFCNT);               
//    
//    return (uint16_t)u4AdcVal;
//}

uint16_t GetBatVal(void)
{
    uint16_t    i = 0;
    uint32_t    u4AdcVal = 0;
      
    for(i = 0; i < ADCBUFCNT; i++) 
    {
        ADC_SoftwareStartConv(ADC1);
        Delay_ms(2);
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//??????            
        guAdcVal[i] = ADC_GetConversionValue(ADC1);          
        u4AdcVal += guAdcVal[i];
    }   
    u4AdcVal /= ADCBUFCNT;      // ???
    u4AdcVal *= (ADCREFVAL);    // ????
    u4AdcVal += 2048;
    u4AdcVal /= 4096;    
    return (uint16_t)u4AdcVal;
}

  
#define              BATMAXCNT              8
//static  uint16_t    gu2BatCnt            = 0;
static  uint16_t    gu2BatVol[BATMAXCNT] = {0};

/*
得到电池百分比
*/
//uint8_t CalBatPer(void)
//{
//    uint16_t    i = 0;
//    uint32_t    u4AdcVal = 0;
//        
//    for(i = 0; i < (BATMAXCNT-1); i++) 
//    {            
//        gu2BatVol[i] = gu2BatVol[i + 1];
//        u4AdcVal += gu2BatVol[i];
//    }        
//    // 采样电池电压
//    gu2BatVol[BATMAXCNT-1] = GetBatVal();    
//    u4AdcVal += gu2BatVol[BATMAXCNT-1];
//    //----------------------------------//    
//    // 简单计算百分比 
//    u4AdcVal /= BATMAXCNT;       
//#if 0	
//    if(u4AdcVal  < 1000) {
//      u4AdcVal = 1000;
//    }
//    else if(u4AdcVal  > 1400) {
//        u4AdcVal = 1400;
//    }         
//    u4AdcVal -= 1000;
//    u4AdcVal /= 4;
//#endif
//    //----------------------------------//           
//#if 0
//    gu2BatCnt++;
//    gu2BatCnt &= 0x0f;
//    if(gu2BatCnt == 0) {
//        printf("Bat Percent is: %d Per.\r\n",u4AdcVal);
//    }
//#endif  
//    return (uint8_t)u4AdcVal;       // 百分比   
//}    
       
uint8_t CalBatPer(void)
{
    uint16_t    i = 0;
    uint32_t    u4AdcVal = 0;
#if 1     
    //----------------------------------//    
    for(i = 0; i < (BATMAXCNT-1); i++) 
    {            
        gu2BatVol[i] = gu2BatVol[i + 1];
        u4AdcVal += gu2BatVol[i];
    }        
    // 采样电池电压
    gu2BatVol[BATMAXCNT-1] = GetBatVal();    
    u4AdcVal += gu2BatVol[BATMAXCNT-1];  
    //----------------------------------//    
    u4AdcVal /= BATMAXCNT;     
#endif    
    //--------------------//    
#if 1 
    // 计算百分比 采样电阻分压范围2.0-2.4v    
    if(u4AdcVal  < 2000) {
      u4AdcVal = 2000;
    }
    else if(u4AdcVal  > 2400) {
        u4AdcVal = 2400;
    }         
    u4AdcVal -= 2000;
    u4AdcVal /= 4;
    
    gucPowPer = (uint8_t)u4AdcVal;
#else   
    u4AdcVal = GetBatVal();   
    u4AdcVal *= 3;
#endif        
    //----------------------------------//           

    //----------------------------------// 
    return gucPowPer;       // ???   
} 





/*******************************(END OF FILE)**********************************/

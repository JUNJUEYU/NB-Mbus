#ifndef __APP_EEPROM_H
#define __APP_EEPROM_H

#include "stm32l1xx.h"



#define DEF_WM_NUM              (190923001UL) 
#define DEF_DTU_ID              (9200UL)
#define DEF_TR_CNT              (6)             // 间隔6小时，通过NB上传一次


/*
水表累计流量
*/
#pragma pack (1)					                    // 字节对齐		
typedef struct                                        // 单位0.01
{                        
      uint32_t  mu4TotVal;                              // 累计水量 
      uint32_t  mu4Pulse1;                              // 1#脉冲值
      uint32_t  mu4Pulse2;                              // 2#脉冲值
      //-------------------------------------------------------//  8 bytes   
      uint8_t   mu1Rsv[2];                              // 保留字节
      uint8_t   mu1Check;                               // 校验           
      uint8_t   mu1Mark;                                // 标注(放在最后，才能说明flash写完了)
      //-------------------------------------------------------//  16 bytes   
}STRWMTOT;
#pragma pack ()




/*
水表参数设置
*/
#pragma pack (1)					// 字节对齐		
typedef struct 
{             
    uint8_t   mu1Rf_Chn;                              // 信道:1~40      
    uint8_t   mu1Rf_Frq;                              // 频段:1~4
    uint8_t   mu1Rf_Bps;                              // 速率:0~9
    uint8_t   mu1Rf_Pow;                              // 功率:0~7
    //-------------------------------------------------------//       
    uint32_t  mu4InitVal;                             // 初始水量      
    uint32_t  mu4Net_Id;                              // 网络ID     
    //-------------------------------------------------------//  12 bytes
    uint32_t  mu4DtuId;                               // dtu_id  
    uint32_t  mu4WaterNum;                            // 9位水表序号（占用4个字节）      

    uint8_t   mu1Tr_Cnt;                              // 传输时钟间隔计数
    uint8_t   mu1Tr_Ho;                               // 传输时间(时钟)
    uint8_t   mu1Tr_Mi;                               // 传输时间(分钟)
    uint8_t   mu1Tr_Se;                               // 传输时间(秒钟)
    //-------------------------------------------------------//  24 bytes
    uint32_t    mu4Rsv[4];                              // 保留
    //-------------------------------------------------------//  40bytes 
    uint32_t  mu4TotVal;                              // 水表指针流量             // 只读参数
    uint32_t  mu4Pulse1;                              // 1#脉冲值
    uint32_t  mu4Pulse2;                              // 2#脉冲值
    //-------------------------------------------------------//
    uint8_t   mu1Rsv[2];                              // 保留字节
    uint8_t   mu1Check;                               // 校验           
    uint8_t   mu1Mark;                                // 标注(放在最后，才能说明flash写完了)
    //-------------------------------------------------------//  56 bytes   
}STRWMPARA;
#pragma pack ()



extern uint32_t                 gu4Pulse1;
extern uint32_t                 gu4Pulse2;

extern STRWMTOT                 gstuWmTot;              // 累计流量
extern STRWMPARA                gstuWmPara;             // 水表参数
        
void InitUserData(void);
void ClearTotFlow(void);

void SaveWMPara(STRWMPARA *pstu);
void SaveTotFlow(STRWMTOT *pstu);       

uint8_t CheckSum(uint8_t *pch,uint16_t n);

#endif


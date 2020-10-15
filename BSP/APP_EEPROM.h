#ifndef __APP_EEPROM_H
#define __APP_EEPROM_H

#include "stm32l1xx.h"


#define     DEF_TR_CNT              (24)        // ??x??,??NB????
#define     DEF_DEV_ID              (1)         // RS485_????

#define     IP_DEF_0                (192)
#define     IP_DEF_1                (168)
#define     IP_DEF_2                (110)
#define     IP_DEF_3                (60)

#if 0
#define     DEF_DTU_ID              (9200)      // ???
#define     PORT_DEF                (8112)      // ????
#else
#define     DEF_WM_NUM              (180808001) // ???
#define     PORT_DEF                (8111)      // ????
#endif
#define     MARK_CODE               0x55
//----------------------------------------------------------------------------//
#define     ADDR_START              (0)         // ????

#define     ADDR_PAGE0              (ADDR_START + 0x00)     
#define     ADDR_PAGE4              (ADDR_START + 0x400)

#define     ADDR_PAGE1              (ADDR_START + 0x100)     
#define     ADDR_PAGE5              (ADDR_START + 0x500)

#define     ADDR_PAGE2              (ADDR_START + 0x200)     
#define     ADDR_PAGE6              (ADDR_START + 0x600)

#define     ADDR_PAGE3              (ADDR_START + 0x300)
#define     ADDR_PAGE7              (ADDR_START + 0x700)     
//----------------------------------------------------------------------------//    
#define     RCRD_ADR2               (2*1024)     
#define     RCRD_ADR3               (3*1024)     
#define     RCRD_ADR4               (4*1024)     
#define     RCRD_ADR5               (5*1024)     
#define     RCRD_ADR6               (6*1024)     
#define     RCRD_ADR7               (7*1024)  
//----------------------------------------------------------------------------//

/*
??????
*/
#pragma pack (1)					                    // ????		
typedef struct                                        // ??0.01
{                        
      uint32_t  mu4TotVal;                              // ???? 
      uint32_t  mu4Pulse1;                              // 1#???
      uint32_t  mu4Pulse2;                              // 2#???
      //---------------------------------------------   ----------//  8 bytes   
      uint8_t   mu1Err;                                 // ????
      uint8_t   mu1Rsv;                                 // ????
      uint8_t   mu1Check;                               // ??           
      uint8_t   mu1Mark;                                // ??(????,????flash???)
      //-------------------------------------------------------//  16 bytes   
}STRWMTOT;
#pragma pack ()




/*
??????
*/
#pragma pack (1)					                    // 字节对齐		
typedef struct 
{       
    //-------------------------------------------------------// 		
    uint8_t   mu1Tr_Cnt;                               // 传输时钟间隔计数
    uint8_t   mu1Tr_Ho;                                // 传输时间（hour）
    uint8_t   mu1Tr_Mi;                                // 传输时间（min）
    uint8_t   mu1Tr_Se;                               // 传输时间（sec）
	//-------------------------------------------------------// 	
	uint16_t  mu2DtuID;                              //DTUID
    uint16_t  mu2CjbID;                              //CJBID
    //-------------------------------------------------------//      
    uint8_t	  IP_ADDR[4];		                   // ip         
    uint16_t  mu2Port;                                // 端口号
#if 0	
    //-------------------------------------------------------//    
    uint8_t   mu1FlowAddr[70];					   //水表地址
	//-------------------------------------------------------// 
#endif
	uint8_t   mu1TxMax;                             //最大传输次数
	//-------------------------------------------------------// 
	uint8_t   mu1Check;                              //累加和
    //-------------------------------------------------------//    
	uint8_t   mu1Mark;								   //完成标志
}STRWMPARA;
#pragma pack ()                                      //默认对齐



extern STRWMTOT                 gstuWmTot;              // ????
extern STRWMPARA                gstuWmPara;             // ????

extern  uint8_t                 isEEPRomOK;
        
void InitUserConfig(void);
void ClearTotFlow(void);


uint32_t EepWr(uint32_t addr, uint8_t *pch, uint16_t n);
uint32_t EepRd(uint32_t addr, uint8_t *pch, uint16_t n);

void TestErom(void);

void SaveWMPara(STRWMPARA *pstu);
void SaveTotFlow(STRWMTOT *pstu);       

uint8_t CheckSum(uint8_t *pch,uint16_t n);

#endif


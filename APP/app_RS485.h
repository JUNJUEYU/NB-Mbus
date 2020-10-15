
/*
*******************************************************************************
*
*                           the app_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_uart3.h
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include<stdint.h>
#ifndef APP_RS485_H_
#define APP_RS485_H_
/*typedef struct
{
    uint16_t    u2Reg09;            // 正 累计流量（低16bit)   
    uint16_t    u2Reg10;            // 正 累计流量（高16bit)   
    uint16_t    u2Reg11;            // 正 累计流量的小数部分（低16bit)
    uint16_t    u2Reg12;            // 正 累计流量的小数部分（高16bit)
    
    uint16_t    u2Reg13;            // 负 累计流量（低16bit)
    uint16_t    u2Reg14;            // 负 累计流量（高16bit)
    uint16_t    u2Reg15;            // 负 累计流量的小数部分（低16bit)
    uint16_t    u2Reg16;            // 负 累计流量的小数部分（高16bit)
    
    uint16_t    u2Reg[8];           // 热量值(不需要 )
   
    uint16_t    u2Reg25;		    // 净累计流量（低16bit): 第25个寄存器,地址:0x18
    uint16_t    u2Reg26;	        // 净累计流量（高16bit): 第26个寄存器,地址:0x19
    
	uint16_t    u2Reg_Rsv[107];     // 107 = (125 - 18)个寄存器
}STUSERCOMDATA;                     // 总共125个寄存器  */
// modbus读返回 寄存器定义
typedef struct
{
    uint16_t    flow_addr[10][7];            // 水表地址
    uint16_t    flow_data[10][5];            // 水表数据 
    uint16_t    flow_date[7];                // 抄表日期
    uint16_t    flow_cnt ;                    // 抄表计数器
    uint16_t    flow_max ;                    // 最大水表号
    
}STUSERCOMDATA;                     // 总共125个寄存器       	



#define     MEMBER_MAX             10     // 成员总数
#define     MEMBER_SIZE            12      // 单个成员占用字节数
// nb上传的流量数据
typedef struct
{  
    uint8_t     mu1Apart[4];
    uint32_t    mu4StuCnt;          // 结构体总长度     
//----------------------------------//     
    uint32_t    mu4WaterNum;
    uint16_t    mu2DtuID;
	uint16_t    mu2CjbID;
           
    uint8_t     mu1Rtc[6];
    uint8_t    mu1Csq;
    
    uint8_t    mu1Err;    
    uint8_t    mu1MemCnt;  
	uint8_t    mu1MemLen;  
	uint8_t    mu1MemBeg; 
	uint8_t    mu1MemTyp; 
    
    uint8_t    mu1AddrNData[120]; 
    uint8_t     mu1Hold[8];      // ??    
	uint32_t    mu4CltCnt;
	uint8_t    mu1DevTyp;
	uint8_t    mu1DevBat;
    uint8_t     mu1Check;           
    uint8_t     mu1Tail;         
}STUNBFLOWDATA;                     // 通过NB上传的流量数据

extern STUNBFLOWDATA    gstuNbFlowData;   
extern STUSERCOMDATA    gstuMbusFlowData; 

void InitFlowData(void);
void FillFlowTail(STUNBFLOWDATA *pstu);
void FillFlowData(STUNBFLOWDATA *pstu);

uint8_t FlowConnect(void);
void ProcUart3Rec(void);

#endif



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
    uint16_t    u2Reg09;            // �� �ۼ���������16bit)   
    uint16_t    u2Reg10;            // �� �ۼ���������16bit)   
    uint16_t    u2Reg11;            // �� �ۼ�������С�����֣���16bit)
    uint16_t    u2Reg12;            // �� �ۼ�������С�����֣���16bit)
    
    uint16_t    u2Reg13;            // �� �ۼ���������16bit)
    uint16_t    u2Reg14;            // �� �ۼ���������16bit)
    uint16_t    u2Reg15;            // �� �ۼ�������С�����֣���16bit)
    uint16_t    u2Reg16;            // �� �ۼ�������С�����֣���16bit)
    
    uint16_t    u2Reg[8];           // ����ֵ(����Ҫ )
   
    uint16_t    u2Reg25;		    // ���ۼ���������16bit): ��25���Ĵ���,��ַ:0x18
    uint16_t    u2Reg26;	        // ���ۼ���������16bit): ��26���Ĵ���,��ַ:0x19
    
	uint16_t    u2Reg_Rsv[107];     // 107 = (125 - 18)���Ĵ���
}STUSERCOMDATA;                     // �ܹ�125���Ĵ���  */
// modbus������ �Ĵ�������
typedef struct
{
    uint16_t    flow_addr[10][7];            // ˮ���ַ
    uint16_t    flow_data[10][5];            // ˮ������ 
    uint16_t    flow_date[7];                // ��������
    uint16_t    flow_cnt ;                    // ���������
    uint16_t    flow_max ;                    // ���ˮ���
    
}STUSERCOMDATA;                     // �ܹ�125���Ĵ���       	



#define     MEMBER_MAX             10     // ��Ա����
#define     MEMBER_SIZE            12      // ������Առ���ֽ���
// nb�ϴ�����������
typedef struct
{  
    uint8_t     mu1Apart[4];
    uint32_t    mu4StuCnt;          // �ṹ���ܳ���     
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
}STUNBFLOWDATA;                     // ͨ��NB�ϴ�����������

extern STUNBFLOWDATA    gstuNbFlowData;   
extern STUSERCOMDATA    gstuMbusFlowData; 

void InitFlowData(void);
void FillFlowTail(STUNBFLOWDATA *pstu);
void FillFlowData(STUNBFLOWDATA *pstu);

uint8_t FlowConnect(void);
void ProcUart3Rec(void);

#endif


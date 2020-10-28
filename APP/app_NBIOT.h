/*
*******************************************************************************
*
*                           the app_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_nbiot.h
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/

#ifndef APP_NBIOT_H_
#define APP_NBIOT_H_


#define     NBTRANS_STEP                16              // NB????ID
#define     RETRANS_MAX                 2              //重传最大次数

#pragma pack (1)
typedef struct {
    
    uint8_t     mu1Head;        // ?
    //--------------------------// ????? 
    uint8_t     mu1Len[2];     
    uint8_t	    comwords;		//??? 0x15:????   0x81:?????   0x83:??????
    uint8_t		name1;			//??????
    uint8_t		name2;			//??????
    
    TTime       uRtc;           // ????    
    uint8_t     wkHour;         // ????
    uint8_t     wkMinute;       // ????    
    uint8_t     mu1Check;
    //--------------------------// ?????    
    uint8_t     mu1Tail;        // ? 
} STUCLOCKSYNC;
#pragma pack ()


#pragma pack (1)
typedef struct
{
    uint8_t     mu1Head;        // ??
    //--------------------------// ?????    
    uint8_t     mu1Len[2];      // ?????
	uint8_t		comwords;		//??? 0x15:????   0x81:?????   0x83:??????
	uint8_t		name1;			//??????
	uint8_t		name2;			//??????
	uint8_t		number;			//??????
	uint8_t		numbermax;		//??????
	uint8_t		netnumber;  	//????
	uint8_t		add[50][7]; 	//????    
    uint8_t     mu1Check;       // ??
    //--------------------------// ?????
    uint8_t     mu1Tail;        // ???
}UPPERPROT;
#pragma pack ()


#pragma pack (1)					// ????
typedef struct 
{
    uint8_t     mu1Apart[4];    			//帧头          3C 2A 6B 01
    uint32_t   mu4StuCnt;				//数据总长  A4 00 00 00
    
    uint32_t    mu4WaterNum;
    uint32_t    mu4DtuId; 
   
    
    uint8_t     mu1Rtc[6];
    uint8_t    mu1Csq;
    
    uint8_t    mu1Err;    
    uint8_t    mu1MemCnt;  
	uint8_t    mu1MemLen;  
	uint8_t    mu1MemBeg; 
	uint8_t    mu1MemTyp; 
    
    uint8_t    mu1AddrNData[10][12]; 
    uint8_t     mu1Hold[8];      // ??    
	uint32_t    mu4CltCnt;
	uint8_t    mu1DevTyp;
	uint8_t    mu1DevBat;
    uint8_t     mu1Check;
    uint8_t     mu1Tail;  
  
}STRNBDAT;
#pragma pack ()

//#pragma pack (1)
//typedef struct 
//{
//	uint8_t 		mu1Head[4];
//	uint8_t 		
//	
//}NBTRANSDATA;
//#pragma pack ()
	
extern __IO     uint8_t     retrans_cnt;
extern __IO     uint8_t     gucCsqValue;
extern __IO     uint32_t    gulNBTransRtc;

//----------public function declare--------------

void NB_Reset(void);
void NB_Close(void);

uint8_t NB_Connect(void);
uint8_t NB_DataUpload(void);

#endif /* APP_NOR_H_*/

/********************************************** (END OF FILE) *******************************************/

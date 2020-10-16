/*
*******************************************************************************
*
*                           the app_uart3 file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_RS485.c
* Version       : V1.0.0
* Programmer(s) : wubo
*******************************************************************************
*/

#include "includes.h"


#define     HAIFENG             (1)                 // 娴峰嘲娴侀噺璁�  
#define     FLOW_INVAL          (0xffffffffUL)      // 鏃犳晥娴侀噺


const uint8_t auchCRCHi[]=
{

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,

0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,

0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,

0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,

0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,

0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

0x40

};

const uint8_t auchCRCLo[] =
{

0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,

0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,

0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,

0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,

0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,

0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,

0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,

0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,

0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,

0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,

0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,

0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,

0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,

0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,

0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,

0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,

0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,

0x40

};

//----------------------------------------------------------------------------//
STUNBFLOWDATA           gstuNbFlowData; 
STUSERCOMDATA           gstuMbusFlowData; 
//----------------------------------------------------------------------------//
static  STUSERCOMDATA   gstuSerComData;
//static  uint32_t        gu4FlowTot      = 0;    // 鍑€娴侀噺    
static  uint8_t         gucUartBuf[256] = {0};  
static  uint16_t        uart3_len1,uart3_len2;
//----------------------------------------------------------------------------//
/*******************************************************************************
* Function Name  : GetFlowRecord
* Description    : 寰楀埌姝ｃ€佽礋銆佸噣娴侀噺鍊�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//uint32_t GetFlowRecord(uint8_t id)
//{    
//    uint32_t ultmp = 0;
//#if HAIFENG == 1 
//    // wubo: 妗冨北浜屾娆¤〃瀵瑰簲鐨勫崗璁€�
//    // 灏忕妯″紡:楂樺湴浣嶄氦鎹�
//    if(id == 1)         // 姝� 绱娴侀噺
//    {
//        ultmp = gstuSerComData.u2Reg10;     // 楂�16浣�
//        ultmp <<= 16;
//        ultmp += gstuSerComData.u2Reg09;    // 浣�16浣�   
//    }  
//    else if(id == 2)    // 璐� 绱娴侀噺
//    {
//        ultmp = gstuSerComData.u2Reg14;     // 楂�16浣�
//        ultmp <<= 16;
//        ultmp += gstuSerComData.u2Reg13;    // 浣�16浣�   
//    }
//    else if(id == 3)    // 鍑€ 绱娴侀噺
//    {
//        ultmp = gstuSerComData.u2Reg26;     // 楂�16浣�
//        ultmp <<= 16;
//        ultmp += gstuSerComData.u2Reg25;    // 浣�16浣�      
//    }
//    else
//    {
//    }    
//#else
//    // wubo: 妗冨北涓€娆¤〃瀵瑰簲鐨勫崗璁€�
//    // 澶х妯″紡锛氶珮鍦颁綅涓嶇敤浜ゆ崲
//    if(id == 1)         // 姝� 绱娴侀噺
//    {
//        ultmp = gstuSerComData.u2Reg09;     // 楂�16浣�
//        ultmp <<= 16;
//        ultmp += gstuSerComData.u2Reg10;    // 浣�16浣�   
//    }  
//    else if(id == 2)    // 璐� 绱娴侀噺
//    {
//        ultmp = gstuSerComData.u2Reg13;     // 楂�16浣�
//        ultmp <<= 16;
//        ultmp += gstuSerComData.u2Reg14;    // 浣�16浣�   
//    }
//    else if(id == 3)    // 鍑€ 绱娴侀噺(鐩存帴鐢ㄦ 娴侀噺鍙栦唬)
//    {
//        ultmp = gstuSerComData.u2Reg09;     // 楂�16浣�
//        ultmp <<= 16;
//        ultmp += gstuSerComData.u2Reg10;    // 浣�16浣�     
//    }
//    else
//    {
//    }   
//#endif          
//    return ultmp;
//}  


/*******************************************************************************
* Function Name  : FillFlowData
* Description    : 濉厖娴侀噺闃熷垪鏁版嵁
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FillFlowData(STUNBFLOWDATA *pstu)
{    
    TTime       mRtc;  
    uint16_t    i = 0 ,j = 0;    
 
    //----------------------------------------------//   
    pstu->mu1Apart[0] = 0x3c;
    pstu->mu1Apart[1] = 0x2a;
    pstu->mu1Apart[2] = 0x6b;
    pstu->mu1Apart[3] = 0x01;          
    
	pstu->mu4StuCnt = 164;
	pstu->mu2DtuID = gstuNbPara.mu2DtuID;
	pstu->mu2CjbID = gstuNbPara.mu2CjbID;
	pstu->mu1MemCnt = gstuNbPara.mu1FlowMax;
	pstu->mu1Csq = gucCsqValue;
	pstu->mu1MemLen = 12;
	pstu->mu1MemBeg = 0;
	pstu->mu1MemTyp = 0;
	for(i = 0; i < 10; i ++)
	{
		for(j = 0; j < 7; j ++)
        {
			pstu->mu1AddrNData[i * 12 + j] = addrcache.addrFlow[i * 7 + j];
		}
		
		for(j = 0; j < 5; j ++)
        {
			pstu->mu1AddrNData[i * 12 + 7 + j] = MbusFlow.flowdata[i * 5 + j];
		}
	}       
    //----------------------------------------------//    
    pstu->mu4WaterNum = DEF_WM_NUM; //gstuWmPara.mu4WaterNum;   
    //----------------------------------------------//    
    ReadRtcTime(&mRtc);     // 鏃堕挓鏁版嵁         
    pstu->mu1Rtc[0] = mRtc.year;  
    pstu->mu1Rtc[1] = mRtc.month;  
    pstu->mu1Rtc[2] = mRtc.day;  
    pstu->mu1Rtc[3] = mRtc.hours;  
    pstu->mu1Rtc[4] = mRtc.minutes;  
    pstu->mu1Rtc[5] = mRtc.seconds;  

	pstu->mu1DevTyp   = 0x85;                         // Mbus鎬荤嚎琛�
    //----------------------------------------------//        
	for(i=0; i<20; i++)
	{
		pstu->mu1DevBat    = CalBatPer();                  // 鐢甸噺鐧惧垎锟�
		Delay_ms(5);
	}
    //----------------------------------------------//        
    // 24灏忔椂娴侀噺璁板綍  

    //----------------------------------------------//    
    for(i = 0; i < 8; i++) {
        pstu->mu1Hold[i] = 0;                         // 淇濈暀瀛楄妭
    }
    //----------------------------------------------//

}

/*******************************************************************************
* Function Name  : FillFlowTail
* Description    : 濉厖娴侀噺闃熷垪鏁版嵁
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FillFlowTail(STUNBFLOWDATA *pstu)
{    
   
    pstu->mu1Check  = CheckSum((uint8_t *)pstu,(uint16_t)(sizeof(STUNBFLOWDATA) - 2));    
    pstu->mu1Tail   = 0x3e;         
}


/*******************************************************************************
*	鍑� 鏁� 鍚�: InitFlowData
*	鍔熻兘璇存槑: 鍒濆鍖栨椂锛屾祦閲忔暟鎹棤鏁�
*	褰�    鍙�: 鏃�
*	杩� 鍥� 鍊�: 鏃�  
*******************************************************************************/
//void InitFlowData(void)
//{
//    uint16_t i ,j;
//    // 娴侀噺鍒濆€煎叏閮芥棤鏁�
//    for(i = 0; i < MEMBER_MAX; i++) 
//	{        
//        gstuNbFlowData.mu1AddrNData[i][7] = 0x75;    
//		gstuNbFlowData.mu1AddrNData[i][8] = 0;        
//		gstuNbFlowData.mu1AddrNData[i][9] = 0;   
//		gstuNbFlowData.mu1AddrNData[i][10] = 0;   
//		gstuNbFlowData.mu1AddrNData[i][11] = 0;   
//    }
//    

//    FillFlowData(&gstuNbFlowData);  // 濉厖娴侀噺鏁版嵁   
//}  

/*******************************************************************************
* Function Name  : MODBUS_CRC16
* Description    : modbus 鏍￠獙
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static uint16_t MODBUS_CRC16(uint8_t *updata,uint16_t len)
{
    uint8_t  uindex;
    uint8_t  uchCRCHi=0xff;
    uint8_t  uchCRCLo=0xff;
    
    while(len--)
    {
        uindex = uchCRCHi ^ *updata++;
        uchCRCHi = uchCRCLo ^ auchCRCHi[uindex];
        uchCRCLo = auchCRCLo[uindex];
    }
    
    return (uchCRCHi<<8|uchCRCLo);
}

/*******************************************************************************
* Function Name  : Modbus_3
* Description    : modbus 璇昏繑鍥�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int16_t Modbus_3(void)
{	        
    uint16_t    i,reg_num;            
    uint8_t     *pchar;
    
    // 瓒呰繃鏈€澶�125涓瘎瀛樺櫒    
    reg_num = gucUartBuf[2];          
       
    if(reg_num == 0 || reg_num > 250) {
        return(-1);
    }
    // 瓒呰繃鎺ユ敹缁撴瀯浣撶殑闀垮害 
    if(reg_num > sizeof(STUSERCOMDATA)) {
        return(-1);
    } 

    pchar = (uint8_t *)&gstuSerComData;    		
  
    // 楂樹綆浣嶄氦鎹�(瑙ｇ爜鎺ユ敹鐨勬暟鎹�) 
    for(i = 0; i < reg_num; i += 2)
    {	
        pchar[i]      = gucUartBuf[i + 4];
        pchar[i + 1]  = gucUartBuf[i + 3];
    }
        
    
//    gu4FlowTot = GetFlowRecord(3);
      
#ifdef DBGCOM       
    UartTxd((uint8_t *)gucUartBuf,(5 + reg_num));         
#endif            	    
	return(0);
}

static uint16_t gucTxd3Cnt = 0;
static uint16_t gucRxd3Cnt = 0;
/*******************************************************************************
* Function Name  : Flow_Data_Rec
* Description    : 澶勭悊浠嶤OM3鍥炰紶鐨勬暟鎹�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Flow_Data_Rec(void)
{
    uint16_t    ret = 0;    	
    uint16_t    cnt = 0;  
    // 鎷疯礉涓插彛鎺ユ敹鐨勬暟鎹�
    cnt =  comMemCpy(COM3,(uint8_t *)gucUartBuf);                                        
    // 璇�2涓瘎瀛樺櫒杩斿洖鐨勯暱搴�
    if(cnt >= 15)                           
    {     		
        ret = MODBUS_CRC16((uint8_t *)gucUartBuf, cnt);  
        if(ret == 0)    // 鏍￠獙姝ｇ‘
        {           
            if(gucUartBuf[0] == 0x68 && gucUartBuf[34] == 0x16)
            {            
                ret = Modbus_3();     
                if(ret == 0)        // 璇绘垚鍔�
                {                  
                    gucRxd3Cnt += 1;                        
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name  : ProcUart3Rec
* Description    : 澶勭悊浠嶤OM3鍥炰紶鐨勬暟鎹�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProcUart3Rec(void)
{
    uart3_len2 = GetRxCount(COM3);                           
    if((uart3_len1 > 0) && (uart3_len1 == uart3_len2))	    
    {									
        if(uart3_len1 <= UART3_RX_BUF_SIZE)         // 瀛楃涓查暱搴﹂檺鍒�
        {	                      
            Flow_Data_Rec();									           
        }													
    }	  
    uart3_len1 = GetRxCount(COM3);	  
}


// 璇绘祦閲忓€�
#if HAIFENG == 1    // Mbus鎬荤嚎琛�
static uint8_t TxdBuf3[22] = {0xFE,0xFE,0xFE,0xFE,0xFE,0xFE
                                ,0X68,0X10,0X10,0X01,0X40,0X30,0X01,0X33,0X78,0X01,0x03,0x1F,0X90,0X00,0X58,0X16};
#else               // 澶╀俊鐢电浠〃
static uint8_t TxdBuf3[16] = {0x08, 0x04, 0x10, 0x18, 0x00, 0x14, 0x74, 0x5B};
#endif

/*******************************************************************************
* Function Name  : FlowConnect
* Description    : 璇绘祦閲忚鏁版嵁(01 03 00 08 00 14 C4 07)  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t FlowConnect(void)
{      
    //TTime       mRtc;  
    uint8_t     ret = 0;  
    uint8_t     i; 
    uint8_t     cnt;    
    //-----------------------------------------//     
    // 濡傛灉闂归挓鏍囧織琚浣嶏紝璇存槑鍙互杩涘叆寰呮満鐘舵€佷簡锛屼笉瑕佹墽琛屾绋嬪簭浜�;
    if(gstuFlag.mbAlmF == 0) 
    {
        gucTxd3Cnt = 0;
        gucRxd3Cnt = 0;
        return 0;
    }   
    // 濡傛灉宸茬粡浣胯兘NB鍙戦€侊紝璇存槑485鎬荤嚎鏁版嵁宸茶鍙栦簡,涓嶈鎵ц姝ょ▼搴忎簡;
    if(gstuFlag.mbNbEn > 0) 
    {
        gucTxd3Cnt = 0;
        gucRxd3Cnt = 0;
        return 0;
    }  
    //-----------------------------------------//       
  
    if(gucRxd3Cnt > 0)          // 鍙鎺ユ敹鎴愬姛1娆★紝鍗冲彲閫€鍑�
    {
        ret = 1;
    }
    else
    {
        gucTxd3Cnt += 1;        
        if(gucTxd3Cnt <= 3)     // 鏈€澶氬彂閫�3娆�
        {
            gucRxd3Cnt = 0;
            cnt = gstuMbusFlowData.flow_cnt;
            for(i=0;i<7;i++)
            {
              TxdBuf3[i+8] = gstuMbusFlowData.flow_addr[cnt][i];
            }
            comSendBuf(COM3,(uint8_t *)TxdBuf3,22);       
        }
        else
        {   
            ret = 1;            
//            gu4FlowTot = FLOW_INVAL;    // 鏃犳晥鏁版嵁                    
        }
    }
    //-----------------------------------------//
    if(ret > 0)                 // 鎺ユ敹瀹屾瘯
    {               
        gucTxd3Cnt = 0;
        gucRxd3Cnt = 0;
        gstuMbusFlowData.flow_cnt += 1;
        // 濉厖鏁版嵁
        FillFlowData(&gstuNbFlowData);    
    }   
        
    return ret;
}

/*******************************(END OF FILE)**********************************/


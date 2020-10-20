/*
*******************************************************************************
*
*                           the app_NBRSV file
*                  ST Microelectronics STM32 with the STM32L151
*
* Filename      : app_NBRSV.c
* Version       : V1.0.0
* Programmer(s) : wubo
*******************************************************************************
*/

#include "includes.h"


#define TXDFLAG     0x00f0
#define RXDFLAG     0x000f


const       uint8_t     NBACK[][16] =   {
                                            "+CSCON",       // 濞ｅ洠鈧啿濞囬梺顐ｇ閺屽洭鎮╅懜纰樺亾閿燂拷                                                                          
                                            "+CEREG",       // 闁艰鲸姊荤紞澶愬箣閹邦剙顫犲☉鎾抽閹拷                         
                                            "+CGATT",       // 婵☆垪鈧櫕鍋ユ繝纰樺亾婵炶尪顔婄粭宀勫触閿燂拷
                                            "+NSOCLI",      // 闁稿繑濞婂Λ纰∣CKET濞ｅ洠鍓濇导锟�
                                            "CONNECT OK",   // 閺夆晜绮庨埢鍏兼交閻愭潙澶嶉柟瀛樺姇婵拷
                                            "+CME ERROR",   // 婵☆垪鈧櫕鍋ラ柟韬插劦閺侊拷                          
                                            "ERROR",        // 闁圭ǹ娲ｉ幎銈夊礄濞差亝鏅�                                  
                                            "+NSOSTR",      // 闁轰胶澧楀畵渚€宕ｉ幋锔瑰亾娴ｇ懓鐏囬柛鏃撴嫹
                                            "+NSORF",       // 闁轰胶澧楀畵渚€骞掗妷锔芥毆闁瑰瓨鍔曟慨锟�                                
                                            "+CSQ",         // 濞ｅ洠鈧啿濞囬悹鎰╁姂閸ｏ拷
                                            "+CCLK",        // 濞ｅ洠鈧啿濞囬柡浣哄瀹擄拷
                                            "460"          // ISMI闁告瑱鎷�                    
                                        };    
                                

static      uint8_t     ucom[UART2_RX_BUF_SIZE]         = {0};      // ???????
#if 0
static      uint8_t     datbuf[UART2_TX_BUF_SIZE]       = {0};      // hex?asc???
#endif
static      uint8_t     gucNbCard[8]        = {0};      // NB?????


__IO        STUNBSTA    gstuNbSta;                      // NB???
__IO        uint8_t     gucCsqValue         = 0;        // ????
  

/*******************************************************************************
* Function Name  : At_SendStr
* Description    : At????   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t Asc2Hex(uint8_t uc)
{
    uint8_t uct = 0;
	
	uct = uc;	
	if(uct >= 0x30 && uct <= 0x39)      {   // 0 ~ 9
		uct -= 0x30;
	}
	else if(uct >= 0x41 && uct <= 0x46) {   // A ~ F
        uct -= 0x37;	
	}	
	else {
		uct = 0;
	}		         
    return uct;
}

/*******************************************************************************
* Function Name  : AscReConv
* Description    : ??NBiot??????,????hex?? 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t AscReConv(uint8_t *src,uint8_t *dst,uint16_t cnt)
{	
    uint16_t    length 		= 0;
	uint16_t 	u2tmp[2] 	= {0};
	uint16_t    i = 0, 	    j = 0;	
	uint8_t     hibit = 0, 	lobit = 0;
	    
	for(i = 0;i < cnt;i++)
	{   	
		if((src[i] == 0x33 && src[i+1] == 0x38) || (src[i] == 0x37 && src[i+1] == 0x42))
		{   // 0x38, 0x7b	
			hibit 	= Asc2Hex(src[i+2]) & 0x0f;
			lobit 	= Asc2Hex(src[i+3]) & 0x0f;					
			u2tmp[0] = (uint16_t)((hibit << 4) + lobit);

			hibit 	= Asc2Hex(src[i+4]) & 0x0f;
			lobit 	= Asc2Hex(src[i+5]) & 0x0f;					
			u2tmp[1] = (uint16_t)((hibit << 4) + lobit);
			
			length 	= 4 + u2tmp[0] + (u2tmp[1]<<8);     
            
			if(src[i+length*2-2] == 0x37 && src[i+length*2-1] == 0x44)     
			{   // 0x7d
				for(j = 0;j < length;j++)
				{
					hibit = Asc2Hex(src[i+j*2]) & 0x0f;
					lobit = Asc2Hex(src[i+j*2+1]) & 0x0f;									
					dst[j] = (hibit << 4) + lobit;					
				}
				return length;	// ?????
			} 			
		}
	}
	return 0;
}


/*******************************************************************************
* Function Name  : Parch
* Description    : At????   
* Input          : ??????
* Output         : pch???????,c??????
* Return         : None
*******************************************************************************/
#if 0
void Parch(uint8_t *pch,uint16_t c)
{   
	uint16_t    len = 0;
    uint8_t     uct = 0;
    

    STUCLOCKSYNC mstuClock;
    
    len = AscReConv(pch,datbuf,c);          // ???????    
    
	if(len > 3)                             // for(i = 0;i < len;i++)
    {
        if(datbuf[0] == 0x38 || datbuf[0] == 0x7B)
        {            
			//length = (uint16_t)datbuf[1] + ((uint16_t)datbuf[2]<<8);
			
            if(datbuf[len-1] == 0x7D)       // ??????
            {                       
                switch(datbuf[3])
                {
                    case 0xA5:	// ???????
                        break;
                    
                    case 0x81:	// ??????
						//xSemaphoreGive(xSem_NbDataReq);																  
                        break;
				
                    case 0x83:	// ??????
                        uct = NbiotSum((uint8_t *)datbuf + 1, len - 3);     
                        if(uct == datbuf[len-2])
                        {
                            memcpy((uint8_t *)&tstuWmTab,  (uint8_t *)datbuf,  len);                            
                            //xQueueSendToBack(xQue_WmTab,( void * )&tstuWmTab,0);                                                                
                        }
                        break;
                  						
                    case 0x15: 	//????  
                        uct = NbiotSum((uint8_t *)datbuf + 1, len - 3);                        
                        if(uct == datbuf[len-2])
                        {
                            memcpy((uint8_t *)&mstuClock,   (uint8_t *)datbuf,  len);                                                  
                            //xQueueSendToBack(xQue_RTC,( void * )&mstuClock,0);           
                        }
                        break;
					
                    default:
                        break;
                }					
            }
        }       
    }
}
#endif	

/*******************************************************************************
* Function Name  : GerNbClock
* Description    : ??NB????(????8??)
* Input          : pch???????,c??????
* Output         : None
* Return         : None
*******************************************************************************/
/*
+CCLK:19/04/19,02:26:51+32
*/
void GerNbClock(uint8_t *pch,uint16_t c)
{	    
    TTime       mRtcTime;       
	uint8_t     hibit = 0, 	lobit = 0;
    uint8_t 	uct = 0;
   
    if(pch[23] == '+')  // ??????
    {
        hibit 	= Asc2Hex(pch[6]) & 0x0f;
        lobit 	= Asc2Hex(pch[7]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);    
        mRtcTime.year = uct;
          
        hibit 	= Asc2Hex(pch[9]) & 0x0f;
        lobit 	= Asc2Hex(pch[10]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.month = uct;
        
        hibit 	= Asc2Hex(pch[12]) & 0x0f;
        lobit 	= Asc2Hex(pch[13]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.day = uct;
        
        hibit 	= Asc2Hex(pch[15]) & 0x0f;
        lobit 	= Asc2Hex(pch[16]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.hours = uct;
        
        hibit 	= Asc2Hex(pch[18]) & 0x0f;
        lobit 	= Asc2Hex(pch[19]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.minutes = uct;
        
        hibit 	= Asc2Hex(pch[21]) & 0x0f;
        lobit 	= Asc2Hex(pch[22]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.seconds = uct;
               
        Set_Time(mRtcTime);
    }
}


/*******************************************************************************
* Function Name  : MsgDecode
* Description    : ??NB?????   
* Input          : pch???????,c??????
* Output         : None
* Return         : None
*******************************************************************************/
static uint16_t  MsgDecode(uint8_t *pch,uint16_t c)
{    
    uint16_t status = 0;
    
    uint8_t i = 0,  j = 0;
    uint8_t row = 0,len=0;        
    
    uint8_t uct = 0;
    //==============================================//
    if(pch[0] == 'O' && pch[1] == 'K')          // ??"OK"
    {
        DISABLE_INT();  
        gstuNbSta.ackok = 1;                    // ????                
        ENABLE_INT();
        return 0;
    }
    //==============================================//
    row = sizeof(NBACK) / sizeof(NBACK[0]);     // ?????     
    for(i = 0; i < row; i++)
    {                 
        len =  strlen((char *)NBACK[i]);        // ??????
        for(j = 0; j < len; j++)
        {
            // ??????NB??????
            if(pch[j] != NBACK[i][j])
            {
                break;
            }                    
        }
        
        if(j == len)        // ????
        {          
            row = i;        // ?????            
            status = 0x5555;// ??????
            break;
        }
    }               
 	//==============================================//
    if(status == 0x5555)    // ?????????
    {      
        DISABLE_INT();         
        switch(row)
        {
            case 0: //"+CSCON",        
                uct = pch[c-5];                 // ??闁硅崵鎲朶n闁跨噦鎷�,???5???
                if(uct == '1' ) 
                {                    
                    gstuNbSta.cnnct = 1;        // ???(?????)
                    gstuNbSta.cscon = 1;        // ??????  
                }
                else 
                {
                    gstuNbSta.cnnct = 0;        // ???(?????)
                    gstuNbSta.cscon = 0;        // ?????
                }                                               
                break;
                                 
            case 1: //"+CEREG",   
                uct = pch[c-3];                 // ??闁硅崵鎲朶n闁跨噦鎷�,???3???
                if(uct == '1' || uct == '5') {
                    gstuNbSta.cereg = 1;        // ????
                }
                else {
                    gstuNbSta.cereg = 0;
                }                   
                break;
                                  
            case 2: //"+CGATT",
                 uct = pch[c-3];                // ??闁硅崵鎲朶n闁跨噦鎷�,???3???
                if(uct == '1')  {
                    gstuNbSta.cgatt = 1;        // ????
                }
                else {                
                    gstuNbSta.cgatt = 0;
                }
                break;
            
            case 3: //"+NSOCLI"                 // NB婵☆垪鈧櫕鍋ラ柟缁樺姉閵囨ocket,閻炴稏鍔庨妵姘跺籍閻樺磭銆婇弶鈺冨仦鐢瓨娼诲⿰鍛煠IP缂佹棏鍨拌ぐ锟�                     
                gstuNbSta.nsocl = 1;            // ??
                break;
            
            case 4: //"CONNECT OK"                               
                gstuNbSta.nsoco = 1;            // 閺夆晜绮庨埢鍏兼交閻愭潙澶嶉柟瀛樺姇婵拷         
                break;
            
            case 5: //"+CME ERROR"              // 闁硅翰鍎甸弫锟�   
                gstuNbSta.nsocl = 1;            // 闂佹彃绉撮幆锟�
                gstuNbSta.ackerr = 1;           // err閹煎瓨姊婚悺锟�                        
                break;            
            
            case 6: //"ERROR"                   // 闁圭ǹ娲ｉ幎銈夊礄濞差亝鏅╅柟瀛樼墵閸ｅ憡寰勯敓锟�          
                gstuNbSta.ackerr = 1;         
                break;
                 
            case 7: //"+NSOSTR"                 // nb????
                status = TXDFLAG;               
                gstuNbSta.nbtxd = 1;          
                break;
            
            case 8: //"+NSORF"                  // nb????
                status = RXDFLAG;   
                gstuNbSta.nbrxd = 1;                 
                break;   
          
            case 9: //"+CSQ"                    // ????                           
                uct = Asc2Hex(pch[5]);
                if(pch[6] >= 0x30 && pch[6] <= 0x39) // ???????
                {                
                    uct *= 10;
                    uct += Asc2Hex(pch[6]);		
                } 
                gucCsqValue = uct;              // ????
                gstuNbSta.csq = 1;            
                break; 
              
            case 10://"+CCLK"                    // ????                
                gstuNbSta.cclk = 1;                         
                
                GerNbClock(pch,c);              // ??NB????                                                                        
                break;   
                
            case 11://"cimi"                    // ????(460040101900126)
                gstuNbSta.cimi = 1;  

                gucNbCard[0] = 0x04;
                gucNbCard[1] = 0x60;
                for(i = 2; i < 8; i++)
                {
                    gucNbCard[i] =  (pch[2*i-1]<<4);
                    gucNbCard[i] += (pch[2*i]&0x0f);
                }
                break;     
            
            default:
                break;        
        }   
        ENABLE_INT();        
    }
    else     // ????
    {
        status = 0;                                   
    }    
    return status;
}


/*******************************************************************************
* Function Name  : GetCardImsi
* Description    : ????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetCardImsi(void)
{
    uint8_t i = 0;
    for(i = 0; i < 8; i++) {            // ????
        gstuNbPara.mu1Imsi[i] = gucNbCard[i];   
    }
    gstuNbPara.mu1Csq = gucCsqValue;    // ????
    gstuNbPara.mu1Check = CheckSum((uint8_t *)&gstuNbPara,(uint16_t)(sizeof(STUNBSET) - 1));                          
}

/*******************************************************************************
* Function Name  : Nb_Data_Rec
* Description    : ???NB?????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Nb_Data_Rec(void)
{
    uint16_t count = 0;  
    uint16_t status = 0;
    
    count = myMemCpy(NB_COM,ucom);	

    if(count > 2)                           // ????"\r\n"
    {		                                             
        status = MsgDecode(ucom,count);	    // ??COM???

        if(status == RXDFLAG) 
        {                     
            //Parch(ucom,count);              // ??????  
        }
        
#if     DBGMODE                   
        if(count <= UART1_TX_BUF_SIZE) 
        {                   
            comSendBuf(DBGCOM,ucom,count);  // ????                                       
        }
#endif 
    }
}


static  uint16_t  NbRsv_len1 = 0;
static  uint16_t  NbRsv_len2 = 0;

/*******************************************************************************
* Function Name  : ProcNbComRec
* Description    : ???NB?????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProcNbComRec(void)
{
    NbRsv_len2 = GetRxCount(NB_COM);                           
    if((NbRsv_len1 > 0) && (NbRsv_len1 == NbRsv_len2))	    // COM????
    {									
        if(NbRsv_len1 <= UART2_RX_BUF_SIZE)                 // ???????
        {	                       
            Nb_Data_Rec();										
        }													
    }	  
    NbRsv_len1 = NbRsv_len2;//GetRxCount(NB_COM);	  
}      
/*******************************(END OF FILE)**********************************/

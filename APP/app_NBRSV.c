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
                                            "+CSCON",       // 濠电儑绲藉ú鐘诲焵椤掆偓閸熸寧绻涢崶顒佲拺妞ゆ劧绲块。鏌ユ煛鐏炶姤鍠橀柟顔规櫊閹虫粎鍠婂Ο杞板闂佽法鍣﹂幏锟�                                                                          
                                            "+CEREG",       // 闂備浇澹堟ご绋款潖閼姐倗绀婂鑸靛姇缁狅綁鏌熼柇锕€澧い顐ゅТ閳藉骞橀幎绛嬧偓妤呮煙椤栥倖瀚�                         
                                            "+CGATT",       // 婵犵妲呴崹顏堝焵椤掑啯鐝柛瀣ㄥ劜缁绘繄鍠婂Ο杞板婵犵數鍋犵亸顏堫敊婵犲嫮绠旂€光偓閸曨偉袝闂佽法鍣﹂幏锟�
                                            "+NSOCLI",      // 闂備胶枪缁绘垶绻涙繝鍋芥稓鍠婇埈顤塊ET濠电儑绲藉ú鐘诲礈濠靛洤顕遍柨鐕傛嫹
                                            "CONNECT OK",   // 闂佸搫顦弲婊呭垝鎼淬劌鐓濋柛蹇撳悑娴溿倝鏌ｉ幇顓熺稇濠㈣泛绉归弻鐔衡偓娑櫭慨鍥р攽椤曞棙瀚�
                                            "+CME ERROR",   // 婵犵妲呴崹顏堝焵椤掑啯鐝柛瀣ㄥ劦閺岀喖鐓幓鎺戝Е闂佽桨绶ら幏锟�                          
                                            "ERROR",        // 闂備礁婀辨灙婵炶缍侀獮搴ㄥΧ婢跺﹦顦╁┑鐐叉▕娴滄繈寮查敓锟�                                  
                                            "+NSOSTR",      // 闂備浇妗ㄩ懗鑸垫櫠濡も偓閻ｅ灚绗熼埀顒€鐣烽敐澶婄闁挎梻鎳撴禍鎯归敐鍥ㄥ殌闁诲繐娲弻娑㈠籍閹炬潙顏�
                                            "+NSORF",       // 闂備浇妗ㄩ懗鑸垫櫠濡も偓閻ｅ灚绗熼埀顒勭嵁閹烘唯闁挎棁濮ゅВ鍡涙⒑閻熸壆鎽犻柛鏃€娲橀幈銊╂晸閿燂拷                                
                                            "+CSQ",         // 濠电儑绲藉ú鐘诲焵椤掆偓閸熸寧绻涢崶顒佸仯闁规壋鏅涙慨鍌炴煕閿濆繑瀚�
                                            "+CCLK",        // 濠电儑绲藉ú鐘诲焵椤掆偓閸熸寧绻涢崶顒佺厸濞达絽鎼。鑲┾偓瑙勬惈閹凤拷
                                            "460"          // ISMI闂備礁鎲￠悷閬嶅箯閿燂拷                    
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
                uct = pch[c-5];                 // ??闂備胶顢婂畷鐢稿箟閺堢闂備浇娉曢崳锕傚箯閿燂拷,???5???
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
                uct = pch[c-3];                 // ??闂備胶顢婂畷鐢稿箟閺堢闂備浇娉曢崳锕傚箯閿燂拷,???3???
                if(uct == '1' || uct == '5') {
                    gstuNbSta.cereg = 1;        // ????
                }
                else {
                    gstuNbSta.cereg = 0;
                }                   
                break;
                                  
            case 2: //"+CGATT",
                 uct = pch[c-3];                // ??闂備胶顢婂畷鐢稿箟閺堢闂備浇娉曢崳锕傚箯閿燂拷,???3???
                if(uct == '1')  {
                    gstuNbSta.cgatt = 1;        // ????
                }
                else {                
                    gstuNbSta.cgatt = 0;
                }
                break;
            
            case 3: //"+NSOCLI"                 // NB婵犵妲呴崹顏堝焵椤掑啯鐝柛瀣ㄥ劦閺岀喓绱掑Ο鍝勵潕闂侀潧娲﹂鐠穋ket,闂佽崵鍋炵粙蹇涘礉鎼淬劌桅婵ǹ娉涚猾宥夋煟濡櫣锛嶉柕鍡楋躬瀵爼鍩￠崘銊ゆ勃闁汇埄鍨伴悺銊ヮ焽鐠囨祴鍫柛娑变簼閻撶嚐P缂傚倷鐒﹀Λ蹇涘垂閹峰被浜归柨鐕傛嫹                     
                gstuNbSta.nsocl = 1;            // ??
                break;
            
            case 4: //"CONNECT OK"                               
                gstuNbSta.nsoco = 1;            // 闂佸搫顦弲婊呭垝鎼淬劌鐓濋柛蹇撳悑娴溿倝鏌ｉ幇顓熺稇濠㈣泛绉归弻鐔衡偓娑櫭慨鍥р攽椤曞棙瀚�         
                break;
            
            case 5: //"+CME ERROR"              // 闂備胶顢婄紙浼村磿閻㈢ǹ鏋侀柨鐕傛嫹   
                gstuNbSta.nsocl = 1;            // 闂傚倷鐒﹁ぐ鍐矓閹绢喖绠氶柨鐕傛嫹
                gstuNbSta.ackerr = 1;           // err闂佸湱鍘ч悺銊ヮ潖婵犳碍鍋ら柨鐕傛嫹                        
                break;            
            
            case 6: //"ERROR"                   // 闂備礁婀辨灙婵炶缍侀獮搴ㄥΧ婢跺﹦顦╁┑鐐叉▕娴滄繈寮查埡鍛厵閻庢稒顭囨晶鐢告煕閿濆懏鍟炵€垫澘瀚伴弫鎾绘晸閿燂拷          
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

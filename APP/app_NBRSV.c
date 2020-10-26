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
                                            "+CSCON",       // 信号通断状态                                                                                
                                            "+CEREG",       // 联网成功与否                         
                                            "+CGATT",       // 模块激活与否
                                            "+NSOCLI",      // 关闭SOCKET信息
                                            "CONNECT OK",   // 远程连接成功
                                            "+CME ERROR",   // 模块报错                             
                                            "ERROR",        // 指令出错                                    
                                            "+NSOSTR",      // 数据发送成功
                                            "+NSORF",       // 数据接收成功                                    
                                            "+CSQ",         // 信号质量
                                            "+CCLK",        // 始终数据
                                            "460"          // ISMI号                       
                                        };    
                                

static      uint8_t     ucom[UART2_RX_BUF_SIZE]         = {0};      // 串口收到的字符
#if 0
static      uint8_t     datbuf[UART2_TX_BUF_SIZE]       = {0};      // hex转asc缓冲区
#endif
static      uint8_t     gucNbCard[8]        = {0};      // NB卡号及信号


__IO        STUNBSTA    gstuNbSta;                      // NB状态字
__IO        uint8_t     gucCsqValue         = 0;        // 信号质量
  

/*******************************************************************************
* Function Name  : At_SendStr
* Description    : At指令发送   
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
* Description    : 得到NBiot返回的字符串,并转换成hex字符 
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
				return length;	// 返回字节数
			} 			
		}
	}
	return 0;
}


/*******************************************************************************
* Function Name  : Parch
* Description    : At指令发送   
* Input          : 识别有效数据
* Output         : pch为接收的字符串，c为字符串长度
* Return         : None
*******************************************************************************/
#if 0
void Parch(uint8_t *pch,uint16_t c)
{   
	uint16_t    len = 0;
    uint8_t     uct = 0;
    

    STUCLOCKSYNC mstuClock;
    
    len = AscReConv(pch,datbuf,c);          // 分解出协议数据    
    
	if(len > 3)                             // for(i = 0;i < len;i++)
    {
        if(datbuf[0] == 0x38 || datbuf[0] == 0x7B)
        {            
			//length = (uint16_t)datbuf[1] + ((uint16_t)datbuf[2]<<8);
			
            if(datbuf[len-1] == 0x7D)       // 数组结束字符
            {                       
                switch(datbuf[3])
                {
                    case 0xA5:	// 心跳包返回应答
                        break;
                    
                    case 0x81:	// 水表数据请求
						//xSemaphoreGive(xSem_NbDataReq);																  
                        break;
				
                    case 0x83:	// 更新水表地址
                        uct = NbiotSum((uint8_t *)datbuf + 1, len - 3);     
                        if(uct == datbuf[len-2])
                        {
                            memcpy((uint8_t *)&tstuWmTab,  (uint8_t *)datbuf,  len);                            
                            //xQueueSendToBack(xQue_WmTab,( void * )&tstuWmTab,0);                                                                
                        }
                        break;
                  						
                    case 0x15: 	//更新时间  
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
* Description    : 获取NB时钟数据(小时要加8小时)
* Input          : pch为接收的字符串，c为字符串长度
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
   
    if(pch[23] == '+')  // 简单判断一下
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
* Description    : 匹配NB返回的数据   
* Input          : pch为接收的字符串，c为字符串长度
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
    if(pch[0] == 'O' && pch[1] == 'K')          // 返回"OK"
    {
        DISABLE_INT();  
        gstuNbSta.ackok = 1;                    // 应答标志                
        ENABLE_INT();
        return 0;
    }
    //==============================================//
    row = sizeof(NBACK) / sizeof(NBACK[0]);     // 数组的维度     
    for(i = 0; i < row; i++)
    {                 
        len =  strlen((char *)NBACK[i]);        // 当前字符长度
        for(j = 0; j < len; j++)
        {
            // 接收字符串与NB反馈字符比较
            if(pch[j] != NBACK[i][j])
            {
                break;
            }                    
        }
        
        if(j == len)        // 匹配成功
        {          
            row = i;        // 回复的序号            
            status = 0x5555;// 成功识别标志
            break;
        }
    }               
 	//==============================================//
    if(status == 0x5555)    // 接收字符串识别成功
    {      
        DISABLE_INT();         
        switch(row)
        {
            case 0: //"+CSCON",        
                uct = pch[c-5];                 // 加上“\r\n”，倒数第5个字符
                if(uct == '1' ) 
                {                    
                    gstuNbSta.cnnct = 1;        // 信号通(暂时没用到)
                    gstuNbSta.cscon = 1;        // 信号连接应答  
                }
                else 
                {
                    gstuNbSta.cnnct = 0;        // 信号断（暂时没用到）
                    gstuNbSta.cscon = 0;        // 信号未连接
                }                                               
                break;
                                 
            case 1: //"+CEREG",   
                uct = pch[c-3];                 // 加上“\r\n”，倒数第3个字符
                if(uct == '1' || uct == '5') {
                    gstuNbSta.cereg = 1;        // 联网成功
                }
                else {
                    gstuNbSta.cereg = 0;
                }                   
                break;
                                  
            case 2: //"+CGATT",
                 uct = pch[c-3];                // 加上“\r\n”，倒数第3个字符
                if(uct == '1')  {
                    gstuNbSta.cgatt = 1;        // 模块激活
                }
                else {                
                    gstuNbSta.cgatt = 0;
                }
                break;
            
            case 3: //"+NSOCLI"                 // NB模块提示关闭socket，表示无法连接远程IP端口                     
                gstuNbSta.nsocl = 1;            // 重启
                break;
            
            case 4: //"CONNECT OK"                               
                gstuNbSta.nsoco = 1;            // 连接远程成功             
                break;
            
            case 5: //"+CME ERROR"              // 报错   
                gstuNbSta.nsocl = 1;            // 重启
                gstuNbSta.ackerr = 1;           // err应答                              
                break;            
            
            case 6: //"ERROR"                   // 指令出错或重复             
                gstuNbSta.ackerr = 1;         
                break;
                 
            case 7: //"+NSOSTR"                 // nb发送完成
                status = TXDFLAG;               
                gstuNbSta.nbtxd = 1;          
                break;
            
            case 8: //"+NSORF"                  // nb接收完成
                status = RXDFLAG;   
                gstuNbSta.nbrxd = 1;                 
                break;   
          
            case 9: //"+CSQ"                    // 信号强度                           
                uct = Asc2Hex(pch[5]);
                if(pch[6] >= 0x30 && pch[6] <= 0x39) // 避免此位不存在
                {                
                    uct *= 10;
                    uct += Asc2Hex(pch[6]);		
                } 
                gucCsqValue = uct;              // 信号强度
                gstuNbSta.csq = 1;            
                break; 
              
            case 10://"+CCLK"                    // 时钟数据                
                gstuNbSta.cclk = 1;                         
                
                GerNbClock(pch,c);              // 获取NB时钟数据                                                                        
                break;   
                
            case 11://"cimi"                    // 卡号识别(460040101900126)
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
    else     // 数据接收
    {
        status = 0;                                   
    }    
    return status;
}


/*******************************************************************************
* Function Name  : GetCardImsi
* Description    : 卡号识别
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetCardImsi(void)
{
    uint8_t i = 0;
    for(i = 0; i < 8; i++) {            // 移动号码
        gstuNbPara.mu1Imsi[i] = gucNbCard[i];   
    }
    gstuNbPara.mu1Csq = gucCsqValue;    // 信号强度
    gstuNbPara.mu1Check = CheckSum((uint8_t *)&gstuNbPara,(uint16_t)(sizeof(STUNBSET) - 1));                          
}

/*******************************************************************************
* Function Name  : Nb_Data_Rec
* Description    : 处理从NB回传的数据
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Nb_Data_Rec(void)
{
    uint16_t count = 0;  
    uint16_t status = 0;
    
    count = myMemCpy(NB_COM,ucom);	

    if(count > 2)                           // 至少包含"\r\n"
    {		                                             
        status = MsgDecode(ucom,count);	    // 解码COM口字符

        if(status == RXDFLAG) 
        {                     
            //Parch(ucom,count);              // 识别有效数据  
        }
        
#if     DBGMODE                   
        if(count <= UART1_TX_BUF_SIZE) 
        {                   
            comSendBuf(DBGCOM,ucom,count);  // 打印字符                                       
        }
#endif 
    }
}


static  uint16_t  NbRsv_len1 = 0;
static  uint16_t  NbRsv_len2 = 0;

/*******************************************************************************
* Function Name  : ProcNbComRec
* Description    : 处理从NB回传的数据
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProcNbComRec(void)
{
    NbRsv_len2 = GetRxCount(NB_COM);                           
    if((NbRsv_len1 > 0) && (NbRsv_len1 == NbRsv_len2))	    // COM口有数据
    {									
        if(NbRsv_len1 <= UART2_RX_BUF_SIZE)                 // 字符串长度限制
        {	                       
            Nb_Data_Rec();										
        }													
    }	  
    NbRsv_len1 = NbRsv_len2;//GetRxCount(NB_COM);	  
}      
/*******************************(END OF FILE)**********************************/

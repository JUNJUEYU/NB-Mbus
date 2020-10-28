/*
*******************************************************************************
*
*                           the app_NBIOT file
*                  ST Microelectronics STM32 with the STM32L151
*
* Filename      : app_NBIOT.c
* Version       : V1.0.0
* Programmer(s) : wubo
*******************************************************************************
*/

#include "includes.h"

#define     NBTR_DELAY0                 40
#define     NBTR_DELAY1                 100             // NB上传延时1
#define     NBTR_DELAY2                 200             // NB上传延时2
#define     NBTR_DELAY3                 300             // NB上传延时3
//------------------------------------------------------------------------------//
#define     NBERRCNT_MAX                10              // nb模块错误次数
#define     NBRECONN_MAX                2               // nb重新连接次数
#define     USENDMAX                    320             // NB上传的字符数  
//------------------------------------------------------------------------------//  
__IO        uint32_t    gulNBTransRtc   = 0;            // NB上传时间备份
//------------------------------------------------------------------------------//
static  uint16_t    guNBConDlyCnt       = 0;            // NB联网延时计数
static  uint16_t    guUpLoadDlyCnt      = 0;            // NB上传延时计数
static  uint8_t     gucLoopCnt          = 0;            // 重新连接次数
static  uint8_t 	trans_history_date	= 0;            // 正在传输历史数据
//------------------------------------------------------------------------------//
extern uint16_t	sleeping ;
extern	__IO		uint8_t     record_point;
static uint8_t  Nb_SendAsc(uint8_t *pch, uint16_t len, uint16_t *pw, uint16_t dly);
//------------------------------------------------------------------------------// 


/*******************************************************************************
* Function Name  : NbiotSum
* Description    : 与后台交换数据时，采用的校验和  
* Input          : *pch 数组首地址	n 计算和校验位置  
* Output         : None
* Return         : sum 校验和
*******************************************************************************/
uint8_t NbiotSum(uint8_t *pch,uint16_t n)
{
    uint16_t    i;
    uint8_t     sum = 0;
    
    for(i = 0; i < n; i++) {
      sum += pch[i];
    }
    return sum;  
}
/*******************************************************************************
* Function Name  : NB_Con_Delay_cnt
* Description    : 按次数进行延时
* Input          : *pw 计数器地址	dly 延时次数
* Output         : *pw 计数+1，延时结束后清0
* Return         : 0 延时继续	1 延时结束
*******************************************************************************/
uint8_t NB_Con_Delay_cnt(uint16_t *pw,uint16_t dly)
{   
    *pw += 1;    
    if(*pw < dly)
	{                    
        return 0;
    }
    else
    {
        *pw = 0;
        return 1;
    }      
}

/*******************************************************************************
* Function Name  : TransErrCnt
* Description    : 联网阶段，多次都不成功，则直接停止联网，不再操作
* Input          : None
* Output         : gstuNbSta.errcnt 计数+1
* Return         : None
*******************************************************************************/
void TransErrCnt(void)
{         
    gstuNbSta.errcnt += 1;        
    if(gstuNbSta.errcnt >= NBERRCNT_MAX)
    {           
        gstuNbSta.errcnt -= 1;        
        gucLoopCnt = NBRECONN_MAX;          // 直接设最大循环次数    
		gstuNbSta.nsocl = 1; 		
//        gstuFlag.mbNbEn = 0;                // 清除NB使能标志       
#if DBGMODE 
        printf("NB Regist Fail.\r\n");
#endif          
    }                
}

/*******************************************************************************
* Function Name  : At_SendStr
* Description    : At指令发送
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t At_SendStr(uint8_t *pch, __IO uint8_t *stt,uint16_t *pw,uint16_t dly)                        
{     
    if(*pw == 0)
    {      
        *stt  = 0;
        comSendBuf(NB_COM,(uint8_t *)pch, strlen((char *)pch));     
#if DBGMODE    
        comSendBuf(DBGCOM,(uint8_t *)pch, strlen((char *)pch));   
#endif    
    }
    
    *pw += 1;         
    if(*pw < dly)                   //while(dly--)    
    {                    
        if(*stt == 0) {             // 未响应       
            return 0;
        }  
        else                        // 有响应
        {  
            *pw  = 0;  
            *stt = 0;               // 清除状态标志                                        
            return 1;
        }
    }   
    else 
    {
        *pw = 0;        
        TransErrCnt();        
        return 0;
    }  
}


/*******************************************************************************
* Function Name  : NB_Reset
* Description    : 复位NB
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NB_Reset(void)
{
    NBRESET_ON();       // 复位开始
    Delay_ms(20);
    NBRESET_OFF();      // 结束复位       
    memset((uint8_t *)&gstuNbSta,   0,      sizeof(STUNBSTA));   
}    

/*******************************************************************************
* Function Name  : Nb_ResetDelay
* Description    : 自带延时参数的复位
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t Nb_ResetDelay(uint16_t *pw, uint16_t dly)
{    
    *pw  += 1;
    if(*pw < 3) {    
        NBRESET_ON();     
    }
    else {    
        NBRESET_OFF();     
    }
    
    if(*pw < dly) {    
        return 0;
    }
    else 
    {
        *pw = 0;
        memset((uint8_t *)&gstuNbSta,   0,  sizeof(gstuNbSta));  
        return 1;
    }
}

/*******************************************************************************
* Function Name  : comHex2Str
* Description    : hex转asc，并从com口发送 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void comHex2Str(COM_PORT_E port,uint16_t val)
{    
    uint16_t    u2tmp   = 0;
    uint8_t     uct[4]  = {0};      // 最多9999    
    uint8_t     len     = 0;        // 10进制字符串的个数
    uint8_t     i       = 0;
    
    u2tmp = val % 10000;
    //------------------------------//           
    if(u2tmp < 10)                  // 数据位数
        len = 1;
    else if(u2tmp < 100)
        len = 2;
    else if(u2tmp < 1000)
        len = 3;
    else if(u2tmp < 10000)
        len = 4;        
    //------------------------------//       
    u2tmp %= 10000;    
    uct[0] = (uint8_t)(u2tmp / 1000);    
    
    u2tmp %= 1000;        
    uct[1] = (uint8_t)(u2tmp / 100);
    
    u2tmp %= 100;        
    uct[2] = (uint8_t)(u2tmp / 10);
    
    u2tmp %= 10;            
    uct[3] = (uint8_t)u2tmp;
    //------------------------------//      
    for(i = 0; i < 4; i++) {
        uct[i]  += 0x30;            //转换成ASC码      
    }   
    comSendBuf(port,    (uint8_t *)&uct[4-len],     len);
#if DBGMODE
    comSendBuf(DBGCOM,  (uint8_t *)&uct[4-len],     len);    
#endif    
}

/*******************************************************************************
* Function Name  : NB_AT_NSOCO
* Description    : 远程连接IP端口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t NB_AT_NSOCO(COM_PORT_E port,__IO uint8_t *stt,uint16_t *pw, uint16_t dly)
{              
    uint16_t    u2tmp = 0;    
    uint16_t    i = 0;
          
    if(*pw == 0)
    {   
        *stt = 0;                       // 先清除标志位     
        // 1.前导符
        comSendBuf(port,	(uint8_t *)"AT+NSOCO=0,",   strlen("AT+NSOCO=0,"));  
    #if DBGMODE    
        comSendBuf(DBGCOM,  (uint8_t *)"AT+NSOCO=0,",   strlen("AT+NSOCO=0,"));       
    #endif    
        // 2.数据内容     
        for(i = 0; i < 4; i++)
        {
            u2tmp = gstuWmPara.IP_ADDR[i];
            comHex2Str(port,    u2tmp);     // 转换后同时发送出去
            if(i < 3) 
            {       
                comSendBuf(port,    (uint8_t *)".",     1);    
    #if DBGMODE     
                comSendBuf(DBGCOM,  (uint8_t *)".",     1);       
    #endif            
            }
        }
        // 3.分割符 // 1个','
        comSendBuf(port,    (uint8_t *)",",             1);    
    #if DBGMODE   
        comSendBuf(DBGCOM,  (uint8_t *)",",             1);       
    #endif
        // 4.端口号      
        comHex2Str(port,    gstuWmPara.mu2Port);        // ??
        // 5.结束符
        comSendBuf(port,    (uint8_t *)"\r\n",          2);
    #if DBGMODE    
        comSendBuf(DBGCOM,  (uint8_t *)"\r\n",          2); 
    #endif       
    }
    //-----------------------------//             
    if(dly < 1) {
        return 0;
    }    
    *pw += 1;         
    if(*pw < dly)                   //while(dly--)    
    {                    
        if(*stt == 0) {             // 未响应        
            return 0;
        }  
        else                        // 有响应
        {  
            *pw  = 0;  
            *stt = 0;               // 清除状态标志                                           
            return 1;
        }
    }   
    //-----------------------------//   
    return 0;
}


/*******************************************************************************
* Function Name  : NB_Connect
* Description    : 按流程自动连接网络 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

uint8_t NB_Connect(void)
{  
    uint16_t *pw = 0;    
    uint8_t  uct = 0;  
    uint8_t  ret = 0;
    
    pw = (uint16_t *)&guNBConDlyCnt;        //延时计数寄存器
    //--------------------------------------------------//
    if(gstuNbSta.nsocl > 0)                 // 请求关闭socket                 
    {           
        *pw = 0;
        gstuNbSta.nsocl = 0;     
        gstuNbSta.steps = 0; 
        memset((uint8_t *)&gstuNbSta,   0,  sizeof(gstuNbSta));            
        //------------------------------//            
        gucLoopCnt += 1;             // 重联次数+1
        if(gucLoopCnt >= NBRECONN_MAX)          
        {
//            gucLoopCnt = NBRECONN_MAX;
			gucLoopCnt = 0;
            retrans_cnt += 1;
            if ((trans_history_date == 0) & (retrans_cnt > RETRANS_MAX))
            {
                SaveRecord(&gstuNbFlowData);
            }
            gstuFlag.mbNbEn = 0;    // 清楚NB使能标志           
#if DBGMODE 
            printf("NB Connect %d Times.\r\n",NBRECONN_MAX);               
#endif           
            return 0;
        }            
        //------------------------------//         
    }        
    //--------------------------------------------------//
    ret = 0;
    uct = gstuNbSta.steps;                        
    if(uct >= NBTRANS_STEP)                 // 联网业务已成功
    {               
        gucLoopCnt = 0;
        return 0;
    }
    
    switch(uct)
    {    
        case 0:     // 发送AT指令	          
            ret = Nb_ResetDelay(pw,200);    // 硬件复位            
            //ret = At_SendStr((uint8_t *)"AT\r\n",               &gstuNbSta.ackok,   pw,     NBTR_DELAY1);                  
            break;    
#if 1            
        case 1:     // 发送AT指令            
            //ret = At_SendStr((uint8_t *)"AT\r\n",               &gstuNbSta.ackok,   pw,     NBTR_DELAY1);   
            //AT+NATSPEED=9600,30,1,X,1,0,0     (X= 0,1,2,3,可以用1,缓解频率偏移)
            ret = At_SendStr((uint8_t *)"AT+NATSPEED=9600,30,1,1,1,0,0\r\n",        &gstuNbSta.ackok,   pw,     NBTR_DELAY1);      
			gucCsqValue = 0;			
            break;                 
#else
        case 1:     // 软件重启模块   
            ret = At_SendStr((uint8_t *)"AT+NRB\r\n",           &ret,               pw,     NBTR_DELAY2);  
            if(*pw > 190)
            {
                ret = 1;
            }             
            break; 
#endif
 
        case 2:     // 禁止eDRX使用
            ret = At_SendStr((uint8_t *)"AT+NPTWEDRXS=0,5\r\n",                 &gstuNbSta.ackok,   pw, NBTR_DELAY1);    
            break;   
                        
        case 3:     // 设置PSM                                          
            ret = At_SendStr((uint8_t *)"AT+CPSMS=1,,,01000011,00000011\r\n",   &gstuNbSta.ackok,   pw,NBTR_DELAY1);                       
            break;            
                
        case 4:                               
#if 0        
            // ICCID            
            ret = At_SendStr((uint8_t *)"AT+NCCID\r\n",     &gstuNbSta.ackok,       pw,     NBTR_DELAY1);    
#endif
            // IMSI
            ret = At_SendStr((uint8_t *)"AT+CIMI\r\n",      &gstuNbSta.cimi,        pw,     NBTR_DELAY2);     
            break;
                      
        case 5:                  
            ret = At_SendStr((uint8_t *)"AT\r\n",           &gstuNbSta.ackok,       pw,     NBTR_DELAY1); 
            break;  
            
        case 6:    // 设置错误码应答
            ret = At_SendStr((uint8_t *)"AT+CMEE=1\r\n",    &gstuNbSta.ackok,       pw,     NBTR_DELAY1);                 
            break;             
                
        case 7:     // 使能信号变更时自动发送
            ret = At_SendStr((uint8_t *)"AT+CSCON=1\r\n",   &gstuNbSta.ackok,       pw,     NBTR_DELAY1);    
            break;   
            
        case 8:     //查询信号状态
            ret = At_SendStr((uint8_t *)"AT+CSCON?\r\n",    &gstuNbSta.cscon,       pw,     NBTR_DELAY1);     
            break;
                        
        case 9:     // 查询信号 		
            ret = At_SendStr((uint8_t *)"AT+CSQ\r\n",       &gstuNbSta.cscon,       pw,     NBTR_DELAY2);                                    
            ret = 0;
            if(gucCsqValue < 40)    // 判断信号强度
            {  
                if(gucCsqValue > 0) {
                    ret = 1;              
                }
            }                                                       
            break;
                           
        case 10:     // 联网状态查询
            ret = At_SendStr((uint8_t *)"AT+CEREG?\r\n",    &gstuNbSta.cereg,       pw,     NBTR_DELAY2);              
            break;

        case 11:     // 模块激活查询
            ret = At_SendStr((uint8_t *)"AT+CGATT?\r\n",    &gstuNbSta.cgatt,       pw,     NBTR_DELAY1);        
            break;
                       
        case 12:    // 同步时钟    
            ret = At_SendStr((uint8_t *)"AT+CCLK?\r\n",                         &gstuNbSta.cclk,    pw,     NBTR_DELAY1);                 
            break; 

        case 13:    // 创建socket
            ret = At_SendStr((uint8_t *)"AT+NSOCR=STREAM,6,6006,2\r\n",         &gstuNbSta.ackok,   pw,     NBTR_DELAY1); 
            
            if(*pw >= (NBTR_DELAY1-10))
            {
                if(ret == 0)     
                {
                    *pw = 0;
                    gstuNbSta.nsocl = 1;    // 关闭socket,重新开始                               
                }
            }
            break;      

        case 14:    // 连接远程端口            
            ret = NB_AT_NSOCO(NB_COM,  &gstuNbSta.nsoco,   pw, NBTR_DELAY3);    // delay:300*30 = 10s                 
            if(*pw >= (NBTR_DELAY3 - 10))
            {
                if(ret == 0)     
                {
                    *pw = 0;
                    gstuNbSta.nsocl = 1;    // 关闭socket,重新开始                    
                }
            }            
            break;    

        case 15:    // 配置ASC模式发送
            ret = At_SendStr((uint8_t *)"AT+NSOCFG=0,1,1\r\n",                  &gstuNbSta.ackok,   pw,     NBTR_DELAY1);                 
            //-----------------------------------//
            guUpLoadDlyCnt = 0;                 // wubo:上传延时清0
            //-----------------------------------//
            break; 
            
        default:
            break;
    }          
      
    if(ret > 0) 
    {           
        *pw = 0;              
        gstuNbSta.steps += 1; 
        gstuNbSta.steps &= 0x1f;
        gstuNbSta.errcnt = 0;               
    }    
	if( gstuNbSta.steps > 15) 
	{
		return  gstuNbSta.steps;
	}
    return  gstuNbSta.steps;
}    
  
/*******************************************************************************
* Function Name  : Hex2Asc
* Description    : 半个字节转ASC   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static uint8_t HToA(uint8_t h)
{
    uint8_t a = 0;
    
    a = h & 0x0f;
    
    if(a < 10) {
        a += 0x30;
    }
    else {
        a += 0x37;   
    }
    return a;
}

/*******************************************************************************
* Function Name  : NbSendStr
* Description    : NB模块发送数据   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void NbSendStr(COM_PORT_E port, uint8_t *pch, uint16_t n)
{      
    uint16_t i = 0;
    uint8_t  ustr[4] = {0};
    
    if(n == 0) {
        return;
    }       
    // 1.前导符
    comSendBuf(port,    (uint8_t *)"AT+NSOSD=0,",   strlen("AT+NSOSD=0,"));  
#if     DBGMODE 
    comSendBuf(DBGCOM,  (uint8_t *)"AT+NSOSD=0,",   strlen("AT+NSOSD=0,"));       
#endif    
    // 2.数据长度                   // 转换成10进制          
    comHex2Str(port,    n);        // 转换后同时发送出去
    // 3.分割符  (1个',')
    comSendBuf(port,    (uint8_t *)",",             1);          
#if     DBGMODE 
    comSendBuf(DBGCOM,  (uint8_t *)",",             1);   
#endif       
    // 4.????     
    for(i = 0; i < n; i++)
    {
        ustr[0] = HToA(pch[i]>>4);          // 转ASC码
        ustr[1] = HToA(pch[i]&0x0f);        // 转ASC码
        comSendBuf(port,    (uint8_t *)ustr,        2); 
#if     DBGMODE 
        comSendBuf(DBGCOM,  (uint8_t *)ustr,        2);  
#endif          
    }          
    // 5.分割符  (2个',')
    comSendBuf(port,    (uint8_t *)",,",            2);    
#if     DBGMODE   
    comSendBuf(DBGCOM,  (uint8_t *)",,",            2);       
#endif       
    // 6.发送序列号 
    gstuNbSta.index++;          // 从1开始 (0不好用)
    comHex2Str(port,    gstuNbSta.index);             
    // 7.结束符
    comSendBuf(port,    (uint8_t *)"\r\n",          2);
#if     DBGMODE 
    comSendBuf(DBGCOM,  (uint8_t *)"\r\n",          2); 
#endif       
    //-----------------------------//   
}

/*******************************************************************************
* Function Name  : Nb_SendAsc
* Description    : hex转换成ASC后数据，发送ASC数据；无论成功与否，都延时dly
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static uint8_t  Nb_SendAsc(uint8_t *pch, uint16_t len, uint16_t *pw, uint16_t dly)
{  
    if(len > USENDMAX) {                // 根据协议传输的最大数量        
        return 0;
    }  
        
    if(*pw  == 0) 
    {
#if 0      
        Hex2Asc(pch,    UCH,    len);   // 把pch转换成asc字符
        NbSendStr(NB_COM, (uint8_t *)UCH, len);    
#else
        NbSendStr(NB_COM, (uint8_t *)pch, len);    
#endif          
    }
    //----------------------//
    *pw += 1;   
    if(*pw < dly)                  
    {     
        return 0;
    }
    else            
    {
        *pw = 0;
        return 1;
    }
    //------------------------------//     
}

/*******************************************************************************
* Function Name  : NB_SendHex
* Description    : At发送数据，只等待延时 ,无须应答
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t NB_SendHex(uint8_t *pch,uint16_t *pw,uint16_t dly)                        
{     
    if(*pw == 0)
    {
        comSendBuf(NB_COM,(uint8_t *)pch, strlen((char *)pch));     
#if     DBGMODE    
        comSendBuf(DBGCOM,(uint8_t *)pch, strlen((char *)pch));   
#endif    
    }
    
    *pw += 1;         
    if(*pw < dly)                    
    {                    
        return 0;
    }   
    else 
    {
        *pw = 0;
        return 1;
    }   
}

/*******************************************************************************
* Function Name  : NB_DataUpload
* Description    : NB数据上传,无须应答
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t NB_DataUpload(void)
{
    
    uint16_t    *pw = NULL;   
    uint8_t     uct = 0;  
    uint8_t     ret = 0;
	
    pw = (uint16_t *)&guUpLoadDlyCnt;    // 延时计数    
    uct = gstuNbSta.steps;  			 // 流程步进  
	
    switch(uct)
    {    
        case NBTRANS_STEP:               //上传实时数据                                            
//			if(*pw == 0)
//			{
//				FillFlowData(&gstuNbFlowData);
//				FillFlowTail(&gstuNbFlowData);
//			}
			
			ret = Nb_SendAsc((uint8_t *)&gstuNbFlowData,    sizeof(STUNBFLOWDATA),  pw,     NBTR_DELAY3);   
			if(gstuNbSta.nbrxd)         // 成功标志
			{
				ret = 1;
				gstuNbSta.nbrxd = 0;
			}                           
            break;    
          
       
        case NBTRANS_STEP+1:            // 上传存储数据
            trans_history_date = 1;
            if(*pw == 0)
            {
                if(ReadRcdHead())
                {
                    ret = 1;
					trans_history_date = 0;   
                    break;
                }
                record_point = ReadRecord(&gstuNbFlowData);
                if (record_point == 0)
                {
                    ret = 1;
					trans_history_date = 0;   
                    break;
                }
            }

			
            Nb_SendAsc((uint8_t *)&gstuNbFlowData,    sizeof(STUNBFLOWDATA),  pw,     NBTR_DELAY3);
            if(gstuNbSta.nbrxd)
            {
                ChgRcdHead(record_point);
                gstuNbSta.nbrxd = 0;
                *pw = 0;
            }   
            break;   
          
        case NBTRANS_STEP+2:        // 延时等待                            
            ret = At_SendStr((uint8_t *)"AT\r\n",           &gstuNbSta.ackok,   pw,     NBTR_DELAY1);                     
            break;
            
        case NBTRANS_STEP+3:        // 断开结束                                     
            ret = At_SendStr((uint8_t *)"AT+NSOCL=0\r\n",   &gstuNbSta.ackok,   pw,     NBTR_DELAY1);    
            break;      
            
        case NBTRANS_STEP+4:        // 结束     
			sleeping = 1;
			Delay_ms(500);
			sleeping = 0;
			gstuFlag.mbNbEn     = 0;    // 使能NB标志                      
			gstuNbSta.steps     = 0;
			gstuNbSta.errcnt    = 0;        
            retrans_cnt         = 0;
            break;              
        default:
            break;        
    }
           
    if(ret > 0) 
    {           
        *pw = 0;      
        gstuNbSta.steps += 1; 
        gstuNbSta.steps &= 0x1f;
        
        gstuNbSta.errcnt = 0;   // ??????
    }    
   
    return  gstuNbSta.steps;    
}

/*******************************************************************************
* Function Name  : NB_Close
* Description    : NB上传结束后，清除结构体数据
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NB_Close(void)
{    
    if(gstuFlag.mbNbEn == 0)                // NBNB已发送完毕
    {
        // NB上传结束，清除所有NB状态          
        memset((uint8_t *)&gstuNbSta,   0,  sizeof(gstuNbSta));
    }
}


/*******************************(END OF FILE)**********************************/

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
                                            "+CSCON",       // �ź�ͨ��״̬                                                                                
                                            "+CEREG",       // �����ɹ����                         
                                            "+CGATT",       // ģ�鼤�����
                                            "+NSOCLI",      // �ر�SOCKET��Ϣ
                                            "CONNECT OK",   // Զ�����ӳɹ�
                                            "+CME ERROR",   // ģ�鱨��                             
                                            "ERROR",        // ָ�����                                    
                                            "+NSOSTR",      // ���ݷ��ͳɹ�
                                            "+NSORF",       // ���ݽ��ճɹ�                                    
                                            "+CSQ",         // �ź�����
                                            "+CCLK",        // ʼ������
                                            "460"          // ISMI��                       
                                        };    
                                

static      uint8_t     ucom[UART2_RX_BUF_SIZE]         = {0};      // �����յ����ַ�
#if 0
static      uint8_t     datbuf[UART2_TX_BUF_SIZE]       = {0};      // hexתasc������
#endif
static      uint8_t     gucNbCard[8]        = {0};      // NB���ż��ź�


__IO        STUNBSTA    gstuNbSta;                      // NB״̬��
__IO        uint8_t     gucCsqValue         = 0;        // �ź�����
  

/*******************************************************************************
* Function Name  : At_SendStr
* Description    : Atָ���   
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
* Description    : �õ�NBiot���ص��ַ���,��ת����hex�ַ� 
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
				return length;	// �����ֽ���
			} 			
		}
	}
	return 0;
}


/*******************************************************************************
* Function Name  : Parch
* Description    : Atָ���   
* Input          : ʶ����Ч����
* Output         : pchΪ���յ��ַ�����cΪ�ַ�������
* Return         : None
*******************************************************************************/
#if 0
void Parch(uint8_t *pch,uint16_t c)
{   
	uint16_t    len = 0;
    uint8_t     uct = 0;
    

    STUCLOCKSYNC mstuClock;
    
    len = AscReConv(pch,datbuf,c);          // �ֽ��Э������    
    
	if(len > 3)                             // for(i = 0;i < len;i++)
    {
        if(datbuf[0] == 0x38 || datbuf[0] == 0x7B)
        {            
			//length = (uint16_t)datbuf[1] + ((uint16_t)datbuf[2]<<8);
			
            if(datbuf[len-1] == 0x7D)       // ��������ַ�
            {                       
                switch(datbuf[3])
                {
                    case 0xA5:	// ����������Ӧ��
                        break;
                    
                    case 0x81:	// ˮ����������
						//xSemaphoreGive(xSem_NbDataReq);																  
                        break;
				
                    case 0x83:	// ����ˮ���ַ
                        uct = NbiotSum((uint8_t *)datbuf + 1, len - 3);     
                        if(uct == datbuf[len-2])
                        {
                            memcpy((uint8_t *)&tstuWmTab,  (uint8_t *)datbuf,  len);                            
                            //xQueueSendToBack(xQue_WmTab,( void * )&tstuWmTab,0);                                                                
                        }
                        break;
                  						
                    case 0x15: 	//����ʱ��  
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
* Description    : ��ȡNBʱ������(СʱҪ��8Сʱ)
* Input          : pchΪ���յ��ַ�����cΪ�ַ�������
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
   
    if(pch[23] == '+')  // ���ж�һ��
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
* Description    : ƥ��NB���ص�����   
* Input          : pchΪ���յ��ַ�����cΪ�ַ�������
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
    if(pch[0] == 'O' && pch[1] == 'K')          // ����"OK"
    {
        DISABLE_INT();  
        gstuNbSta.ackok = 1;                    // Ӧ���־                
        ENABLE_INT();
        return 0;
    }
    //==============================================//
    row = sizeof(NBACK) / sizeof(NBACK[0]);     // �����ά��     
    for(i = 0; i < row; i++)
    {                 
        len =  strlen((char *)NBACK[i]);        // ��ǰ�ַ�����
        for(j = 0; j < len; j++)
        {
            // �����ַ�����NB�����ַ��Ƚ�
            if(pch[j] != NBACK[i][j])
            {
                break;
            }                    
        }
        
        if(j == len)        // ƥ��ɹ�
        {          
            row = i;        // �ظ������            
            status = 0x5555;// �ɹ�ʶ���־
            break;
        }
    }               
 	//==============================================//
    if(status == 0x5555)    // �����ַ���ʶ��ɹ�
    {      
        DISABLE_INT();         
        switch(row)
        {
            case 0: //"+CSCON",        
                uct = pch[c-5];                 // ���ϡ�\r\n����������5���ַ�
                if(uct == '1' ) 
                {                    
                    gstuNbSta.cnnct = 1;        // �ź�ͨ(��ʱû�õ�)
                    gstuNbSta.cscon = 1;        // �ź�����Ӧ��  
                }
                else 
                {
                    gstuNbSta.cnnct = 0;        // �źŶϣ���ʱû�õ���
                    gstuNbSta.cscon = 0;        // �ź�δ����
                }                                               
                break;
                                 
            case 1: //"+CEREG",   
                uct = pch[c-3];                 // ���ϡ�\r\n����������3���ַ�
                if(uct == '1' || uct == '5') {
                    gstuNbSta.cereg = 1;        // �����ɹ�
                }
                else {
                    gstuNbSta.cereg = 0;
                }                   
                break;
                                  
            case 2: //"+CGATT",
                 uct = pch[c-3];                // ���ϡ�\r\n����������3���ַ�
                if(uct == '1')  {
                    gstuNbSta.cgatt = 1;        // ģ�鼤��
                }
                else {                
                    gstuNbSta.cgatt = 0;
                }
                break;
            
            case 3: //"+NSOCLI"                 // NBģ����ʾ�ر�socket����ʾ�޷�����Զ��IP�˿�                     
                gstuNbSta.nsocl = 1;            // ����
                break;
            
            case 4: //"CONNECT OK"                               
                gstuNbSta.nsoco = 1;            // ����Զ�̳ɹ�             
                break;
            
            case 5: //"+CME ERROR"              // ����   
                gstuNbSta.nsocl = 1;            // ����
                gstuNbSta.ackerr = 1;           // errӦ��                              
                break;            
            
            case 6: //"ERROR"                   // ָ�������ظ�             
                gstuNbSta.ackerr = 1;         
                break;
                 
            case 7: //"+NSOSTR"                 // nb�������
                status = TXDFLAG;               
                gstuNbSta.nbtxd = 1;          
                break;
            
            case 8: //"+NSORF"                  // nb�������
                status = RXDFLAG;   
                gstuNbSta.nbrxd = 1;                 
                break;   
          
            case 9: //"+CSQ"                    // �ź�ǿ��                           
                uct = Asc2Hex(pch[5]);
                if(pch[6] >= 0x30 && pch[6] <= 0x39) // �����λ������
                {                
                    uct *= 10;
                    uct += Asc2Hex(pch[6]);		
                } 
                gucCsqValue = uct;              // �ź�ǿ��
                gstuNbSta.csq = 1;            
                break; 
              
            case 10://"+CCLK"                    // ʱ������                
                gstuNbSta.cclk = 1;                         
                
                GerNbClock(pch,c);              // ��ȡNBʱ������                                                                        
                break;   
                
            case 11://"cimi"                    // ����ʶ��(460040101900126)
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
    else     // ���ݽ���
    {
        status = 0;                                   
    }    
    return status;
}


/*******************************************************************************
* Function Name  : GetCardImsi
* Description    : ����ʶ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetCardImsi(void)
{
    uint8_t i = 0;
    for(i = 0; i < 8; i++) {            // �ƶ�����
        gstuNbPara.mu1Imsi[i] = gucNbCard[i];   
    }
    gstuNbPara.mu1Csq = gucCsqValue;    // �ź�ǿ��
    gstuNbPara.mu1Check = CheckSum((uint8_t *)&gstuNbPara,(uint16_t)(sizeof(STUNBSET) - 1));                          
}

/*******************************************************************************
* Function Name  : Nb_Data_Rec
* Description    : �����NB�ش�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Nb_Data_Rec(void)
{
    uint16_t count = 0;  
    uint16_t status = 0;
    
    count = myMemCpy(NB_COM,ucom);	

    if(count > 2)                           // ���ٰ���"\r\n"
    {		                                             
        status = MsgDecode(ucom,count);	    // ����COM���ַ�

        if(status == RXDFLAG) 
        {                     
            //Parch(ucom,count);              // ʶ����Ч����  
        }
        
#if     DBGMODE                   
        if(count <= UART1_TX_BUF_SIZE) 
        {                   
            comSendBuf(DBGCOM,ucom,count);  // ��ӡ�ַ�                                       
        }
#endif 
    }
}


static  uint16_t  NbRsv_len1 = 0;
static  uint16_t  NbRsv_len2 = 0;

/*******************************************************************************
* Function Name  : ProcNbComRec
* Description    : �����NB�ش�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProcNbComRec(void)
{
    NbRsv_len2 = GetRxCount(NB_COM);                           
    if((NbRsv_len1 > 0) && (NbRsv_len1 == NbRsv_len2))	    // COM��������
    {									
        if(NbRsv_len1 <= UART2_RX_BUF_SIZE)                 // �ַ�����������
        {	                       
            Nb_Data_Rec();										
        }													
    }	  
    NbRsv_len1 = NbRsv_len2;//GetRxCount(NB_COM);	  
}      
/*******************************(END OF FILE)**********************************/

/*
*******************************************************************************
*
*                           the APP_BLUE file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : APP_BLUE.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "includes.h"

#define BL_LEN      (256)

__IO    STUNBSET    gstuNbPara;                     // ???????
static  uint8_t     gucBuf[BL_LEN] = {0};           // ???????
static  uint16_t    blue_len1,blue_len2;
//------------------------------------------------------------------------------// 
__IO    uint8_t     gu1NbSetFlag  = 0;              // NB????
__IO    uint8_t     gu1NbTrgFlag  = 0;              // NB????     
//------------------------------------------------------------------------------// 
/*******************************************************************************
*	? ? ?: NbSetPara
*	????: ??????NB???????
*	?    ?: cnt???
*	? ? ?: ?
*******************************************************************************/
static uint8_t NbSetPara(uint16_t cnt)
{
    //STUNBSET    tstuNbPara; 
    uint16_t    count = 0;    
    uint8_t     ret = 0;  
	uint8_t     i;  
	uint8_t     addr_buf[100]; 
    
    count = cnt;
    if(count == sizeof(STUPARARD))                  // ???
    {
        if(gucBuf[0] == NBRD_HEAD)                  // ???
        {                                
            if(CheckSum(gucBuf,count) == 0)                            
            {                                  
                if(gucBuf[2] == NBCN_CMD)           // ??NB??
                {
//                    gstuFlag.mbHndF = 1;            // ????
                    gstuFlag.mbTrgF = 1;            // ????   
                }
                else if(gucBuf[2] == NBRD_CMD)     // ?NB??
                {  
                    gstuNbPara.mu1Head = NBRD_HEAD;
                    gstuNbPara.mu1Len =  sizeof(STUNBSET);
                    //------------------------------//
                    gstuNbPara.IP_ADDR[0] = gstuWmPara.IP_ADDR[0];
                    gstuNbPara.IP_ADDR[1] = gstuWmPara.IP_ADDR[1];
                    gstuNbPara.IP_ADDR[2] = gstuWmPara.IP_ADDR[2];
                    gstuNbPara.IP_ADDR[3] = gstuWmPara.IP_ADDR[3];
                    gstuNbPara.mu2Port  = gstuWmPara.mu2Port;                    
                    gstuNbPara.mu2DtuID = gstuWmPara.mu2DtuID;
					gstuNbPara.mu2CjbID = gstuWmPara.mu2CjbID;
					gstuNbPara.mu1TxMax = gstuWmPara.mu1TxMax;
                    //------------------------------//    
                    GetCardImsi();                  // NB???????                      
                    comSendBuf(BL_COM,(uint8_t *)&gstuNbPara,sizeof(STUNBSET));                                                                                                       
                }
            }
        }
    }
    else if(count == sizeof(STUNBSET))              // ???                       
    {
        if(gucBuf[0] == NBRD_HEAD)
        {                                
//            if(CheckSum(gucBuf,count) == 0)                            
//            {   
                memcpy((uint8_t *)&gstuNbPara,(uint8_t *)gucBuf,count);    
                //--------------------------------------------------//
                gstuWmPara.mu2DtuID   = gstuNbPara.mu2DtuID;            // DTUID
				gstuWmPara.mu2CjbID   = gstuNbPara.mu2CjbID;            // CJBID
                //--------------------------------------------------//                
                gstuWmPara.IP_ADDR[0] = gstuNbPara.IP_ADDR[0];
                gstuWmPara.IP_ADDR[1] = gstuNbPara.IP_ADDR[1];
                gstuWmPara.IP_ADDR[2] = gstuNbPara.IP_ADDR[2];
                gstuWmPara.IP_ADDR[3] = gstuNbPara.IP_ADDR[3];
                gstuWmPara.mu2Port = gstuNbPara.mu2Port;    
                //--------------------------------------------------//           
                gstuWmPara.mu1TxMax = gstuNbPara.mu1TxMax;
#if 0
				addr_buf[0] = gstuNbPara.mu1FlowMax;
				for(i = 0; i <71; i++)
				{
					addr_buf[i+1] = gstuNbPara.mu1FlowAddr[i];
				}
				SaveFlowAddr(addr_buf);
#endif
				//--------------------------------------------------//  
                comSendBuf(BL_COM,(uint8_t *)&gstuNbPara,sizeof(STUNBSET));                 
                SaveWMPara(&gstuWmPara);   
//            }
        }
    } 
	else
	{
	  	if(gucBuf[0] == BL_HEAD)
		{
			switch (gucBuf[3])
			{
				case 0x81:
					for(i =0; i < 71; i ++)
					{
						addr_buf[i] = gucBuf[i + 8];
					}
					SaveFlowAddr(addr_buf);
					comSendBuf(BL_COM,gucBuf,81);  
					break;
				case 0x82:
					if(ReadFlowAddr())
					{
						addr_buf[0] = 0x38;
						addr_buf[1] = 0x51;
						addr_buf[2] = 0x00;
						addr_buf[3] = 0x82;
						addr_buf[4] = gucBuf[4];
						addr_buf[5] = gucBuf[5];
						addr_buf[6] = gucBuf[6];
						addr_buf[7] = gucBuf[7];
						addr_buf[8] = addrcache.addrMax;
						for(i = 0; i < 70; i ++)
						{
							addr_buf[i + 8] = addrcache.addrFlow[i];
						}
						addr_buf[80] = 0x3D;
						
						comSendBuf(BL_COM,addr_buf,81);  
					}
					else
					{
						addr_buf[0] = 0x38;
						addr_buf[1] = 0x51;
						addr_buf[2] = 0x00;
						addr_buf[3] = 0x82;
						addr_buf[4] = gucBuf[4];
						addr_buf[5] = gucBuf[5];
						addr_buf[6] = gucBuf[6];
						addr_buf[7] = gucBuf[7];
						addr_buf[8] = 0;
						for(i = 0; i < 70; i ++)
						{
							addr_buf[i + 8] = 0xFF;
						}
						addr_buf[80] = 0x3D;
						
						comSendBuf(BL_COM,addr_buf,81);  
					}
									
			}
				
		}
	}
    return ret;    
}


void ProcBlueData(void)
{
    uint16_t cnt = 0;
 
    blue_len2 = GetRxCount(BL_COM);                           
    if((blue_len1 > 0) && (blue_len1 == blue_len2))	    
    {									
        if(blue_len1 <= BL_LEN)                 // ???????
        {	          
            cnt = comMemCpy(BL_COM,gucBuf);     // ??COM???                                                                     
            if(cnt > 0) 
            {                                               
                NbSetPara(cnt);
            }       					          
        }													
    }	  
    blue_len1 = blue_len2;	
}   
/*******************************(END OF FILE)**********************************/    



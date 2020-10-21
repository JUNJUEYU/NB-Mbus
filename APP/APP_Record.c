/*
*******************************************************************************
*
*                           the app_RECORD file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_RECORD.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/

#include "includes.h"

#define     RCD_DBG             (0)             // 打印记录
//-------------------------------------------------------------//
#define     PULSE_CAP           (256)           // 每组字节数
#define     PULSE_MAXCNT        (24)            // 脉冲最大组数
//-------------------------------------------------------------//
static      uint8_t             erom[256]       = {0};
static      STRRCDHEAD          gstuRcdHead;    // 数据记录头

FLOWADDRSET		addrcache;

//-------------------------------------------------------------//
   
/*
********************************************************************************
水量参数为16bytes,分成16组；
每组的最后一个字节为MARK值；
共占用256bytes的空间。
********************************************************************************
*/
void SaveHead(uint32_t pageaddr,STRRCDHEAD *pstu)
{
    uint32_t addr;      
	addr = ADDR_PAGE0;   
//============================================================================//       
    pstu->mu1Mark  = MARK_RCRD;           
//----------------------------------------------------------------------------//      

//----------------------------------------------------------------------------//     

    EepWr(addr,(uint8_t *)pstu,sizeof(STRRCDHEAD));   
//============================================================================//  
}



/*******************************************************************************
* Function Name  : ReadRcdHead
* Description    : 读集中器的设置参数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t ReadRcdHead(void)
{    
	uint32_t    addr;  
	uint8_t  i;

    if(isEEPRomOK>0)
    {    
		addr = ADDR_PAGE0; 
		EepRd(addr + sizeof(STRRCDHEAD) - 1, (uint8_t *)erom, 1);   
        if(erom[0] == MARK_RCRD)        // 读取记录标记
        {   
            EepRd(addr, (uint8_t *)erom, sizeof(STRRCDHEAD));                                        
            memcpy((uint8_t *)&gstuRcdHead, erom, sizeof(STRRCDHEAD));    
			return 0;                 
        }
		else
		{
			gstuRcdHead.mu1PageIn = 0;
			for(i=0;i<8;i++)
			{
				gstuRcdHead.mu1Rsv[i] = 0;
			}
			return 1;
		}
    } 
	return 1;  
}    

/*******************************************************************************
* Function Name  : SaveRcdHead
* Description    : 读集中器的设置参数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SaveRcdHead(STRRCDHEAD *pstu)
{
    uint32_t    addr = 0;        
    if(isEEPRomOK > 0)
    {
        addr = ADDR_PAGE0;
        SaveHead(addr,pstu);        // 记录区
    }
}  

// #define SetBit(x)           (uint32_t)(1UL<<(x&0x1f))


/*
********************************************************************************
脉冲采集板数据存储
********************************************************************************
*/
void SaveRecord(STUNBFLOWDATA *pstu)
{
    uint32_t    addr = 0;
    uint16_t    len = 0;
   //--------------------------//      
    ReadRcdHead(); 
	gstuRcdHead.mu1PageIn += 1;
	if(gstuRcdHead.mu1PageIn > 8)
	{
		gstuRcdHead.mu1PageIn = 8;
	}
	gstuRcdHead.mu1Rsv[gstuRcdHead.mu1PageIn - 1] = 1;
	SaveRcdHead(&gstuRcdHead);
	len = sizeof(STUNBFLOWDATA);
	memcpy(erom,(uint8_t *)pstu, len);  
	addr = RCRD_ADR2 + ((gstuRcdHead.mu1PageIn - 1)*256);
	EepWr(addr, (uint8_t *)erom, len);    // 写入eep
}

/*
********************************************************************************
脉冲采集板数据读取
********************************************************************************
*/
uint8_t ReadRecord(STUNBFLOWDATA *pstu)
{
    uint32_t    addr = 0;
    uint8_t     i = 0;
    
    // ReadRcdHead();
    for(i=8; i>0; i--)
	{
		if(gstuRcdHead.mu1Rsv[i - 1] == 1)
		{
			addr = RCRD_ADR2 + ((i - 1)*256);
			EepRd(addr,(uint8_t *)erom,sizeof(STUNBFLOWDATA));                                        
            memcpy((uint8_t *)&gstuNbFlowData, erom, sizeof(STUNBFLOWDATA));        
			return i; 
		}
	}    
    return 0;                           // 有记录
}   

// 读取成功后，修正记录总量
void ChgRcdHead(uint8_t u4id)
{
    gstuRcdHead.mu1Rsv[u4id - 1] = 0;
	gstuRcdHead.mu1PageIn -= 1;
	SaveRcdHead(&gstuRcdHead);     		
}  


uint8_t ReadFlowAddr(void)
{
	uint8_t i,len ;
	uint32_t	addr;
	uint8_t buf[256];
	
	addr = ADDR_PAGE5;
	EepRd(addr,(uint8_t *)buf,1);    // 读取地址标示位
	
	if(buf[0] != ADDR_ENABLE)
	{
		addrcache.addrEn = 0;
#if DBGMODE            
        printf("Addr Err.\r\n");
#endif  
		return 0;
	}
	
	else
	{
		len = sizeof(FLOWADDRSET);
		EepRd(addr,(uint8_t *)buf,len);
		
		addrcache.addrEn = buf[0];
		addrcache.addrMax = buf[1];
		for(i = 0; i < 70; i ++)
		{
			addrcache.addrFlow[i] = buf[i +2];
		}
#if DBGMODE            
        printf("Addr is OK.\r\n");
		Delay_ms(100);
		printf("AddrMax = %d.\r\n",addrcache.addrMax);
#endif  
		return 1;
	}
}

void InitFlowAddr(void)
{
	ReadFlowAddr();
}

void SaveFlowAddr(uint8_t *pnbrsv)
{
	uint32_t    addr;
	uint8_t buf[256];
	uint8_t i ,len;
	
//	len = sizeof(FLOWADDRSET);
	len = 72;
	addr = ADDR_PAGE5;
	buf[0] = addrcache.addrEn = ADDR_ENABLE;
	buf[1] = addrcache.addrMax = pnbrsv[0];
	for(i = 0; i < 70; i ++)
	{
		buf[i +2] = addrcache.addrFlow[i] = pnbrsv[i +1];
	}
	
	EepWr(addr ,(uint8_t *)buf,len);	
}

/*******************************(END OF FILE)**********************************/

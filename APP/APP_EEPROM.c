/*
*******************************************************************************
*
*                           the APP_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : APP_EEPROM.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/

#include "includes.h"


static uint8_t urom[256]        = {0};
//----------------------------------------------------------------------------//
STRWMTOT        gstuWmTot;              // ????
STRWMPARA       gstuWmPara;             // ????

uint8_t         isEEPRomOK  = 0;
//----------------------------------------------------------------------------//
void SaveWMPara(STRWMPARA *pstu);
void SaveTotFlow(STRWMTOT *pstu);
//----------------------------------------------------------------------------//

/*******************************************************************************
* Function Name  : EepWr
* Description    : EepWr
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t EepWr(uint32_t addr, uint8_t *pch, uint16_t n)
{
    uint32_t err = 0;

    E2WP_LOW();
    err = BSP_EE_WriteBytes(EE_DEV_ADDR,(uint8_t *)pch,(uint16_t)addr,n);				
    To_Enter_Wait(10);		
    E2WP_HIGH();				// protect
    
    err = 1;
    return err;
}

/*******************************************************************************
* Function Name  : EepRd
* Description    : EepRd
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t EepRd(uint32_t addr, uint8_t *pch, uint16_t n)
{
    uint32_t err = 0;
    err = BSP_EE_ReadBytes(EE_DEV_ADDR,(uint8_t *)pch,(uint16_t)addr,n);			
    err = 1;
    return err;
}		


void TestErom(void)
{
    uint8_t i = 0;
    for(i = 0; i < 32; i++) {
        urom[i] = i;
    }    
    EepWr(ADDR_PAGE1,urom,32);
    comSendBuf(DBGCOM,(uint8_t *)urom, 32); 
    //---------------------------------------------//  
    Delay_ms(50); 
    
    for(i = 0; i < 32; i++) {
        urom[i] = 0;
    }        
    EepRd(ADDR_PAGE1,urom,32);         
    comSendBuf(DBGCOM,(uint8_t *)urom, 32); 
    //---------------------------------------------//
}
/*
********************************************************************************
???
********************************************************************************
*/
uint8_t CheckSum(uint8_t *pch,uint16_t n)
{
    uint16_t    i;
    uint8_t     sum = 0;
    
    for(i = 0; i < n; i++)
      sum += pch[i];
    
    sum = 0xff - sum;
    return sum;  
}

/*
********************************************************************************
???????
********************************************************************************
*/
void SetZeroFlow(void)
{
    gstuWmTot.mu4TotVal = 0;
    gstuWmTot.mu4Pulse1 = 0;
    gstuWmTot.mu4Pulse2 = 0;
    gstuWmTot.mu1Err = 0;       // ???
    gstuWmTot.mu1Rsv = 0;
    gstuWmTot.mu1Check = CheckSum((uint8_t *)&gstuWmTot,(uint16_t)(sizeof(STRWMTOT) - 2));
    gstuWmTot.mu1Mark = MARK_CODE;
}        
        
/*
********************************************************************************
?????,2??????;

?????16bytes,??16?;
??????????MARK?;
???256bytes???
********************************************************************************
*/
void ClearTotFlow(void)
{
    uint32_t addr;
    uint16_t i;   
//============================================================================//     
    for(i = 0; i < 16; i++)
    {               
        addr = ADDR_PAGE2;     
        addr += (uint32_t)i * sizeof(STRWMTOT);
        // ????????
        EepRd(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);
        if(urom[0] == MARK_CODE)        // ????
        {   
            urom[0] = 0;                // ?????
            EepWr(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);                 
            break;
        }
    }  
//============================================================================// 
    for(i = 0; i < 16; i++)
    {               
        addr = ADDR_PAGE6;     
        addr += (uint32_t)i * sizeof(STRWMTOT);
        // ????????
        EepRd(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);
        if(urom[0] == MARK_CODE)        // ????
        {   
            urom[0] = 0;                // ?????
            EepWr(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);                 
            break;
        }
    }      
//============================================================================//         
    SetZeroFlow();                      // ???????             
//============================================================================//        
}


/*
********************************************************************************
?????16bytes,??16?;
??????????MARK?;
???256bytes????
********************************************************************************
*/
void SaveFlow(uint32_t pageaddr,STRWMTOT *pstu)
{
    uint32_t addr;
    uint16_t i,status;         
//============================================================================//       
    pstu->mu1Check = CheckSum((uint8_t *)pstu,(uint16_t)(sizeof(STRWMTOT) - 2));
    pstu->mu1Mark = MARK_CODE;           
//----------------------------------------------------------------------------//      
    status = 0;
    for(i = 0; i < 16; i++)             // ????16???
    {               
        addr = pageaddr;     
        addr += (uint32_t)i * sizeof(STRWMTOT);
        // ????????
        EepRd(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);
        if(urom[0] == MARK_CODE)        // ????
        {   
            urom[0] = 0;                // ?????
            EepWr(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);                 
            status = 0xff;              // ????
            break;
        }
    }          
//----------------------------------------------------------------------------//     
    if(status == 0) {           // ?????
        i = 0;
    }
    else //if(status == 0xff) 
    {   
        i += 1;                 // ????????
        i &= 0x0f;          
    }
    
    addr = pageaddr;            // ???
    addr += (uint32_t)i * sizeof(STRWMTOT);    
    EepWr(addr,(uint8_t *)pstu,sizeof(STRWMTOT));   
//============================================================================//  
}


/*
********************************************************************************
?????,?????
********************************************************************************
*/
uint8_t ReadTotFlow1(void)
{    
    uint32_t    addr;                
    uint16_t    i;
    uint8_t     uct;
//----------------------------------------------------------------------------// 
    // ??????
    for(i = 0; i < 16; i++)
    {      
        addr = ADDR_PAGE2;              // ???
        addr += (uint32_t)i * sizeof(STRWMTOT);
        // ???????????
        EepRd(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);
        if(urom[0] == MARK_CODE)        // ??????
        {   
            EepRd(addr,(uint8_t *)urom,sizeof(STRWMTOT));                
            uct = CheckSum(urom,(sizeof(STRWMTOT)-1));              
            if(uct == 0)                // ??OK              
            {                    
                memcpy((uint8_t *)&gstuWmTot, urom, sizeof(STRWMTOT));            
                return 1;               // ??????
            }
        }
    }      
//----------------------------------------------------------------------------//   
    // ??????,?????
    for(i = 0; i < 16; i++)
    {      
        addr = ADDR_PAGE6;              // ??? 
        addr += (uint32_t)i * sizeof(STRWMTOT);
        // ???????????
        EepRd(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);
        if(urom[0] == MARK_CODE)         // ??????
        {   
            EepRd(addr,(uint8_t *)urom,sizeof(STRWMTOT));
            uct = CheckSum(urom,(sizeof(STRWMTOT)-1));   
            if(uct == 0)                // ??OK   
            {    
                memcpy((uint8_t *)&gstuWmTot, urom, sizeof(STRWMTOT));     
                
                /*copy????page2*/
                addr = ADDR_PAGE2; 
                SaveFlow(addr,&gstuWmTot);                
                return 2;       // ??????
            }
        }
    }      
//----------------------------------------------------------------------------//      
    return 0;    
}

/*
********************************************************************************
?????,?????
********************************************************************************
*/
uint8_t ReadTotFlow2(void)
{    
    uint32_t    addr;                
    uint16_t    i;
    uint8_t     uct;
//----------------------------------------------------------------------------// 
    // ?????
    for(i = 0; i < 16; i++)
    {      
        addr = ADDR_PAGE6;              // ??? 
        addr += (uint32_t)i * sizeof(STRWMTOT);
        // ???????????
        EepRd(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);
        if(urom[0] == MARK_CODE)        // ??????
        {   
            EepRd(addr,(uint8_t *)urom,sizeof(STRWMTOT));           
            uct = CheckSum(urom,(sizeof(STRWMTOT)-1)); 
            if(uct == 0)                // ??OK   
            {    
                memcpy((uint8_t *)&gstuWmTot, urom, sizeof(STRWMTOT));                                               
                return 1;       // ??????
            }
        }
    }      
//----------------------------------------------------------------------------//    
    // ?????
    for(i = 0; i < 16; i++)
    {      
        addr = ADDR_PAGE2;              // ???
        addr += (uint32_t)i * sizeof(STRWMTOT);
        // ???????????
        EepRd(addr + sizeof(STRWMTOT) - 1,(uint8_t *)urom,1);
        if(urom[0] == MARK_CODE)        // ??????
        {   
            EepRd(addr,(uint8_t *)urom,sizeof(STRWMTOT));
            uct = CheckSum(urom,(sizeof(STRWMTOT)-1));  
            if(uct == 0)                // ??OK   
            {                    
                memcpy((uint8_t *)&gstuWmTot, urom, sizeof(STRWMTOT));          
               
                /*copy????page6*/
                addr = ADDR_PAGE6; 
                SaveFlow(addr,&gstuWmTot);
                return 2;       // ??????
            }
        }
    }  
//----------------------------------------------------------------------------//    
    return 0;    
}

/*
********************************************************************************
??????
********************************************************************************
*/
//void SetDftPara(void)
//{
////---------------------------------------------------//   
//    gstuWmPara.mu1Rf_Chn        = 20;       
//    gstuWmPara.mu1Rf_Frq        = 1;                        
//    gstuWmPara.mu1Rf_Bps        = 0;        
//    gstuWmPara.mu1Rf_Pow        = 7;
////---------------------------------------------------//       
//    gstuWmPara.mu4InitVal       = 0;                        // ????    
//    gstuWmPara.mu4Net_Id        = DEF_DEV_ID;               // rs485??
//    
//#if 0    
//    gstuWmPara.mu4DtuId         = DEF_DTU_ID;               // dut_id    
//#else
//    gstuWmPara.mu4DtuId         = DEF_WM_NUM;               // dut_id = DEF_WM_NUM    
//#endif    
////---------------------------------------------------//       
//    gstuWmPara.mu1Tr_Cnt        = DEF_TR_CNT;               // ??????   
//    gstuWmPara.mu1Tr_Ho         = 0;                        // ????
//    gstuWmPara.mu1Tr_Mi         = 0;
//    gstuWmPara.mu1Tr_Se         = 0;
////---------------------------------------------------//        
//    gstuWmPara.IP_ADDR[0]       = IP_DEF_0;    
//    gstuWmPara.IP_ADDR[1]       = IP_DEF_1;    
//    gstuWmPara.IP_ADDR[2]       = IP_DEF_2;    
//    gstuWmPara.IP_ADDR[3]       = IP_DEF_3;   
//    gstuWmPara.mu2Port          = PORT_DEF;    
////---------------------------------------------------//                             
//    gstuWmPara.mu1Check = CheckSum((uint8_t *)&gstuWmPara,(uint16_t)(sizeof(STRWMPARA) - 2));
//    gstuWmPara.mu1Mark  = MARK_CODE;
////---------------------------------------------------//        
//}

/*
********************************************************************************
//?????1
********************************************************************************
*/
uint8_t CheckWmPara1(void)
{
    uint32_t addr;  
//----------------------------------------------------------------------------//               
    addr = ADDR_PAGE3;                   // page3??    
    // ???????????
    EepRd(addr + sizeof(STRWMPARA) - 1, (uint8_t *)urom, 1);
    if(urom[0] == MARK_CODE)
    {
        EepRd(addr, (uint8_t *)urom, sizeof(STRWMPARA));
        CheckSum(urom, (sizeof(STRWMPARA)-1)); 
		memcpy((uint8_t *)&gstuWmPara, (uint8_t *)urom,  sizeof(STRWMPARA));            
		return 1;              
    }  
//----------------------------------------------------------------------------//
//    addr = ADDR_PAGE7;                   // page7??    
//    // ???????????
//    EepRd(addr + sizeof(STRWMPARA) - 1,(uint8_t *)urom,1);
//    if(urom[0] == MARK_CODE)
//    {
//        EepRd(addr,urom,sizeof(STRWMPARA));
//        uct = CheckSum(urom,(sizeof(STRWMPARA)-1)); 
//        if(uct == 0)
//        {
//            memcpy((uint8_t *)&gstuWmPara, (uint8_t *)urom,  sizeof(STRWMPARA));            
//            /*copy????page3*/
//            //-------------------------------//
//            addr = ADDR_PAGE3;  
//            EepWr(addr,(uint8_t *)urom,sizeof(STRWMPARA));   
//            //-------------------------------//
//            return 2;
//        }
//    }   
//----------------------------------------------------------------------------//  
    return 0;
}

/*
********************************************************************************
//?????2
********************************************************************************
*/
uint8_t CheckWmPara2(void)
{
    uint32_t addr; 
    uint8_t uct;  
//----------------------------------------------------------------------------//               
    addr = ADDR_PAGE7;                      
    // ???????????
    EepRd(addr + sizeof(STRWMPARA) - 1,(uint8_t *)urom,1);
    if(urom[0] == MARK_CODE)
    {
        EepRd(addr,(uint8_t *)urom,sizeof(STRWMPARA));    
        uct = CheckSum(urom,(sizeof(STRWMPARA)-1)); 
        if(uct == 0)
        {
            memcpy((uint8_t *)&gstuWmPara, (uint8_t *)urom,  sizeof(STRWMPARA));           
            return 1;              
        }
    }  
//----------------------------------------------------------------------------//
    addr = ADDR_PAGE3;             
    // ???????????
    EepRd(addr + sizeof(STRWMPARA) - 1,(uint8_t *)urom,1);
    if(urom[0] == MARK_CODE)        // ???
    {
        EepRd(addr,urom,sizeof(STRWMPARA));
        uct = CheckSum(urom,(sizeof(STRWMPARA)-1)); 
        if(uct == 0)          
        {
            memcpy((uint8_t *)&gstuWmPara, (uint8_t *)urom,  sizeof(STRWMPARA));
            //-------------------------------//
            /* copy????page7*/            
            addr = ADDR_PAGE7;  
            EepWr(addr,(uint8_t *)urom,sizeof(STRWMPARA));                                  
            //-------------------------------//
            return 2;
        }
    }   
//----------------------------------------------------------------------------//  
    return 0;
}

/*
********************************************************************************
?????
????2??????,??1?????
********************************************************************************
*/
void SaveWMPara(STRWMPARA *pstu)
{
    uint32_t    addr;
  
    if(isEEPRomOK == 0) {
        return;
    }

    pstu->mu1Check = CheckSum((uint8_t *)pstu,(sizeof(STRWMPARA)-2));    
    pstu->mu1Mark  = MARK_CODE;    
    //--------------------------//    
    addr = ADDR_PAGE3;          // ???
    EepWr(addr,(uint8_t *)pstu,sizeof(STRWMPARA));

//    addr = ADDR_PAGE7;          // ???
//    EepWr(addr,(uint8_t *)pstu,sizeof(STRWMPARA));          
    //--------------------------//       
}


uint8_t  ReadUserConfig(void)
{
	uint8_t i,len ;
	uint32_t    addr;
	uint8_t buf[256];
	
	len = sizeof(STRWMPARA);
	addr = ADDR_PAGE3;
	EepRd(addr,(uint8_t *)buf,1);    // 读取地址标示位
	
	if(buf[len-1] != MARK_CODE)
	{
#if DBGMODE            
        printf("Addr Err.\r\n");
#endif  
		return 0;
	}
	
	else
	{
		EepRd(addr,(uint8_t *)buf,len);
		 memcpy((uint8_t *)&gstuWmPara, (uint8_t *)buf,  sizeof(STRWMPARA));
		for(i = 0; i < 4; i ++)
		{
			gstuNbPara.IP_ADDR[i] = gstuWmPara.IP_ADDR[i];
		}
		gstuNbPara.mu2Port = gstuWmPara.mu2Port;
		gstuNbPara.mu2DtuID = gstuWmPara.mu2DtuID;
		gstuNbPara.mu2CjbID = gstuWmPara.mu2CjbID;
#if DBGMODE            
        printf("Addr is OK.\r\n");
		printf("AddrMax = %d.\r\n",addrcache.addrMax);
#endif  
		return 1;
	}
}
/*
********************************************************************************
??????,?1?????;
?????1?,?????;
********************************************************************************
*/
void SaveTotFlow(STRWMTOT *pstu)
{       
    uint32_t addr;
    
    if(isEEPRomOK == 0)
      return;
    //--------------------------//  
    memcpy(urom,(uint8_t *)pstu,sizeof(STRWMTOT));    
  
    addr = ADDR_PAGE2;
    SaveFlow(addr,pstu);          // ???
    
    addr = ADDR_PAGE6;
    SaveFlow(addr,pstu);          // ???
    //--------------------------//    
}

/*
********************************************************************************
?????
********************************************************************************
*/
void InitUserConfig(void)
{
	uint8_t i;

    //-----------------------------------------//     
 
    isEEPRomOK = 0;
    if(BSP_EE_CheckOk(EE_DEV_ADDR)>0) 
	{     
        isEEPRomOK = 1;
#if DBGMODE            
        printf("Eeprom is ok.\r\n");
#endif        
    }	
    //-----------------------------------------//     
    if(isEEPRomOK > 0)
    {  
        //--------------------------// 读取设置参数
        CheckWmPara1();          
		ReadFlowAddr();
        //--------------------------//               
        gstuNbPara.IP_ADDR[0] = gstuWmPara.IP_ADDR[0];
        gstuNbPara.IP_ADDR[1] = gstuWmPara.IP_ADDR[1];
        gstuNbPara.IP_ADDR[2] = gstuWmPara.IP_ADDR[2];
        gstuNbPara.IP_ADDR[3] = gstuWmPara.IP_ADDR[3];
        gstuNbPara.mu2Port  = gstuWmPara.mu2Port;
         //--------------------------//  
        gstuNbPara.mu2DtuID  = gstuWmPara.mu2DtuID;
		gstuNbPara.mu2CjbID  = gstuWmPara.mu2CjbID;
        //--------------------------//      
        for(i = 0; i < 70; i ++)
        {
			gstuNbPara.mu1FlowAddr[i] = MbusFlow.flowaddr[i]= addrcache.addrFlow[i];
		}	
		gstuNbPara.mu1FlowMax	= MbusFlow.nummax =addrcache.addrMax;
    }
    //-----------------------------------------//     
#if 0    
    comSendBuf(DBGCOM,(uint8_t *)&gstuWmTot, sizeof(gstuWmTot));      
#endif    
    
#if 0    
    comSendBuf(DBGCOM,(uint8_t *)&gstuWmPara, sizeof(gstuWmPara));      
#endif    
}
/*******************************(END OF FILE)**********************************/

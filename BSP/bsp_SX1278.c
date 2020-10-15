/*
*******************************************************************************
*
*                           the bsp_sx1278 file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : bsp_sx1278.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "bsp.h"



uint8_t SX_Send(uint8_t c)
{
    uint8_t uct = 0;
    uct = HalSpiTransmit(c);
    return uct;
}


uint8_t Read_Byte(uint8_t addr, uint8_t n)
{    
    uint8_t i,uct = 0;
    
    SpiSetNss(0);
    
    uct = addr;
    SX_Send(uct);       
    printf("%02x_",uct);         
    
    for(i = 0; i < n; i++)
    {
        uct = SX_Send(0);     
        printf("%02x_",uct); 
    }
    
    SpiSetNss(1);
    printf(",");
    return uct;
}

static uint8_t ucdat[8] = {0};

void Write_Byte(uint8_t addr,uint8_t *pch,uint8_t n)
{
    uint8_t i,uct = 0;

    SpiSetNss(0);
//-------------------------------------//
    uct = addr;
    SX_Send(uct);    
    printf("%02x_",uct);   

    for(i = 0; i < n; i++)
    {
        uct = *pch++;
        SX_Send( uct ); 
        printf("%02x_",uct);       
    }
//-------------------------------------//    
    SpiSetNss(1);
    printf(",");   

}

/*
��110���Ĵ���
*/
void ReadLoraReg(void)
{
    uint8_t i = 0;
    uint8_t uct = 0;
    
    ucdat[0] = 0;
    ucdat[1] = 0;
    Write_Byte(0xC0, (uint8_t *)ucdat, 2);
    
    
    SpiSetNss(0);    
    uct = 0x01;
    SX_Send( uct );     
    printf("%02x.",uct);   
        
    uct = SX_Send( 0x00 );     
    printf("%02x.",uct); 
    
    for(i = 0; i < 110; i++)
    {
        uct = SX_Send( 0x00 );     
        printf("%02x.",uct); 
    }       
    SpiSetNss(1);
    
    printf(",");
}


void RadioInitSpi(void)
{
    Read_Byte(0x01, 1);             // ���Ĵ���
    
    ucdat[0] = 0x80;                // LORA˯��   
    Write_Byte(0x81, (uint8_t *)ucdat, 1);
    
    ucdat[0] = 0x81;                // LORA����  
    Write_Byte(0x81, (uint8_t *)ucdat, 1);
    	   
    Read_Byte(0x01, 1);             // ���Ĵ���    
#if 0    
    ucdat[0] = 0xff;                // ���س���     
    Write_Byte(0xA3, (uint8_t *)ucdat, 1);
#endif        
    //-------------------//     
    Read_Byte(0x42, 1);             // ���汾��    
    //-------------------//     
    Read_Byte(0x0C, 1);             // ���Ĵ���    
    ucdat[0] = 0x20;             
    Write_Byte(0x8C, (uint8_t *)ucdat, 1);
    //-------------------//        
    ucdat[0] = 0x75;                // ����Ƶ��     
    ucdat[1] = 0xA0;             
    ucdat[2] = 0x00;             
    Write_Byte(0x86, (uint8_t *)ucdat, 3);
    //-------------------//       
    Read_Byte(0x31, 1);             // ���Ĵ���            
    ucdat[0] = 0xC3;             
    Write_Byte(0xB1, (uint8_t *)ucdat, 1);    
    //------------------------------------------//
    Read_Byte(0x1E, 1);             // ����Ƶ�Ĵ���  
    ucdat[0] = 0xC0;             
    Write_Byte(0x9E, (uint8_t *)ucdat, 1);
           
    Read_Byte(0x1D, 1);             // �����������    
    ucdat[0] = 0x74;                // д���������
    Write_Byte(0x9D, (uint8_t *)ucdat, 1);
        		   
    Read_Byte(0x1E, 1);             // ����Ƶ�Ĵ���    
    ucdat[0] = 0xC4;                // д��Ƶ����
    Write_Byte(0x9E, (uint8_t *)ucdat, 1);
            	  
    Read_Byte(0x1D, 1);             // �����������    
    ucdat[0] = 0x74;                // д����Ĵ���
    Write_Byte(0x9D, (uint8_t *)ucdat, 1);
    
    Read_Byte(0x1E, 2);             
    ucdat[0] = 0xC7;                
    ucdat[1] = 0xE8;                
    Write_Byte(0x9E, (uint8_t *)ucdat, 2);	         
    //------------------------------------------//    
    ucdat[0] = 0x80;                // ���ø����ֽڳ���
    Write_Byte(0xA2, (uint8_t *)ucdat, 1);	   
    //------------------------------------------//  
    Read_Byte(0x26, 1);             // ���Ĵ���
    ucdat[0] = 0x0C;             
    Write_Byte(0xA6, (uint8_t *)ucdat, 1);
    
    
    Read_Byte(0x09, 1);           
    ucdat[0] = 0xCF;             
    Write_Byte(0x89, (uint8_t *)ucdat, 1);
    
    
    Read_Byte(0x4D, 1);             // ���Ĵ���
    Read_Byte(0x09, 1); 
    ucdat[0] = 0x87;                // PA��������
    Write_Byte(0xCD, (uint8_t *)ucdat, 1);
    
    Read_Byte(0x09, 1);             // ���Ĵ���
    Read_Byte(0x4D, 1);             // ���Ĵ���
    ucdat[0] = 0xFF;                // PAѡ��͹������
    Write_Byte(0x89, (uint8_t *)ucdat, 1);
    
    
    ucdat[0] = 0x81;                // LORA����
    Write_Byte(0x81, (uint8_t *)ucdat, 1);              
    Read_Byte(0x01, 1);             // ���Ĵ���	 
    
    
    ucdat[0] = 0x81;                // LORA����
    Write_Byte(0x81, (uint8_t *)ucdat, 1);          
    Read_Byte(0x01, 1);             // ���Ĵ���	   
}


/*******************************************************************************
* Function Name  : RcvConfig
* Description    : ���չ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RcvConfig(void)
{     
    ucdat[0] = 0x0f;               
    Write_Byte(0x91, (uint8_t *)ucdat, 1);      

    ucdat[0] = 0xff;               
    Write_Byte(0xA4, (uint8_t *)ucdat, 1);      

    ucdat[0] = 0x01;                
    ucdat[1] = 0x00;                
    Write_Byte(0xC0, (uint8_t *)ucdat, 2);      
    
    Read_Byte(0x20, 2);
    
    ucdat[0] = 0xF0;                
    ucdat[1] = 0xFF;                
    Write_Byte(0xA0, (uint8_t *)ucdat, 2);    
    
    ucdat[0] = 0x00;               
    Write_Byte(0x8D, (uint8_t *)ucdat, 1);      
    
    ucdat[0] = 0x85;               
    Write_Byte(0x81, (uint8_t *)ucdat, 1);      

    Read_Byte(0x01, 1);    
    Read_Byte(0x23, 1);            // ���Ĵ���
    //-------------------// 
    ReadLoraReg();    
    //-------------------// 
}

void Lora_Txd()
{

}

/*
sx1278��ʼ����
*/
void bsp_InitSpi(void)
{
    HalPinInit();
    HalSpiWriteNss(1);    
}





/*******************************(END OF FILE)**********************************/

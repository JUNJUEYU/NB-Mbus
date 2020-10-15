#ifndef __APP_UART_H
#define __APP_UART_H

#include "stm32l1xx.h"


#define     INVALFLAG               (0x00)
#define     VALIDFLAG               (0x01)

#define     NBRD_HEAD               (0xA5)
#define     BL_HEAD               	(0x7B)

#define     NBRD_LEN                (0x04)
#define     NBRD_CMD                (0x80)      // ???

#define     NBCN_CMD                (0x81)      // ????
#define     NBCN_SUC                (0x01)      // ????
#define     NBCN_ERR                (0x7E)      // ????




// ?NB???
typedef struct 
{   
    uint8_t     m1Head;     // ͷ  
    uint8_t     m1Len;      // ����   
    
    uint8_t     m1Cmd;      // ������      
    uint8_t     m1Check;    // У���
}STUPARARD;



#pragma pack (1)					                    // ????	
typedef struct
{
    uint8_t     mu1Head;	//ͷ
    uint8_t     mu1Len;  	//����
//----------------------------------//    
    uint8_t		IP_ADDR[4];		    // IP         
    uint16_t    mu2Port;            // �˿ں�    
//----------------------------------//----------------------//8bytes       
    uint8_t     mu1Imsi[8];         // SIM����
//----------------------------------//----------------------//16bytes           
    uint32_t    mu4NbID;           // nbID
    uint16_t    mu2DtuID;         // DTUID
    uint16_t    mu2CjbID;          // �ɼ���ID
 //----------------------------------//----------------------//
	uint8_t	   mu1FlowMax;		//���ˮ����
    uint8_t     mu1FlowAddr[70];         // ˮ���ַ
//----------------------------------//----------------------//28bytes          
    uint8_t     mu1Type;            // ˮ������
    uint8_t     mu1TxMax;           // ��������    
    uint8_t     mu1Csq;             // CSQֵ
    uint8_t     mu1Check; 	        // У���---------------//32bytes     
}STUNBSET;
#pragma pack ()

extern __IO    STUNBSET    gstuNbPara; 

void ProcBlueData(void);

#endif


#ifndef __APP_RECORD_H
#define __APP_RECORD_H

#include "stm32l1xx.h"


#define     MARK_RCRD           0x35
#define 		ADDR_ENABLE		1
#define		ADDR_DISABLE		0XFF

// 记录头
typedef struct 
{                             

    uint8_t    mu1PageIn;              // 记录序号
      
    uint8_t     mu1Rsv[11];              // 记录标记
	uint8_t     mu1Mark;              // 记录标记

}STRRCDHEAD;                            // 16bytes

typedef struct
{
	uint8_t	addrEn;
	uint8_t	addrMax;
	uint8_t	addrFlow[70];
	uint8_t	addrSum;
}FLOWADDRSET;

extern  FLOWADDRSET		addrcache;				

void SaveRecord(STUNBFLOWDATA *pstu);
void InitFlowAddr(void);
uint8_t ReadFlowAddr(void);
void SaveFlowAddr(uint8_t *pnbrsv); 

uint32_t ReadRecord(STUNBFLOWDATA *pstu);
void ChgRcdHead(uint32_t u4id);

#endif


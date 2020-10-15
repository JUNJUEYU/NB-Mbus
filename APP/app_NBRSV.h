/*
*******************************************************************************
*
*                           the app_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : app_uart2.h
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#ifndef APP_UART2_H_
#define APP_UART2_H_


typedef struct
{
    uint16_t    index;      // ????   
    uint8_t     errcnt;     // ????
    uint8_t     cnncnt;     // ????
    
    uint8_t     steps;      // ????       
    uint8_t     cnnct;      // ??????(????)    
    uint8_t     cscon;      // ????????     
    
    uint8_t     cereg;      // ??????
    uint8_t     cgatt;      // ??????
    uint8_t     csq;        // ??????
    uint8_t     cimi;       // ???
    uint8_t     cclk;       // ????
    uint8_t     cmee;       // ???????
    
    uint8_t     nsocl;      // ??sock??    
    uint8_t     nsocr;      // ??sock??
    uint8_t     nsoco;      // ??????    
    uint8_t     nsocfg;     // ??????
                  
    uint8_t     ackok;      // ok??
    uint8_t     ackerr;     // error??
    
    uint8_t     nbtxd;      // ??????
    uint8_t     nbrxd;      // ??????     
				   
    uint8_t     nbreq;     	// ????
}STUNBSTA;  

extern __IO     STUNBSTA    gstuNbSta;    

void GetCardImsi(void);
void Nb_Data_Rec(void);
void ProcNbComRec(void);

//void TASK_UART2(void *pvParameters);


#endif /* APP_UART2_H_ */

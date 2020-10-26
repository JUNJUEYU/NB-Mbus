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
    uint16_t    index;      // 上报序号   
    uint8_t     errcnt;     // 失败次数
    uint8_t     cnncnt;     // 联网次数
    
    uint8_t     steps;      // 流程步骤   
    uint8_t     cnnct;      // 信号通断标志    
    uint8_t     cscon;      // 对应连接应答标志 
    
    uint8_t     cereg;      // 联网状态应答
    uint8_t     cgatt;      // 网络激活应答
    uint8_t     csq;        // 信号强度应答
    uint8_t     cimi;       // 电话卡
    uint8_t     cclk;       // 时钟数据
    uint8_t     cmee;       // 激活错误码应答
    
    uint8_t     nsocl;      // 关闭sock请求  
    uint8_t     nsocr;      // 创建sock请求
    uint8_t     nsoco;      // 建立远程连接       
    uint8_t     nsocfg;     // 数据格式配置
                  
    uint8_t     ackok;      // ok应答
    uint8_t     ackerr;     // error应答
    
    uint8_t     nbtxd;      // 数据发送完毕
    uint8_t     nbrxd;      // 数据接收完成  
				   
    uint8_t     nbreq;     	// 数据请求
}STUNBSTA;  

extern __IO     STUNBSTA    gstuNbSta;    

void GetCardImsi(void);
void Nb_Data_Rec(void);
void ProcNbComRec(void);

//void TASK_UART2(void *pvParameters);


#endif /* APP_UART2_H_ */

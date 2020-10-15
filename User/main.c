
#include	"includes.h"

#define	SYSTEM_SPEED 20								//系统运行速率

__IO	uint16_t    gu2BlueCnt = 0;                 //蓝牙触发计数器
__IO    uint16_t    gu2IdlCnt = 0;                 	//空闲时间计数器         
__IO    STRFLAG     gstuFlag;                       //标志结构体
uint32_t uid[3];									//处理器ID		
	
	
void EXTI0_IRQHandler(void)
{   
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
    {           
        EXTI_ClearFlag(EXTI_Line0);                 //清除标志中断位
        EXTI_ClearITPendingBit( EXTI_Line0 );  

        printf("setup.\r\n");                   
    }
}

void EXTI15_10_IRQHandler(void)
{               
    if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
    {                    
        EXTI_ClearFlag(EXTI_Line11);                //清除标志中断位             
        EXTI_ClearITPendingBit( EXTI_Line11);                
          
        if(GPIO_ReadInputDataBit( BL_STA_PORT, BL_STA_PIN ) != 0)           
        {
            gstuFlag.mBlueF = 1;                    //蓝牙触发
			gstuFlag.mbWorkF =	CONFIGING;			
        }
    }    
}

void EXTI9_5_IRQHandler(void)
{                
    if(EXTI_GetITStatus(EXTI_Line8) != RESET) 
    {                    
        EXTI_ClearFlag(EXTI_Line8);             	//清除标志中断位
        EXTI_ClearITPendingBit( EXTI_Line8 );
        
        if(GPIO_ReadInputDataBit( EX_KEY_PORT, EX_KEY_PIN ) == 0)         
        {   
			gstuFlag.mbAlmF = 1;                	//触发唤醒
			if(addrcache.addrEn)
			{
				gstuFlag.mbWorkF =	READING;		//有地址，去读表	
			}
			else
			{
				gstuFlag.mbWorkF =	IDLING;			//没地址，空闲等待配置
			}
        }        
    }
}


static void CheckBlueSta(void)	//检查蓝牙连接
{
    if(GPIO_ReadInputDataBit( BL_STA_PORT, BL_STA_PIN) != 0)
    {
        gu2BlueCnt += 1;
        if(gu2BlueCnt >= 5) 
        {                    
            gu2BlueCnt = 5;
            gstuFlag.mBlueF = 1;              
			gstuFlag.mbWorkF = CONFIGING;
        }
    }
    else
    {
        if(gu2BlueCnt > 0) {        
            gu2BlueCnt -= 1;            
        } 
		else 
		{        
            gstuFlag.mBlueF = 0;            
			if(gstuFlag.mbWorkF == CONFIGING)
			{
				gstuFlag.mbWorkF = IDLING;
			}
        }
    }  
} 

void SleepTime(void)
{
	uint8_t time = 0;
	uint32_t seed = 0;
	uint32_t *id = (uint32_t *)0x1FF80050;
	
	seed = id[2];	//取ID低32位

	srand(seed);
	time= rand()%59; 
	gstuWmPara.mu1Tr_Mi = time;
	
	seed = seed - time;
	srand(seed);
	time= rand()%59; 
	gstuWmPara.mu1Tr_Se = time;
	
	seed = seed - time;
	srand(seed);
	time= rand()%23; 
	gstuWmPara.mu1Tr_Ho = time;
}


int main( void )
{             
    RCC_MSIRangeConfig(RCC_MSIRange_3);  //524k 系统时钟  
    SystemCoreClockUpdate();           
    Delay_ms(100);                                            
    bsp_Init();                         
    Delay_ms(200);                     
    
	InitUserConfig();
    gstuMbusFlowData.flow_addr[1][0] = 0X10;
	LED_ON();
	          
    gstuFlag.mbAlmF = 1;                //闹钟唤醒标志

	SleepTime();						//设置唤醒时间
	
	if(GPIO_ReadInputDataBit( BL_STA_PORT, BL_STA_PIN ) != 0)           
	{
		gstuFlag.mBlueF = 1;           	//蓝牙触发      
		gstuFlag.mbWorkF =	CONFIGING;			
	}
	
	else if(addrcache.addrEn){	
		gstuFlag.mbWorkF =	READING;
	} 
   	
	else{
		gstuFlag.mbWorkF =  IDLING;
	}

//-----------------------------//        
    while(1)
    {       
		if(gstuFlag.mbAlmF == 0)              //休眠时间    		
        {              
			gstuFlag.mbNbEn = 0;
			NB_Close();                       //清除所有NB标志位   
			LED_OFF();
			SleepTime();					  //设置唤醒时间
            To_Enter_Stop(); 				  //进入休眠
			InitUserConfig();
			if(addrcache.addrEn)
			{
				gstuFlag.mbWorkF =	READING;
				MBUS_ON();
				MbusFlow.flownumber = 0;
				Delay_ms(500); 
			}
        }         
        else
		{				
			CheckBlueSta();
				
			if((gstuFlag.mbWorkF != CONFIGING) & (gstuFlag.mbWorkF != READING) & (gstuFlag.mbWorkF != COMPLETE))
			{
				gstuFlag.mbWorkF = IDLING;
			}
			
			if(gstuFlag.mbWorkF == CONFIGING)
			{
				ProcBlueData();
				Delay_ms(400);
			}
			
			else if(gstuFlag.mbWorkF == READING)
			{		
				if(Timer >= 1000)		//2000*15ms=30S抄表间隔
				{
					Timer = 0;								
#if DBGMODE            
					printf("Now is reading NO.%d.\r\n", (MbusFlow.flownumber + 1));
#endif   
					flow_read( MbusFlow.flownumber);	//抄表成功
					MbusFlow.flownumber += 1;
				}
				
				if(MbusFlow.flownumber > (MbusFlow.nummax - 1))	//超过最大表号，抄表工作结束
				{			
					MbusFlow.flownumber = 0;
					gstuFlag.mbWorkF = COMPLETE;
					gstuFlag.mbNbEn = 1;
					SleepTimer = 0;
					mbus_shutdown();
					NBPOWER_ON();
					Delay_ms(100);
				}
			}
				 
			else if(gstuFlag.mbWorkF == COMPLETE) //完成抄表工作，上传数据
			{ 
				ProcNbComRec();                    // 处理UART2接收  
				NB_Connect();                      // 建立连接                                                      
				NB_DataUpload();                   // 数据上传				
				NB_Close();                        // 清除所有标志位    
				
				if(gstuFlag.mbNbEn == 0)
				{
					gstuFlag.mbWorkF = SLEEP;
					gstuFlag.mbAlmF = 0;
				}
			}	
			
			else if(gstuFlag.mbWorkF == IDLING)	//系统空闲
			{
				gu2IdlCnt += 1;
				if(gu2IdlCnt > 100)				   //空闲超时，进入睡眠
				{
					gu2IdlCnt = 0;
					gstuFlag.mbWorkF = SLEEP;
					gstuFlag.mbAlmF = 0;
				}			
			}
			//----------------------------------// 
			Delay_ms(SYSTEM_SPEED);	//系统运行延时
			//----------------------------------// 
		}
	}
}

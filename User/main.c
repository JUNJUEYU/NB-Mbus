
#include	"includes.h"

#define	SYSTEM_SPEED	20								//ϵͳ��������
#define	IDLE_CYCLE		100								//ϵͳ���еȴ�����

__IO	uint16_t    gu2BlueCnt = 0;                 //��������������
__IO    uint16_t    gu2IdlCnt = 0;                 	//����ʱ�������
__IO 	uint8_t		retrans_cnt = 0;				//�ش�������         
__IO    STRFLAG     gstuFlag;                       //��־�ṹ��
__IO	uint8_t     record_point = 0;
	STRRCDHEAD          gstuRcdHead1;

uint32_t uid[3];									//������ID		
	
	
void EXTI0_IRQHandler(void)
{   
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
    {           
        EXTI_ClearFlag(EXTI_Line0);					//�����־�ж�λ
        EXTI_ClearITPendingBit(EXTI_Line0);  

        printf("setup.\r\n");                   
    }
}

void EXTI15_10_IRQHandler(void)
{               
    if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
    {                    
        EXTI_ClearFlag(EXTI_Line11);				//�����־�ж�λ             
        EXTI_ClearITPendingBit(EXTI_Line11);                
          
        if(GPIO_ReadInputDataBit( BL_STA_PORT, BL_STA_PIN ) != 0)           
        {
            gstuFlag.mBlueF = 1;					//��������
			gstuFlag.mbWorkF = CONFIG;			
        }
    }    
}

void EXTI9_5_IRQHandler(void)
{                
    if(EXTI_GetITStatus(EXTI_Line8) != RESET) 
    {                    
        EXTI_ClearFlag(EXTI_Line8);					//�����־�ж�λ
        EXTI_ClearITPendingBit(EXTI_Line8);
        
        if(GPIO_ReadInputDataBit( EX_KEY_PORT, EX_KEY_PIN ) == 0)         
        {   
			gstuFlag.mbAlmF = 1;                	//��������
			if(addrcache.addrEn)
			{
				gstuFlag.mbWorkF = READ;			//�е�ַ��ȥ����	
			}
			else
			{
				gstuFlag.mbWorkF = IDLE;			//û��ַ�����еȴ�����
			}
        }        
    }
}


static void CheckBlueSta(void)	//�����������
{
    if(GPIO_ReadInputDataBit( BL_STA_PORT, BL_STA_PIN) != 0)
    {
        gu2BlueCnt += 1;
        if(gu2BlueCnt >= 5) 
        {                    
            gu2BlueCnt = 5;
            gstuFlag.mBlueF = 1;              
			gstuFlag.mbWorkF = CONFIG;
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
			if(gstuFlag.mbWorkF == CONFIG)
			{
				gstuFlag.mbWorkF = IDLE;
			}
        }
    }  
} 

void SleepTime(uint8_t cnt)
{
	uint8_t time = 0;
	uint32_t seed = 0;
	uint32_t *id = (uint32_t *)0x1FF80050;
	TTime	mRtc;

	if(cnt == 0)	//���ش�
	{
		seed = id[2];	//ȡID��32λ

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
	
	else if(cnt == 1)		//��һ���ش�
	{
		ReadRtcTime(&mRtc);
		gstuWmPara.mu1Tr_Ho = mRtc.hours + 2;  //2Сʱ����
		gstuWmPara.mu1Tr_Mi = mRtc.minutes;
		gstuWmPara.mu1Tr_Se = mRtc.seconds;
		if(gstuWmPara.mu1Tr_Ho > 23)
		{
			gstuWmPara.mu1Tr_Ho -= 24;
		}
		if(gstuWmPara.mu1Tr_Mi > 59)
		{
			gstuWmPara.mu1Tr_Mi -= 60;
		}
	}

	else if(cnt == 2)		//�ڶ����ش�
	{
		ReadRtcTime(&mRtc);
		gstuWmPara.mu1Tr_Ho = mRtc.hours + 6; //6Сʱ����
		gstuWmPara.mu1Tr_Mi = mRtc.minutes;
		gstuWmPara.mu1Tr_Se = mRtc.seconds;
		if(gstuWmPara.mu1Tr_Ho > 23)
		{
			gstuWmPara.mu1Tr_Ho -= 24;
		}	
		if(gstuWmPara.mu1Tr_Mi > 59)
		{
			gstuWmPara.mu1Tr_Mi -= 60;
		}	
	}
}


int main( void )
{             
    RCC_MSIRangeConfig(RCC_MSIRange_3);  //524k ϵͳʱ��  
    SystemCoreClockUpdate();           
    Delay_ms(100);                                            
    bsp_Init();                         
    Delay_ms(200);                     
    
	InitUserConfig();
    gstuMbusFlowData.flow_addr[1][0] = 0X10;
	LED_ON();
	         
#if 0

	gstuRcdHead1.mu1PageIn = 0;
	gstuRcdHead1.mu1Mark = 0;
	gstuRcdHead1.mu1Rsv[0] = 0;
	gstuRcdHead1.mu1Rsv[1] = 0;
	SaveRcdHead(&gstuRcdHead1);
#endif
	
    gstuFlag.mbAlmF = 1;                //���ӻ��ѱ�־

	SleepTime(0);						//���û���ʱ��
	
	if(GPIO_ReadInputDataBit( BL_STA_PORT, BL_STA_PIN ) != 0)           
	{
		gstuFlag.mBlueF = 1;           	//��������      
		gstuFlag.mbWorkF =	CONFIG;			
	}
	
	else if(addrcache.addrEn){	
		gstuFlag.mbWorkF =	READ;
	} 
   	
	else{
		gstuFlag.mbWorkF =  IDLE;
	}

//-----------------------------//        
    while(1)
    {       
		if(gstuFlag.mbAlmF == 0)				//����ʱ��    		
        {              
			gstuFlag.mbNbEn = 0;
			NB_Close();							//�������NB��־λ    
			LED_OFF();
			if(retrans_cnt > RETRANS_MAX)		//�����ش�������
			{
				retrans_cnt = 0;
			}
			SleepTime(retrans_cnt);				//�����ش��������û���ʱ��

            To_Enter_Stop();					//��������
			InitUserConfig();
			if((addrcache.addrEn) & (retrans_cnt == 0))
			{
				gstuFlag.mbWorkF = READ;
				MBUS_ON();
				MbusFlow.flownumber = 0;
				Delay_ms(500); 
			}
			else if((addrcache.addrEn))
			{
				gstuFlag.mbWorkF = TRANS;
				gstuFlag.mbNbEn = 1;
				NBPOWER_ON();
				Delay_ms(100);
			}
        }         
        else
		{				
			CheckBlueSta();
				
			if((gstuFlag.mbWorkF != CONFIG) & (gstuFlag.mbWorkF != READ) & (gstuFlag.mbWorkF != TRANS))
			{
				gstuFlag.mbWorkF = IDLE;
			}
			
			if(gstuFlag.mbWorkF == CONFIG)
			{
				ProcBlueData();
				Delay_ms(400);
			}
			
			else if(gstuFlag.mbWorkF == READ)
			{		
				if(Timer >= 1000)		//2000*15ms=30S������
				{
					Timer = 0;								
#if DBGMODE            
					printf("Now is READ NO.%d.\r\n", (MbusFlow.flownumber + 1));
#endif   
					flow_read( MbusFlow.flownumber);	//����ɹ�
					MbusFlow.flownumber += 1;
				}
				
				if(MbusFlow.flownumber > (MbusFlow.nummax - 1))	//��������ţ�����������
				{			
					FillFlowData(&gstuNbFlowData);
					FillFlowTail(&gstuNbFlowData);
					MbusFlow.flownumber = 0;
					gstuFlag.mbWorkF = TRANS;
					gstuFlag.mbNbEn = 1;
					mbus_shutdown();
					NBPOWER_ON();
					Delay_ms(100);
				}
			}
				 
			else if(gstuFlag.mbWorkF == TRANS) //��ɳ��������ϴ�����
			{ 
				ProcNbComRec();                    // ����UART2����  
				NB_Connect();                      // ��������                                                      
				NB_DataUpload();                   // �����ϴ�				
				NB_Close();                        // ������б�־λ    
				
				if(gstuFlag.mbNbEn == 0)
				{
					gstuFlag.mbWorkF = SLEEP;
					gstuFlag.mbAlmF = 0;
				}
			}	
			
			else if(gstuFlag.mbWorkF == IDLE)	//ϵͳ����
			{
				gu2IdlCnt += 1;
				if(gu2IdlCnt > IDLE_CYCLE)				   //���г�ʱ������˯��
				{
					gu2IdlCnt = 0;
					gstuFlag.mbWorkF = SLEEP;
					gstuFlag.mbAlmF = 0;
				}			
			}
			//----------------------------------// 
			Delay_ms(SYSTEM_SPEED);	//ϵͳ������ʱ
			//----------------------------------// 
		}
	}
}

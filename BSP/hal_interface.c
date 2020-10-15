/*
*******************************************************************************
*
*                           the bsp_spi file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : bsp_spi.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "bsp.h"


static interruptCb dio0Irq = NULL;
static interruptCb dio1Irq = NULL;
static interruptCb dio2Irq = NULL;
static interruptCb dio3Irq = NULL;


//使用SPI1//PB3--SCK//PB4--MISO//PB5--MOSI
static void HalSpiInit( void )
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
            
    /* Enable SPI1 and GPIO clocks */    
    RCC_AHBPeriphClockCmd(SPI_GPIO_RCC, ENABLE);                                /* GPIO clocks */        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);                        /* SPI1 clocks */

    GPIO_PinAFConfig(SPI_GPIO_PORT, GPIO_PinSource3, GPIO_AF_SPI1);
    GPIO_PinAFConfig(SPI_GPIO_PORT, GPIO_PinSource4, GPIO_AF_SPI1);
    GPIO_PinAFConfig(SPI_GPIO_PORT, GPIO_PinSource5, GPIO_AF_SPI1);

    /*!< Configure SPI_FLASH_SPI pins: SCK */    
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;
    
    GPIO_InitStructure.GPIO_Pin         = SPI_CLK_PIN;
    GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure SPI_FLASH_SPI pins: MISO */
    GPIO_InitStructure.GPIO_Pin         = SPI_MISO_PIN;
    GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure SPI_FLASH_SPI pins: MOSI */
    GPIO_InitStructure.GPIO_Pin         = SPI_MOSI_PIN;
    GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);
    
    /*-------------------------------------------------------------*/
    //SPI_INTERFACE Config
    SPI_Cmd(SPI1, DISABLE);                                                     //必须先禁能,才能改变MODE
    SPI_InitStructure.SPI_Direction     = SPI_Direction_2Lines_FullDuplex;      //两线全双工
    SPI_InitStructure.SPI_Mode                  = SPI_Mode_Master;              //主
    SPI_InitStructure.SPI_DataSize              = SPI_DataSize_8b;              //8位
    SPI_InitStructure.SPI_CPOL                  = SPI_CPOL_Low;                 //CPOL=1时钟悬空低
    SPI_InitStructure.SPI_CPHA                  = SPI_CPHA_1Edge;               //CPHA=1 数据捕获第2个
    SPI_InitStructure.SPI_NSS                   = SPI_NSS_Soft;                 //软件NSS
    SPI_InitStructure.SPI_BaudRatePrescaler     = SPI_BaudRatePrescaler_2;      //分频
    SPI_InitStructure.SPI_FirstBit              = SPI_FirstBit_MSB;             //高位在前
    SPI_InitStructure.SPI_CRCPolynomial         = 7;                             //CRC7
    SPI_Init(SPI1,&SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
    /*-------------------------------------------------------------*/     
    SPI_Init(SPI1,&SPI_InitStructure);
}


//SPI使能//PB6--NSS
void HalPinInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);    
//-------------------------------------------------------------------//    
    // Configure SPI_NSS
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;        
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;
 
    GPIO_InitStructure.GPIO_Pin         = NSS_PIN;   
    GPIO_WriteBit(NSS_IOPORT,NSS_PIN,Bit_SET);    
    GPIO_Init(NSS_IOPORT, &GPIO_InitStructure);
//-------------------------------------------------------------------//   
#if 0    
    //接收发送控制脚
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_400KHz;
      
    GPIO_InitStructure.GPIO_Pin         = RXTX_PIN;
    GPIO_Init(RXTX_IOPORT, &GPIO_InitStructure);
#endif    
//-------------------------------------------------------------------//          
    HalSpiInit();
}

//控制spi nss信号
void HalSpiWriteNss(uint8_t level)
{
    if(!level){
        GPIO_WriteBit(NSS_IOPORT,NSS_PIN,Bit_RESET);
    }else{
        GPIO_WriteBit(NSS_IOPORT,NSS_PIN,Bit_SET);
    }
}

uint8_t HalSpiTransmit( uint8_t outData )
{
    uint8_t rxData = 0;
    uint8_t txData = outData;

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);    
    SPI_I2S_SendData(SPI1,txData);
    
    /* 等待数据接收完毕 */
    //while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);        
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    
    rxData = SPI_I2S_ReceiveData(SPI1);
 
    return rxData;
}

//控制复位信号
void HalWriteReset(uint8_t level)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if(!level){ //输出低电平
        GPIO_WriteBit(RESET_IOPORT,RESET_PIN,Bit_RESET);
        
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;        
        GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;        
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_400KHz;
        GPIO_InitStructure.GPIO_Pin     = RESET_PIN;
        GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );
    }else{ //设为输入
        GPIO_WriteBit(RESET_IOPORT,RESET_PIN,Bit_SET);
        
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_400KHz;
        GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;      
        GPIO_InitStructure.GPIO_Pin     =  RESET_PIN;
        GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );
    }
}

//控制发射接收引脚 高收低发
void HalWriteRxTx(uint8_t level)
{
#if 0
    if(!level){  //输出低
        GPIO_WriteBit( RXTX_IOPORT, RXTX_PIN, Bit_RESET );
    }else{      //输出高
        GPIO_WriteBit( RXTX_IOPORT, RXTX_PIN, Bit_SET );
    }
#endif    
}

void HalAttachInterrupt(uint16_t pin, interruptCb irq)
{
    switch(pin)
    {
    case DIO0_PIN:
        if(irq != NULL)
        {
            dio0Irq = irq;
        }
        break;
    case DIO1_PIN:
        if(irq != NULL)
        {
            dio1Irq = irq;
        }

        break;
    case DIO2_PIN:
        if(irq != NULL)
        {
            dio2Irq = irq;
        }

        break;
    case DIO3_PIN:
        if(irq != NULL)
        {
            dio3Irq = irq;
        }
        break;
    case DIO4_PIN:
        break;
    case DIO5_PIN:
        break;
    default:
        break;
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
    case DIO0_PIN:
        if(dio0Irq != NULL)
        {
            dio0Irq();
        }
        break;
    case DIO1_PIN:
        if(dio1Irq != NULL)
        {
            dio1Irq();
        }
        break;
    case DIO2_PIN:
        if(dio2Irq != NULL)
        {
            dio2Irq();
        }
        break;
    case DIO3_PIN:
        if(dio3Irq != NULL)
        {
            dio3Irq();
        }
        break;
    case DIO4_PIN:
    case DIO5_PIN:
    default:
        break;
    }
}


/*******************************(END OF FILE)**********************************/
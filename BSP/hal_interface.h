#ifndef __HAL_INTERFACE_H
#define __HAL_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32l1xx.h"  
#include <stdint.h>
#include <string.h>
  
  
                                                                                                                                                                
#define SPI_GPIO_PORT                               GPIOB                   /* GPIOB */
#define SPI_GPIO_RCC                                RCC_AHBPeriph_GPIOB
                                                                                                                                                                
#define SPI_CLK_PIN                                 GPIO_Pin_3              /* PB.3 */
#define SPI_MISO_PIN                                GPIO_Pin_4              /* PB.4 */
#define SPI_MOSI_PIN                                GPIO_Pin_5              /* PB.5 */
#define SPI_CS_PIN                                  GPIO_Pin_6              /* PB.6 */
  

/*!
 * SX1278 SPI NSS I/O definitions
 */
#define NSS_IOPORT                                  GPIOB
#define NSS_PIN                                     GPIO_Pin_6
   
  


//接收发送使能控制 高收低发
#define RXTX_IOPORT                                 GPIOA
#define RXTX_PIN                                    GPIO_Pin_7   

/*!
 * SX1278 DIO pins  I/O definitions
 */
    
#define DIO0_IOPORT                                 GPIOB
#define DIO0_PIN                                    GPIO_Pin_15

#define DIO1_IOPORT                                 GPIOB
#define DIO1_PIN                                    GPIO_Pin_14

#define DIO2_IOPORT                                 GPIOB
#define DIO2_PIN                                    GPIO_Pin_13

#define DIO3_IOPORT                                 GPIOB
#define DIO3_PIN                                    GPIO_Pin_12

#define DIO4_IOPORT                                 GPIOB
#define DIO4_PIN                                    GPIO_Pin_7

#define DIO5_IOPORT                                 GPIOB
#define DIO5_PIN                                    GPIO_Pin_8
  



typedef void (*interruptCb)(void);

void HalPinInit(void);
void HalSpiWriteNss(uint8_t level);
uint8_t HalSpiTransmit( uint8_t outData );
void HalWriteReset(uint8_t level);
void HalWriteRxTx(uint8_t level);
void HalAttachInterrupt(uint16_t pin, interruptCb irq);

//void delay(uint32_t ms);
//uint32_t millis(void);

#ifdef __cplusplus
}
#endif

#endif



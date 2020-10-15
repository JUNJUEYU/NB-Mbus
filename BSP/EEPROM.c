/*
*******************************************************************************
*
*                           the app_EEPROM file
*                  ST Microelectronics STM32 with the STM32L15x
*
* Filename      : main.c
* Version       : V1.0.0
* Programmer(s) : self
*******************************************************************************
*/
#include "EEPROM.h"




uint8_t EEPROM_WriteBytes(uint32_t addr, uint8_t *data, uint16_t len)
{
    uint16_t i;
    FLASH_Status status;
    
    //-------------------------------//
    if(len > 256)
      return 1;
    
    if((len == 0) || (addr < EEPROM_START_ADDR) || ((addr + (len - 1)) > EEPROM_END_ADDR))
    return 1;
    //-------------------------------//     
     /* Unlock the FLASH PECR register and Data EEPROM memory */
    DATA_EEPROM_Unlock();
    /* Clear all pending flags */      
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
              | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);
    
/*
    // wubo:不用加擦除指令
    for(i=0; i<len; i++)
    {
        DATA_EEPROM_EraseByte(addr + i);     
    }
*/
 
    i = 0;
    while(i < len)
    {                 
        if(*(__IO uint8_t *)(addr + i) == data[i])
        {
            i++;
        }
        else
        {  
            __disable_irq();    //系统关全局中断                       
            status = DATA_EEPROM_ProgramByte(addr + i, data[i]);      
            __enable_irq();     //系统开全局中断

            if(status == FLASH_COMPLETE)
            {
            i++;
            }
            else
            {
            FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
            }
        }
        
    }

    DATA_EEPROM_Lock();

    for(i=0; i<len; i++)
    {
      if(*(__IO uint8_t *)(addr + i) != data[i])
        return 1;
    }

    return 0; 
}

#if 0
uint8_t EEPROM_WriteBytes(uint32_t addr, uint8_t *data, uint16_t len)
{
    uint16_t i;
    FLASH_Status status;

    if((len == 0) || (addr < EEPROM_START_ADDR) || ((addr + (len - 1)) > EEPROM_END_ADDR))
    return 1;

    /* Unlock the FLASH PECR register and Data EEPROM memory */
    DATA_EEPROM_Unlock();
    /* Clear all pending flags */      
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
    | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);

    for(i=0; i<len; i++)
    {
      DATA_EEPROM_EraseByte(addr + i);
    }

    i = 0;
    while(i < len)
    {  
      status = DATA_EEPROM_ProgramByte(addr + i, data[i]);

      if(status == FLASH_COMPLETE)
      {
        i++;
      }
      else
      {
        FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
      | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
      }
    }

    DATA_EEPROM_Lock();

    for(i=0; i<len; i++)
    {
      if(*(__IO uint8_t *)(addr + i) != data[i])
        return 1;
    }

    return 0;
}
#endif

uint8_t EEPROM_ReadBytes(uint32_t addr, uint8_t *data, uint16_t len)
{
    uint16_t i = 0;
    
    if((len == 0) || (addr < EEPROM_START_ADDR) || ((addr + (len - 1)) > EEPROM_END_ADDR))
            return 1;
    
    for(i=0; i<len; i++)
    {
            data[i] = *(__IO uint8_t *)(addr + i);
    }
    
    return 0;
}



/*******************************(END OF FILE)**********************************/



#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32l1xx.h"

#define EEPROM_START_ADDR     			0x08080000
#define EEPROM_END_ADDR       			0x08080fff      // 4096 bytes


#define BOOTLOADER_PARAMETER_ADDR		EEPROM_START_ADDR
#define BOOTLOADER_PARAMETER_SIZE		0x40


#define TEST_PARAMETER_ADDR		        0x08080040
#define TEST_PARAMETER_SIZE			0x40


#define PARAMETER_ADDR				0x08080080
#define PARAMEYER_SIZE				0x80
#define PARAMETER_MAXNUM			23


#define ACQUIRE_DATA_ADDR			0x08080C00
#define ACQUIRE_DATA_SIZE			0x20
#define ACQUIRE_DATA_MAXNUM			288


struct bStruct
{
	uint32_t appSize;
	
	uint8_t md5Val[16];
	
	char appUpdateIp[16];
	char appUpdatePort[6];
	
	uint8_t appExistFlag;
	uint8_t appUpdateFlag;
	
	char newAppName[8];
};



uint8_t EEPROM_WritePage(uint32_t addr, uint8_t *data, uint16_t len);

uint8_t EEPROM_WriteBytes(uint32_t addr, uint8_t *data, uint16_t len);
uint8_t EEPROM_ReadBytes(uint32_t addr, uint8_t *data, uint16_t len);





#endif

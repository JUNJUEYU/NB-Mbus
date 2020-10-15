#ifndef __APP_WATER_H
#define __APP_WATER_H

#include "stm32l1xx.h"

extern STRWATER         gstuWater;


uint32_t HexToBcd(uint32_t v);
uint32_t BcdToHex(uint32_t v);


void DataPack(void);
void WaterPulse(void);

uint8_t LoraSum(uint8_t *pch,uint16_t n);

#endif


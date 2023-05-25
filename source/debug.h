/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_DEBUG_H_
#define _DMCU_DEBUG_H_

#include <stdint.h>

void DEBUG_Write(uint16_t reg, uint32_t value);

uint32_t DEBUG_Read(uint16_t reg);

void DEBUG_Write0x6(uint32_t value);

void DEBUG_SetLowMarker(uint16_t val);

void DEBUG_SetHighMarker(uint16_t bits);

void DEBUG_Write0x7(uint32_t value);

void DEBUG_ClearMarker(uint16_t bits);

#endif

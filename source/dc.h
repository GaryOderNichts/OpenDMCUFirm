/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_DC_H_
#define _DMCU_DC_H_

#include <stdint.h>
#include "dc_registers.h"

void DC_ImmediateWrite32(uint16_t reg, uint32_t val);

void DC_Write32(uint16_t reg, uint32_t val);

void DC_DirectWrite32(uint16_t addr, uint32_t val);

void DC_Write8(uint16_t reg, uint16_t off, uint8_t val);

void DC_DirectWrite8(uint16_t addr, uint8_t val);

void DC_WaitWrite(void);

void DC_PerformRead(uint16_t reg);

uint8_t DC_Get8(uint16_t off);

uint32_t DC_Get32(void);

uint32_t DC_Read32(uint16_t reg);

uint8_t DC_Read8(uint16_t reg, uint16_t off);

void DC_Set8(uint16_t reg, uint16_t off, uint8_t val);

void DC_Clear8(uint16_t reg, uint16_t off, uint8_t val);

#endif

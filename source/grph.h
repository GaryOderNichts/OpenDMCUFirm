/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_GRPH_H_
#define _DMCU_GRPH_H_

#include <stdint.h>

void GRPH_SetEnable(uint8_t enable);

void GRPH_WaitUpdate(void);

void GRPH_SetD1Viewport(uint16_t xSize, uint16_t ySize, uint16_t xEnd, uint16_t yEnd, uint16_t xStart, uint16_t yStart);

void GRPH_SetD2Viewport(uint16_t xSize, uint16_t ySize, uint16_t xEnd, uint16_t yEnd, uint16_t xStart, uint16_t yStart);

#endif

/*
 *   This file is part of OpenDMCUFirm
 */
#include "grph.h"
#include "dc.h"

void GRPH_SetEnable(uint8_t enable)
{
    DC_Write8(D1GRPH_ENABLE, 0, enable);
    // No D2GRPH_ENABLE?
}

void GRPH_WaitUpdate(void)
{
    while (DC_Read32(D1GRPH_UPDATE) & GRPH_MODE_UPDATE_PENDING)
        ;
}

void GRPH_SetD1Viewport(uint16_t xSize, uint16_t ySize, uint16_t xEnd, uint16_t yEnd, uint16_t xStart, uint16_t yStart)
{
    GRPH_WaitUpdate();

    DC_Write32(D1MODE_VIEWPORT_SIZE, MODE_VIEWPORT_WIDTH(xSize) | MODE_VIEWPORT_HEIGHT(ySize));
    DC_Write32(D1MODE_VIEWPORT_START, MODE_VIEWPORT_X_START(xStart) | MODE_VIEWPORT_Y_START(yStart));
    DC_Write32(D1GRPH_X_END, xEnd & GRPH_X_END_MASK);
    DC_Write32(D1GRPH_Y_END, yEnd & GRPH_Y_END_MASK);
}

void GRPH_SetD2Viewport(uint16_t xSize, uint16_t ySize, uint16_t xEnd, uint16_t yEnd, uint16_t xStart, uint16_t yStart)
{
    GRPH_WaitUpdate();

    DC_Write32(D2MODE_VIEWPORT_SIZE, MODE_VIEWPORT_WIDTH(xSize) | MODE_VIEWPORT_HEIGHT(ySize));
    DC_Write32(D2MODE_VIEWPORT_START, MODE_VIEWPORT_X_START(xStart) | MODE_VIEWPORT_Y_START(yStart));
    DC_Write32(D2GRPH_X_END, xEnd & GRPH_X_END_MASK);
    DC_Write32(D2GRPH_Y_END, yEnd & GRPH_Y_END_MASK);
}

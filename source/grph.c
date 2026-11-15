/*
 *   This file is part of OpenDMCUFirm
 */
#include "grph.h"
#include "dc.h"

#ifdef CUSTOM
#include "custom.h"
#endif

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
#ifdef CUSTOM
    // Check if 576i
    Viewport* vp = (ySize > 480) ? &_custom.tvViewport_576i : &_custom.tvViewport;

    if (vp->x.size != 0xffff) {
        xSize = vp->x.size;
    }
    if (vp->x.end != 0xffff) {
        xEnd = vp->x.end;
    }
    if (vp->x.start != 0xffff) {
        xStart = vp->x.start;
    }
    if (vp->y.size != 0xffff) {
        ySize = vp->y.size;
    }
    if (vp->y.end != 0xffff) {
        yEnd = vp->y.end;
    }
    if (vp->y.start != 0xffff) {
        yStart = vp->y.start;
    }
#endif

    GRPH_WaitUpdate();

    DC_Write32(D1MODE_VIEWPORT_SIZE, MODE_VIEWPORT_WIDTH(xSize) | MODE_VIEWPORT_HEIGHT(ySize));
    DC_Write32(D1MODE_VIEWPORT_START, MODE_VIEWPORT_X_START(xStart) | MODE_VIEWPORT_Y_START(yStart));
    DC_Write32(D1GRPH_X_END, xEnd & GRPH_X_END_MASK);
    DC_Write32(D1GRPH_Y_END, yEnd & GRPH_Y_END_MASK);
}

void GRPH_SetD2Viewport(uint16_t xSize, uint16_t ySize, uint16_t xEnd, uint16_t yEnd, uint16_t xStart, uint16_t yStart)
{
#ifdef CUSTOM
    // Check if 576i
    Viewport* vp = (ySize > 480) ? &_custom.drcViewport_576i : &_custom.drcViewport;

    if (vp->x.size != 0xffff) {
        xSize = vp->x.size;
    }
    if (vp->x.end != 0xffff) {
        xEnd = vp->x.end;
    }
    if (vp->x.start != 0xffff) {
        xStart = vp->x.start;
    }
    if (vp->y.size != 0xffff) {
        ySize = vp->y.size;
    }
    if (vp->y.end != 0xffff) {
        yEnd = vp->y.end;
    }
    if (vp->y.start != 0xffff) {
        yStart = vp->y.start;
    }
#endif

    GRPH_WaitUpdate();

    DC_Write32(D2MODE_VIEWPORT_SIZE, MODE_VIEWPORT_WIDTH(xSize) | MODE_VIEWPORT_HEIGHT(ySize));
    DC_Write32(D2MODE_VIEWPORT_START, MODE_VIEWPORT_X_START(xStart) | MODE_VIEWPORT_Y_START(yStart));
    DC_Write32(D2GRPH_X_END, xEnd & GRPH_X_END_MASK);
    DC_Write32(D2GRPH_Y_END, yEnd & GRPH_Y_END_MASK);
}

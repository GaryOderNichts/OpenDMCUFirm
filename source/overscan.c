/*
 *   This file is part of OpenDMCUFirm
 */
#include "overscan.h"
#include "dc.h"
#include "debug.h"
#include "conf.h"

uint32_t DMCU_MboxRead(void)
{
    return DC_Read32(0x1042);
}

uint8_t setupOverscan(DisplayType display, uint32_t left, uint32_t right)
{
    uint16_t addr = 0;
    if (display == DISPLAY_TV){
        addr = D1MODE_EXT_OVERSCAN_LEFT_RIGHT;
    } else if (display == DISPLAY_DRC) {
        addr = D2MODE_EXT_OVERSCAN_LEFT_RIGHT;
    }

    if (addr != 0) {
        DC_Write32(addr, MODE_EXT_OVERSCAN_LEFT(left) | MODE_EXT_OVERSCAN_RIGHT(right));
        return TRUE;
    }

    return FALSE;
}

void disableOverscan(DisplayType display)
{
    setupOverscan(display, 0, 0);
}

BOOL getOverscanConfiguration(uint16_t yStart, uint32_t* outLeft, uint32_t* outRight)
{
    uint32_t value;
    switch (yStart) {
    case 0x1:
    case 0x2:
        value = 67;
        break;
    case 0x3:
        value = 160;
        break;
    case 0x4:
        value = 240;
        break;
    case 0x5:
    case 0x6:
        value = 107;
        break;
    default:
        return 0;
    }

    *outLeft = value;
    *outRight = value;
    return TRUE;
}

static void enableOverscan(DisplayType display, uint16_t yStart)
{
    uint32_t left;
    uint32_t right;
    if (getOverscanConfiguration(yStart, &left, &right)) {
        setupOverscan(display, left, right);
    }
}

BOOL handleOverscanClear(DMCUState* state)
{
    uint32_t reg = DMCU_MboxRead();

    if (reg & 0x20000000) {
        disableOverscan(DISPLAY_TV);
    }

    if (reg & 0x10000000) {
        disableOverscan(DISPLAY_DRC);
    }

    return TRUE;
}

BOOL handleOverscanConfig(DMCUState* state)
{
    uint32_t reg = DMCU_MboxRead();

    if (reg & 0x20000000) {
        enableOverscan(DISPLAY_TV, state->yStart);
    }

    if (reg & 0x10000000) {
        enableOverscan(DISPLAY_DRC, 5);
    }

    return TRUE;
}

BOOL handleOverscanNop(DMCUState* state)
{
    return FALSE;
}

OVERSCANHandlerFn OVERSCAN_GetHandler(void)
{
    uint32_t reg = DMCU_MboxRead();
    DEBUG_Write(0x3, reg);

    if (reg & 0x1) {
        return &handleOverscanNop;
    } else if (reg & 0x2) {
        return &handleOverscanConfig;
    } else if (reg & 0x4) {
        return &handleOverscanClear;
    }

    return NULL;
}

/*
 *   This file is part of OpenDMCUFirm
 */
#include "debug.h"
#include "dc.h"

// Debug output over scratch registers
// None of this is necessary for the DMCU to work

static uint32_t debug0x6Value;
static uint32_t errorMask;

void DEBUG_Write(uint16_t reg, uint32_t value)
{
    if (reg > 8) {
        DEBUG_SetLowMarker(0x4);
        return;
    }

    DC_Write32(DOUT_SCRATCH0 + reg, value);
}

uint32_t DEBUG_Read(uint16_t reg)
{
    if (reg > 8) {
        DEBUG_SetLowMarker(0x4);
        return ~0;
    }

    return DC_Read32(DOUT_SCRATCH0 + reg);
}

static void DEBUG_311a(uint32_t* out, uint32_t val, uint32_t mask)
{
    val = val & mask;
    if (val == 0) {
        return;
    }

    *out = (*out & ~mask) | val;
}

void DEBUG_Write0x6(uint32_t value)
{
    DEBUG_311a(&debug0x6Value, value, 0xf8000000);
    DEBUG_311a(&debug0x6Value, value, 0x30);
    DEBUG_311a(&debug0x6Value, value, 0x03);
    DEBUG_311a(&debug0x6Value, value, 0x0C);
    DEBUG_311a(&debug0x6Value, value, 0xC0);
    DEBUG_Write(0x6, debug0x6Value);
}

void DEBUG_SetLowMarker(uint16_t val)
{
    DEBUG_Write0x7(val);
}

void DEBUG_SetHighMarker(uint16_t bits)
{
    errorMask |= ((uint32_t) bits << 16);
    DEBUG_Write0x7(errorMask);
}

void DEBUG_Write0x7(uint32_t value)
{
    errorMask |= value;
    DEBUG_Write(0x7, errorMask);
}

void DEBUG_ClearMarker(uint16_t bits)
{
    errorMask = errorMask & ~bits;
    DEBUG_Write(0x7, errorMask);
}

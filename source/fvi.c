/*
 *   This file is part of OpenDMCUFirm
 */
#include "fvi.h"
#include "dc.h"
#include "irq.h"
#include "debug.h"
#include "dmcu.h"
#include "conf.h"
#include "timer.h"

// FVI = Faux Video Interface (This is what f0f called it)
// FVI is a hardware shim which emulates the old video interface, which state is read by the DMCU

FVIRegs gFVIRegs;
uint16_t gUpdateFVIRegs = 1;
FVIState gFVIState;

BOOL FVI_Wait(void)
{
    if ((DC_Read32(0x1080) & 1) != 0) {
        while ((DC_Read32(0x108B) >> 16) != 0x7)
            ;
    }

    return TRUE;
}

BOOL FVI_ReadRegs(FVIRegs* regs)
{
    IRQ_Lock();
    regs->reg0x1061 = DC_Read32(0x1061);
    regs->reg0x109B = DC_Read32(0x109B);
    regs->reg0x1080 = DC_Read32(0x1080);
    regs->reg0x1084 = DC_Read32(0x1084);
    regs->reg0x1083 = DC_Read32(0x1083);
    regs->reg0x1087 = DC_Read32(0x1087);
    regs->reg0x1089 = DC_Read32(0x1089);
    regs->reg0x1082 = DC_Read32(0x1082);
    regs->reg0x1092 = DC_Read32(0x1092);
    gUpdateFVIRegs = 0;
    IRQ_Unlock();
    return TRUE;
}

void FVI_SetUpdateRegs(uint16_t update)
{
    gUpdateFVIRegs = update;
}

FVIRegs* FVI_GetRegs(void)
{
    if (gUpdateFVIRegs) {
        FVI_Wait();
        FVI_ReadRegs(&gFVIRegs);
    }

    return &gFVIRegs;
}

FVIState* FVI_GetState(void)
{
    FVI_Wait();
    FVI_ReadRegs(&gFVIRegs);

    gFVIState.unk0x00 = (gFVIRegs.reg0x109B >> 24) & 0xF;
    gFVIState.unk0x04 = 0;
    gFVIState.unk0x10 = (gFVIRegs.reg0x1083 >> 16) & 0x3FF;
    gFVIState.unk0x0A = gFVIRegs.reg0x1083 & 0x3FF;
    gFVIState.unk0x08 = 0;
    gFVIState.unk0x14 = 0;
    gFVIState.unk0x0C = 0;
    gFVIState.unk0x1A = gFVIRegs.reg0x1092 & 0xFF;
    gFVIState.unk0x18 = 0;
    // I think this is right?
    gFVIState.videoMode = ((uint16_t)gFVIState.unk0x00 * 0x3) + ((gFVIRegs.reg0x1080 >> 16) & 0x3ff) * 0x2 + ((uint16_t)gFVIState.unk0x10) + gFVIState.unk0x0A;
    gFVIState.unk0x06 = (gFVIRegs.reg0x1080 >> 16) & 0x3FF;
    gFVIState.unk0x0E = gFVIRegs.reg0x1084 & 0x3FF;
    gFVIState.unk0x16 = (gFVIRegs.reg0x1084 >> 16) & 0x3FF;
    return &gFVIState;
}

static FVIVideoMode convertVideoMode(uint32_t rawMode)
{
    FVIVideoMode mode = FVI_VIDEO_MODE_INVALID;

    switch (rawMode) {
    case 0x41A:
        mode = FVI_VIDEO_MODE_480P;
        break;
    case 0x4E2:
        mode = FVI_VIDEO_MODE_576I;
        break;
    case 0x20D:
        mode = FVI_VIDEO_MODE_UNK1;
        break;
    case 0x271:
        mode = FVI_VIDEO_MODE_UNK2;
        break;
    case 0x270:
        mode = FVI_VIDEO_MODE_UNK3;
        break;
    case 0x272:
        mode = FVI_VIDEO_MODE_UNK4;
        break;
    case 0x20C:
        mode = FVI_VIDEO_MODE_UNK5;
        break;
    case 0x20E:
        mode = FVI_VIDEO_MODE_UNK6;
        break;
    default:
        break;
    }

    if (mode == FVI_VIDEO_MODE_INVALID) {
        DEBUG_SetLowMarker(0x2);
        DEBUG_SetHighMarker((rawMode >> 16) | 0x1000);
    } else {
        DEBUG_ClearMarker(0x2);
    }

    return mode;
}

BOOL FVI_GetVideoMode(FVIVideoMode* outVideoMode)
{
    FVIState* state = FVI_GetState();
    *outVideoMode = 0;

    if (state) {
        FVIVideoMode mode = convertVideoMode(state->videoMode);
        if (mode != FVI_VIDEO_MODE_INVALID) {
            *outVideoMode = mode;
            return TRUE;
        }
    }

    return FALSE;
}

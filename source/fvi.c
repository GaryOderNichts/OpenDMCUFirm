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

    gFVIState.equalization = (gFVIRegs.reg0x109B >> 24) & 0xF;
    gFVIState.activeLines = (gFVIRegs.reg0x1080 >> 16) & 0x3FF;
    gFVIState.postBlanking = (gFVIRegs.reg0x1083 >> 16) & 0x3FF;
    gFVIState.preBlanking = gFVIRegs.reg0x1083 & 0x3FF;
    gFVIState.unk0x18 = gFVIRegs.reg0x1092 & 0xFF;
    gFVIState.unk0x14 = (gFVIRegs.reg0x1084 >> 16) & 0x3FF;
    gFVIState.unk0x0C = gFVIRegs.reg0x1084 & 0x3FF;
    gFVIState.halfLines = ((uint16_t)gFVIState.equalization * 0x3) + // Multiply by 3 for pre-equalization, vert. sync. and post-equalization
                          ((uint16_t)gFVIState.activeLines * 0x2) +
                          ((uint16_t)gFVIState.postBlanking) +
                          ((uint16_t)gFVIState.preBlanking);
    return &gFVIState;
}

static FVIVideoMode getVideoMode(uint32_t halfLines)
{
    FVIVideoMode mode = FVI_VIDEO_MODE_INVALID;

    switch (halfLines) {
    case 1050:
        mode = FVI_VIDEO_MODE_480P;
        break;
    case 1250:
        mode = FVI_VIDEO_MODE_576P;
        break;
    case 525:
        mode = FVI_VIDEO_MODE_480I;
        break;
    case 625:
        mode = FVI_VIDEO_MODE_576I;
        break;
    case 624:
        mode = FVI_VIDEO_MODE_288P_312;
        break;
    case 626:
        mode = FVI_VIDEO_MODE_288P_313;
        break;
    case 524:
        mode = FVI_VIDEO_MODE_240P_262;
        break;
    case 526:
        mode = FVI_VIDEO_MODE_240P_263;
        break;
    default:
        break;
    }

    if (mode == FVI_VIDEO_MODE_INVALID) {
        DEBUG_SetLowMarker(0x2);
        DEBUG_SetHighMarker(((uint16_t) halfLines) | 0x1000);
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
        FVIVideoMode mode = getVideoMode(state->halfLines);
        if (mode != FVI_VIDEO_MODE_INVALID) {
            *outVideoMode = mode;
            return TRUE;
        }
    }

    return FALSE;
}

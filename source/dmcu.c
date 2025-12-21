/*
 *   This file is part of OpenDMCUFirm
 */
#include "dmcu.h"
#include "dc.h"
#include "crtc.h"
#include "irq.h"
#include "dvo.h"
#include "grph.h"
#include "timer.h"
#include "conf.h"
#include "debug.h"
#include "fvi.h"
#include "deint.h"
#include "overscan.h"

#include <asm-m68hc11/ports_def.h>

typedef void (*DMCUEventHandlerFn)(DMCUState* state);

static uint32_t lastIrqTime = 0;

DMCUState gDmcuState = {
    .stateFlags = 0,
    .reg0x1060 = 0xFCC7FFF,
    .isVisible = FALSE,
    .currentVideoMode = FVI_VIDEO_MODE_INVALID,
    .requestedVideoMode = FVI_VIDEO_MODE_INVALID,
    .cbcrSwapEnabled = FALSE,
    .unk0x0E = 0,
    .currentVideoStandard = DMCU_VIDEO_STANDARD_INVALID,
    .deintNeedsUpdate = -1,
    .unk0x14 = 0xFFFF,
    .pendingOverscanConfiguration = FALSE,
    .yStart = 0
};
FVIVideoMode gVideoMode;

uint8_t gTVMode;
uint8_t gDRCMode;
uint16_t gTVXStart = 0x4;
uint16_t gTVYStart = 0x6;

uint32_t gMaskCount[0x4];

struct {
    uint32_t in;
    uint16_t out;
} irqMasks[4] = {
    { 0x00000003, 0x0001, },
    { 0x08007B1C, 0x0002, },
    { 0x07C084E0, 0x0004, },
    { 0x003C0000, 0x0008, },
};

// not sure what this register does
void write0x10a0_3(uint8_t param_1)
{
    IRQ_Lock();
    DC_Write8(0x10a0, 3, param_1);
    IRQ_Unlock();
}

uint8_t check0x10a0_3(void)
{
    DC_PerformRead(0x10A0);
    return DC_Get8(3) & 0x1;
}

void DMCU_HandleIrq(void)
{
    uint32_t currentIrqTime = GetTimerOverflowCount();
    if (currentIrqTime == lastIrqTime) {
        // TODO this increments some (unused) counter
    }
    lastIrqTime = currentIrqTime;

    DC_PerformRead(DMCU_INTERRUPT_STATUS);
    DC_Get32(); // discards read result?

    DC_PerformRead(0x1061);
    uint32_t irqFlags = DC_Get32();
    uint32_t handledFlags = 0;
    uint16_t stateFlags = gDmcuState.stateFlags;

    for (int i = 0; i < 4; i++) {
        uint32_t flags = irqMasks[i].in & irqFlags;
        if (!flags) {
            continue;
        }

        handledFlags |= flags;

        if (((irqFlags >> 4) & irqMasks[i].out & 0xf) != 0) {
            stateFlags |= (irqMasks[i].out & 0xff) << 8;
        } else {
            stateFlags |= irqMasks[i].out;
        }
    }

    gDmcuState.stateFlags = stateFlags;

    DC_PerformRead(0x1080);
    uint32_t unk0x1080 = DC_Get32();
    if ((unk0x1080 & 1) != 0 && (irqFlags & 1) != 0) {
        uint32_t tries = 0;
        while (tries < 0x10000) {
            if (CRTC_GetVertCount() == 0) {
                break;
            }

            tries++;
        }
        if (tries >= 0x10000) {
            DEBUG_SetLowMarker(1);
            DEBUG_SetHighMarker(1);
        }

        gDmcuState.isVisible = 1;
        write0x10a0_3(1);
    } else if ((unk0x1080 & 1) == 0 && (irqFlags & 1) != 0) {
        gDmcuState.isVisible = 0;
        DC_Write32(0x10a0, 0);
    }

    FVI_SetUpdateRegs(1);

    // Write back handled flags
    DC_Write32(0x1061, handledFlags);

    // Check DMCU mbox for pending overscan configurations
    if (DC_Read32(0x1043) & 0x1) {
        DC_Write32(0x1043, 1);
        gDmcuState.pendingOverscanConfiguration = 1;
    }
}

const CRTCConfig crtcConfigs[10] = {
    [0] = { CRTC_H_TOTAL(0xA4F), CRTC_H_BLANK_END(0x00C0) | CRTC_H_BLANK_START(0x0840), CRTC_H_SYNC_A_END(0x2C), CRTC_V_TOTAL(0x464), CRTC_V_BLANK_END(0x29) | CRTC_V_BLANK_START(0x0461), CRTC_V_SYNC_A_END(0x5), },
    [1] = { CRTC_H_TOTAL(0x897), CRTC_H_BLANK_END(0x00C0) | CRTC_H_BLANK_START(0x0840), CRTC_H_SYNC_A_END(0x2C), CRTC_V_TOTAL(0x464), CRTC_V_BLANK_END(0x29) | CRTC_V_BLANK_START(0x0461), CRTC_V_SYNC_A_END(0x5), },
    [2] = { CRTC_H_TOTAL(0xA4F), CRTC_H_BLANK_END(0x00C0) | CRTC_H_BLANK_START(0x0840), CRTC_H_SYNC_A_END(0x2C), CRTC_V_TOTAL(0x464), CRTC_V_BLANK_END(0x28) | CRTC_V_BLANK_START(0x0460), CRTC_V_SYNC_A_END(0xA), },
    [3] = { CRTC_H_TOTAL(0x897), CRTC_H_BLANK_END(0x00C0) | CRTC_H_BLANK_START(0x0840), CRTC_H_SYNC_A_END(0x2C), CRTC_V_TOTAL(0x464), CRTC_V_BLANK_END(0x28) | CRTC_V_BLANK_START(0x0460), CRTC_V_SYNC_A_END(0xA), },
    [4] = { CRTC_H_TOTAL(0x7BB), CRTC_H_BLANK_END(0x0104) | CRTC_H_BLANK_START(0x0604), CRTC_H_SYNC_A_END(0x28), CRTC_V_TOTAL(0x2ED), CRTC_V_BLANK_END(0x19) | CRTC_V_BLANK_START(0x02E9), CRTC_V_SYNC_A_END(0x5), },
    [5] = { CRTC_H_TOTAL(0x671), CRTC_H_BLANK_END(0x0104) | CRTC_H_BLANK_START(0x0604), CRTC_H_SYNC_A_END(0x28), CRTC_V_TOTAL(0x2ED), CRTC_V_BLANK_END(0x19) | CRTC_V_BLANK_START(0x02E9), CRTC_V_SYNC_A_END(0x5), },
    [6] = { CRTC_H_TOTAL(0x359), CRTC_H_BLANK_END(0x007A) | CRTC_H_BLANK_START(0x034A), CRTC_H_SYNC_A_END(0x3E), CRTC_V_TOTAL(0x20C), CRTC_V_BLANK_END(0x24) | CRTC_V_BLANK_START(0x0204), CRTC_V_SYNC_A_END(0x6), },
    [7] = { CRTC_H_TOTAL(0x35F), CRTC_H_BLANK_END(0x0084) | CRTC_H_BLANK_START(0x0354), CRTC_H_SYNC_A_END(0x40), CRTC_V_TOTAL(0x270), CRTC_V_BLANK_END(0x2C) | CRTC_V_BLANK_START(0x026C), CRTC_V_SYNC_A_END(0x5), },
    [8] = { CRTC_H_TOTAL(0x8C9), CRTC_H_BLANK_END(0x02BA) | CRTC_H_BLANK_START(0x0610), CRTC_H_SYNC_A_END(0x28), CRTC_V_TOTAL(0x225), CRTC_V_BLANK_END(0x2A) | CRTC_V_BLANK_START(0x020A), CRTC_V_SYNC_A_END(0x2), },
    [9] = { CRTC_H_TOTAL(0x3E7), CRTC_H_BLANK_END(0x0049) | CRTC_H_BLANK_START(0x039F), CRTC_H_SYNC_A_END(0x28), CRTC_V_TOTAL(0x21B), CRTC_V_BLANK_END(0x26) | CRTC_V_BLANK_START(0x0206), CRTC_V_SYNC_A_END(0x2), },
};

void FUN_1eca(void)
{
    // Inline CRTC disable
    uint32_t crtcControl = DC_Read32(D1CRTC_CONTROL);
    DC_Write32(D1CRTC_CONTROL, crtcControl & ~CRTC_MASTER_EN);

    // Inline wait disable
    while (DC_Read32(D1CRTC_CONTROL) & CRTC_CURRENT_MASTER_EN_STATE)
        ;

    if (check0x10a0_3()) {
        while ((DC_Read32(0x108B) & 0xFFFF0000) < 0x00F00000)
            ;
    }

    // Re-enable CRTC
    DC_Write32(D1CRTC_CONTROL, crtcControl | CRTC_MASTER_EN);

    // Wait enable
    while ((DC_Read32(D1CRTC_CONTROL) & CRTC_CURRENT_MASTER_EN_STATE) == 0)
        ;
}

void FUN_22eb(void)
{
    DC_Write32(0x1044, DC_Read32(0x1044) | 1);
}

void DMCU_Setup()
{
    DC_ImmediateWrite32(DOUT_SCRATCH0, 0);
    DC_ImmediateWrite32(DOUT_SCRATCH0, 0);
    DC_ImmediateWrite32(DOUT_SCRATCH0, 0);

    DC_Write8(DEINT_RSVD2, 2, 3);
    DC_Write8(DEINT_RSVD2, 3, 1);

    DC_Write8(DMCU_INTERRUPT_TO_UC_EN_MASK, 0, ABM1_HG_READY_INT_TO_UC_EN);
    DC_Write8(DMCU_INTERRUPT_TO_UC_EN_MASK, 3, 8); // VBLANK4_INT_TO_UC_EN
    DC_Write32(DMCU_INTERRUPT_TO_HOST_EN_MASK, 0);
    DC_Write32(DMCU_INTERRUPT_TO_UC_XIRQ_IRQ_SEL, 0);

    // setup timer
    _io_ports[M6811_TMSK2] = M6811_TOI | M6811_TPR_16;

    DC_DirectWrite32(0x8180, 0x3fff);

    gDmcuState.reg0x1060 = 0xFC07FFF;
    DC_Write32(0x1060, gDmcuState.reg0x1060);

    gDmcuState.isVisible = 0;

    DC_Write32(D1CRTC_DOUBLE_BUFFER_CONTROL, 0);
    DC_Write32(D2CRTC_DOUBLE_BUFFER_CONTROL, 0);

    CRTC_Blank();
    CRTC_WaitUpdate();

    IRQ_Lock();

    gDmcuState.currentVideoMode = gVideoMode;

    gTVMode = (CONF_GetOpMode(DISPLAY_TV) & OP_MODE_MASK) >> OP_MODE_SHIFT;
    gDRCMode = (CONF_GetOpMode(DISPLAY_DRC) & OP_MODE_MASK) >> OP_MODE_SHIFT;

    DVO_SetEnable(0);
    CRTC_DisableAll();

    DC_Write32(D2SCL_HORZ_FILTER_CONTROL, SCL_H_2TAP_ALPHA_COEF_EN);
    DC_Write32(D2SCL_VERT_FILTER_CONTROL, SCL_V_2TAP_ALPHA_COEF_EN);
    DC_Write32(D2SCL_TAP_CONTROL, SCL_HORZ_NUM_OF_TAPS(1) | SCL_VERT_NUM_OF_TAPS(1));
    DC_Write8(D2SCL_SCALER_ENABLE, 0, SCL_SCALE_EN);

    DC_Write32(D1SCL_AUTOMATIC_MODE_CONTROL, SCL_HORZ_CALC_AUTO_RATIO_EN | SCL_VERT_CALC_AUTO_RATIO_EN);
    DC_Write32(D2SCL_AUTOMATIC_MODE_CONTROL, SCL_HORZ_CALC_AUTO_RATIO_EN | SCL_VERT_CALC_AUTO_RATIO_EN);

    DVO_StopVIClock(DISPLAY_DRC);

    uint16_t yEnd, ySize;
    if (CONF_IsPAL(DISPLAY_TV)) {
        yEnd = 576;
        ySize = 550;
    } else {
        yEnd = 480;
        ySize = 458;
    }
    GRPH_SetD2Viewport(688, ySize, 720, yEnd, 16, 14);
    DC_Write32(D2GRPH_PITCH, GRPH_PITCH(736));

    DVO_RunVIClock(DISPLAY_DRC);

    udelay(0x286);

    if (CONF_IsHD(DISPLAY_TV)) {
        DEBUG_Write0x6(0x20);
        gDmcuState.unk0x0E = 0x1;

        if (gTVMode == OP_MODE_OTHER) {
            CRTC_SetD1Config(&crtcConfigs[5]);
            gTVYStart = 3;
            gDmcuState.yStart = 3;
            gTVXStart = 4;
        } else if (gTVMode == OP_MODE_1080I) {
            CRTC_SetD1Config(&crtcConfigs[3]);
            gTVYStart = 4;
            gDmcuState.yStart = 4;
            gTVXStart = 4;
        } else if (gTVMode == OP_MODE_1080P) {
            CRTC_SetD1Config(&crtcConfigs[1]);
            gTVYStart = 6;
            gDmcuState.yStart = 4;
            gTVXStart = 4;
        }

        CRTC_SetD2Config(&crtcConfigs[8]);

        DVO_StopVIClock(DISPLAY_TV);

        GRPH_SetD1Viewport(711, 474, 720, 480, gTVXStart, gTVYStart);
        DVO_SetVideoFormat(FVI_VIDEO_MODE_480I);
        DVO_SetRefreshRate(1, FVI_VIDEO_MODE_480I);

        DVO_RunVIClock(DISPLAY_TV);

        udelay(0x286);

        DEINT_Configure(FVI_VIDEO_MODE_480I);
    } else {
        DEBUG_Write0x6(0x10);
        gDmcuState.unk0x0E = 0x2;

        DC_Write32(D1MODE_MASTER_UPDATE_LOCK, MODE_MASTER_UPDATE_LOCK);

        if (CONF_IsPAL(DISPLAY_TV)) {
            DEINT_17dc();
            CRTC_SetD2Config(&crtcConfigs[9]);
            gDmcuState.currentVideoMode = FVI_VIDEO_MODE_576I;
            gDmcuState.currentVideoStandard = DMCU_VIDEO_STANDARD_PAL;
            gDmcuState.yStart = 0x2;
            GRPH_SetD1Viewport(712, 569, 720, 576, 4, 2);
            DVO_SetVideoFormat(FVI_VIDEO_MODE_576I);
            DVO_SetRefreshRate(2, FVI_VIDEO_MODE_576I);
            DEINT_Configure(FVI_VIDEO_MODE_576I);
        } else {
            gDmcuState.currentVideoMode = FVI_VIDEO_MODE_480P;
            gDmcuState.currentVideoStandard = DMCU_VIDEO_STANDARD_NTSC;
            gDmcuState.yStart = 0x1;
            GRPH_SetD1Viewport(712, 474, 720, 480, 4, 2);
            DVO_SetVideoFormat(FVI_VIDEO_MODE_480I);
            DVO_SetRefreshRate(2, FVI_VIDEO_MODE_480I);
            DEINT_Configure(FVI_VIDEO_MODE_480I);
        }

        DC_Write32(D1MODE_MASTER_UPDATE_LOCK, 0);
        udelay(0x286);
    }

// test start
    // CRTC_WaitUpdate();
    // CRTC_SetBlackColor(25, 0x3ff, 0);

    // CRTC_Blank();
    // CRTC_WaitUpdate();
// test end

    CRTC_EnableAll();
    CRTC_WaitUpdate();

    FUN_1eca();

    DVO_SetEnable(1);

    GRPH_SetEnable(1);

    IRQ_Unlock();

    FUN_22eb();
}

// TODO
static void FUN_15f0(DMCUState* state)
{
    DEINT_Lock();
    write0x10a0_3(0);
    state->isVisible = 0;
    CRTC_Blank();
    CRTC_WaitUpdate();
}

static void FUN_1555(DMCUState* state)
{
    FVIState* fviState = FVI_GetState();
    if (!fviState) {
        state->stateFlags |= 0x1000;
        return;
    }

    uint32_t count = 0;
    while (CRTC_GetVertCount() != fviState->equalization) {
        count++;

        if (count > 0x1FFF) {
            break;
        }
    }

    if (count > 0x1FFF) {
        DEBUG_SetLowMarker(0x1);
        DEBUG_SetHighMarker(0x4);
    }

    state->isVisible = 1;
    write0x10a0_3(1);
}

void DMCU_EventHandler0(DMCUState* state)
{
    FVIRegs* regs = FVI_GetRegs();
    if ((regs->reg0x1080 & 1) != 0 && state->isVisible == 0) {
        FUN_1555(state);
        DC_PerformRead(DEINT_RSVD2);
        DC_Write8(DEINT_RSVD2, 0x2, DC_Get8(0x2) | 0x04);
        DVO_SetEnable(0);
        udelay(0x411B);
        DVO_SetEnable(1);
    } else if ((regs->reg0x1080 & 1) == 0 && state->isVisible == 1) {
        DC_PerformRead(DEINT_RSVD2);
        DC_Write8(DEINT_RSVD2, 0x2, DC_Get8(0x2) & 0xFB);
        FUN_15f0(state);
    }
}

void DMCU_EventHandler1(DMCUState* state)
{
    DEBUG_Write0x6(0x40000000);
    if (state->deintNeedsUpdate == 0x1) {
        DEINT_Reconfigure();
    }

    FVIState* fviState = FVI_GetState();
    if (FVI_GetVideoMode(&gVideoMode) == TRUE) {
        if (gVideoMode == state->requestedVideoMode) {
            return;
        }

        if (state->isVisible != TRUE) {
            return;
        }

        CRTC_Blank();
        CRTC_WaitVertPosition();
        CRTC_WaitUpdate();

        uint16_t ret;
        if ((FVI_GetRegs()->reg0x109B & 0x10000) == 0x0) {
            ret = DEINT_0dff(gVideoMode, fviState);
        } else {
            ret = DEINT_0de4(gVideoMode);
        }

        if (ret == 0x1) {
            if (fviState->preBlanking < 101) {
                state->requestedVideoMode = gVideoMode;
            } else {
                state->requestedVideoMode = 0xffff;
            }

            return;
        }
    }

    state->stateFlags |= 0x2000;
}

void DMCU_EventHandler2(DMCUState* state)
{
    IRQ_Lock();
    DEINT_ConfigureCBCRSwap(&state->cbcrSwapEnabled);
    IRQ_Unlock();
}

static uint16_t FUN_21be(FVIRegs* regs)
{
    return 0;
}

void DMCU_EventHandler3(DMCUState* state)
{
    if (FUN_21be(FVI_GetRegs()) == 0x0) {
        if (state->deintNeedsUpdate) {
            // TODO these are usually read below, can be ignored for now
            // DAT_3a00 = 0x0;
            // DAT_39fe = 0x0;

            if (!CRTC_IsBlanked()) {
                CRTC_Blank();
                CRTC_WaitVertPosition();
                CRTC_WaitUpdate();
            }

            DEINT_Reconfigure();
        }
    } else {
        // TODO this is never reached, but could be filled out for completeness eventually
    }
}

void getEventHandler(DMCUEventHandlerFn* outFn, uint16_t* outMask, uint8_t flags)
{
    if (flags & 0x1) {
        *outFn = &DMCU_EventHandler0;
        *outMask = 0x1;
    } else if (flags & 0x2) {
        *outFn = &DMCU_EventHandler1;
        *outMask = 0x2;
    } else if (flags & 0x4) {
        *outFn = &DMCU_EventHandler2;
        *outMask = 0x4;
    } else if (flags & 0x8) {
        *outFn = &DMCU_EventHandler3;
        *outMask = 0x8;
    } else {
        *outFn = NULL;
        *outMask = 0;
    }
}

BOOL IsVideoModeSet(DMCUState* state)
{
    return state->currentVideoMode == state->requestedVideoMode;
}

BOOL NeedsUnblank(DMCUState* state)
{
    if (state->isVisible == 1) {
        if (IsVideoModeSet(state)) {
            return TRUE;
        }
    }

    return FALSE;
}

static DMCUVideoStandard getVideoStandard(FVIVideoMode videoMode)
{
    switch (videoMode) {
    case FVI_VIDEO_MODE_480I:
    case FVI_VIDEO_MODE_240P_262:
    case FVI_VIDEO_MODE_240P_263:
    case FVI_VIDEO_MODE_480P:
        return DMCU_VIDEO_STANDARD_NTSC;
    case FVI_VIDEO_MODE_576I:
    case FVI_VIDEO_MODE_288P_312:
    case FVI_VIDEO_MODE_288P_313:
    case FVI_VIDEO_MODE_576P:
        return DMCU_VIDEO_STANDARD_PAL;
    default:
        return DMCU_VIDEO_STANDARD_INVALID;
    }
}

// TODO move and rename this
typedef struct {
    uint16_t unk0x00;
    uint16_t unk0x02;
    uint16_t unk0x04;
    uint16_t unk0x06;
    uint16_t unk0x08;
    uint16_t unk0x0A;
    uint16_t unk0x0C;
    uint16_t unk0x0E;
    uint16_t unk0x10;
    uint32_t unk0x12;
    uint32_t unk0x16;
    uint32_t unk0x1A;
    uint16_t unk0x1E;
    uint16_t unk0x20;
    uint32_t unk0x22;
    uint32_t unk0x26;
    uint16_t unk0x2A;
    uint16_t unk0x2C;
    uint16_t unk0x2E;
    uint16_t unk0x30;
    uint32_t unk0x32;
} FUN355cParams;

FUN355cParams fun355cParams0 = {
    .unk0x00 = 0x0,
    .unk0x02 = 0x1,
    .unk0x04 = 0x1,
    .unk0x06 = 0x0,
    .unk0x08 = 0x0,
    .unk0x0A = 0x0,
    .unk0x0C = 0x0,
    .unk0x0E = 0x0,
    .unk0x10 = 0xC,
    .unk0x12 = 0x1F4,
    .unk0x16 = 0x0,
    .unk0x1A = 0xC,
    .unk0x1E = 0x0,
    .unk0x20 = 0xC,
    .unk0x22 = 0xC,
    .unk0x26 = 0x0,
    .unk0x2A = 0x0,
    .unk0x2C = 0x0,
    .unk0x2E = 0x0,
    .unk0x30 = 0x0,
    .unk0x32 = 0x3D,
};

FUN355cParams fun355cParams1 = {
    .unk0x00 = 0x0,
    .unk0x02 = 0x1,
    .unk0x04 = 0x1,
    .unk0x06 = 0x0,
    .unk0x08 = 0x0,
    .unk0x0A = 0x0,
    .unk0x0C = 0x0,
    .unk0x0E = 0x0,
    .unk0x10 = 0x7,
    .unk0x12 = 0x134,
    .unk0x16 = 0x0,
    .unk0x1A = 0xC,
    .unk0x1E = 0x0,
    .unk0x20 = 0x6,
    .unk0x22 = 0x42,
    .unk0x26 = 0x0,
    .unk0x2A = 0x0,
    .unk0x2C = 0x0,
    .unk0x2E = 0x0,
    .unk0x30 = 0x0,
    .unk0x32 = 0x25,
};

static void FUN_3532(void)
{
    DC_Write32(0x110f, 0x1);
    DC_Write32(0x111f, 0x1);
}

static void FUN_355c(uint16_t param_1)
{
    FUN355cParams* params;
    if (param_1 == 0) {
        params = &fun355cParams0;
    } else if (param_1 == 1) {
        params = &fun355cParams1;
    } else {
        return;
    }

    FUN_3532();

    uint32_t reg0x1109 = DC_Read32(0x1109);
    if (((reg0x1109 >> 31) & 1) != 0) {
        reg0x1109 &= 0x7FFFFFFF;
        DC_Write32(0x1109, reg0x1109);
        udelay(5);
    }

    DC_Write32(0x1109, reg0x1109 & 0xBFFFFFFF);
    udelay(10);

    DC_Write32(0x1108, (params->unk0x12 << 0x6) | (params->unk0x10 & 0x3F) | (((params->unk0x1A << 0x12) | ((params->unk0x0E & 0xF803ul) << 0x10)) & 0xFFFCFFFFul) | 0x80000000ul);

    uint32_t value = ((params->unk0x22 << 0x9) | ((params->unk0x1E & 0xFFFCul) << 0x10)) & 0xF7FFFFFFul;
    if (params->unk0x04) {
        value |= 0x8000000;
    }

    value &= 0xEFFFFFFF;

    if (params->unk0x00) {
        value |= 0x10000000;
    }

    DC_Write32(0x1109, (value & 0x1FFFFFFF) | (params->unk0x20 & 0x1FF));

    DC_Write32(0x110A, ((uint32_t) params->unk0x02 << 0x1B) | params->unk0x30 | (((params->unk0x2E & 0xFC00ul) | params->unk0x2C) & 0xF3FFul));

    DC_Write32(0x110B, (params->unk0x32 << 0xC) | (params->unk0x26 & 0xFF000FFF));

    DC_Write32(0x110C, params->unk0x16);

    udelay(5);

    value = ((((((params->unk0x22 << 0x9) | ((params->unk0x1E & 0xFFFCul) << 0x10)) & 0xF7FFFFFF) | ((uint32_t) params->unk0x04 << 0x1B)) & 0xEFFFFFFF) | ((uint32_t) params->unk0x00 << 0x1C)) & 0x1FFFFFFF;
    value |= 0x40000000;
    value |= params->unk0x20 & 0x1FF;

    DC_Write32(0x1109, value);

    udelay(0x32);

    DC_Write32(0x1109, (value /*& 0xFFFFFF00*/) | 0x80000000);
}

static void FUN_1669(void)
{
    FUN_355c(1);
    udelay(0x9c40);

    if (gTVMode == OP_MODE_1080P && gDRCMode == OP_MODE_OTHER) {
        DC_Write8(DEINT_RSVD2, 0x0, 0x1a);
        DC_Write8(DEINT_RSVD2, 0x0, 0x1b);
        DC_Write8(DEINT_RSVD2, 0x0, 0x17);
        DC_Write32(0x1ce0, 0x10000);
        udelay(0x9c40);
        return;
    }

    if (gTVMode == OP_MODE_OTHER && gDRCMode == OP_MODE_1080P) {
        DC_Write8(DEINT_RSVD2, 0x0, 0x5a);
        DC_Write8(DEINT_RSVD2, 0x0, 0x5b);
        DC_Write8(DEINT_RSVD2, 0x0, 0x5e);
        DC_Write32(0x1ce0, 0x0);
        udelay(0x9c40);
        return;
    }
}

static void FUN_1732(void)
{
    FUN_355c(0);
    udelay(0x9c40);

    if (gTVMode == OP_MODE_1080P) {
        DC_Write8(DEINT_RSVD2, 0x0, 0x1b);
        DC_Write8(DEINT_RSVD2, 0x0, 0x1a);
        DC_Write8(DEINT_RSVD2, 0x0, 0x16);
        DC_Write32(0x1ce0, 0x0);
    } else if (gTVMode == OP_MODE_OTHER && gDRCMode == OP_MODE_1080P) {
        DC_Write8(DEINT_RSVD2, 0x0, 0x5b);
        DC_Write8(DEINT_RSVD2, 0x0, 0x5a);
        DC_Write8(DEINT_RSVD2, 0x0, 0x5e);
        DC_Write32(0x1ce0, 0x10000);
    }

    udelay(0x9c40);
}

void configureVideoStandardHD(DMCUState* state, FVIVideoMode videoMode)
{
    DMCUVideoStandard videoStandard = getVideoStandard(videoMode);
    if (videoStandard == DMCU_VIDEO_STANDARD_INVALID) {
        DEBUG_SetLowMarker(0x8);
        DEBUG_SetHighMarker(state->currentVideoMode);
        return;
    }

    state->currentVideoMode = videoMode;
    if (state->currentVideoStandard == videoStandard) {
        return;
    }

    DVO_SetEnable(0);
    DVO_StopVIClockAll();
    CRTC_DisableAll();

    if (videoStandard == DMCU_VIDEO_STANDARD_NTSC) {
        FUN_1732();
        DEINT_17f2();

        GRPH_SetD1Viewport(0x2c7, 0x1da, 0x2d0, 0x1e0, gTVXStart, gTVYStart);
        GRPH_SetD2Viewport(0x2b0, 0x1ca, 0x2d0, 0x1e0, 0x10, 0xe);

        if (gTVMode == OP_MODE_OTHER) {
            CRTC_SetD1Config(&crtcConfigs[5]);
        } else if (gTVMode == OP_MODE_1080I) {
            CRTC_SetD1Config(&crtcConfigs[3]);
        } else if (gTVMode == OP_MODE_1080P) {
            CRTC_SetD1Config(&crtcConfigs[1]);
        }

        CRTC_SetD2Config(&crtcConfigs[8]);
        state->currentVideoStandard = DMCU_VIDEO_STANDARD_NTSC;
        DVO_SetVideoFormat(FVI_VIDEO_MODE_480I);
        DVO_SetRefreshRate(1, FVI_VIDEO_MODE_480I);
    } else if (videoStandard == DMCU_VIDEO_STANDARD_PAL) {
        FUN_1669();
        DEINT_17dc();

        GRPH_SetD1Viewport(0x2c6, 0x238, 0x2d0, 0x240, gTVXStart, gTVYStart);
        GRPH_SetD2Viewport(0x2b0, 0x226, 0x2d0, 0x240, 0x10, 0xe);

        if (gTVMode == OP_MODE_OTHER) {
            CRTC_SetD1Config(&crtcConfigs[4]);
        } else if (gTVMode == OP_MODE_1080I) {
            CRTC_SetD1Config(&crtcConfigs[2]);
        } else if (gTVMode == OP_MODE_1080P) {
            CRTC_SetD1Config(&crtcConfigs[0]);
        }

        CRTC_SetD2Config(&crtcConfigs[9]);
        state->currentVideoStandard = DMCU_VIDEO_STANDARD_PAL;
        DVO_SetVideoFormat(FVI_VIDEO_MODE_576I);
        DVO_SetRefreshRate(1, FVI_VIDEO_MODE_576I);
    }

    CRTC_EnableAll();
    CRTC_WaitUpdate();
    FUN_1eca();
    DVO_SetEnable(1);
    DVO_RunVIClockAll();
    udelay(0x286);
}

void configureVideoStandardSD(DMCUState* state, FVIVideoMode videoMode)
{
    DMCUVideoStandard videoStandard = getVideoStandard(videoMode);
    if (videoStandard == DMCU_VIDEO_STANDARD_INVALID) {
        DEBUG_SetLowMarker(0x8);
        DEBUG_SetHighMarker(state->currentVideoMode);
        return;
    }

    state->currentVideoMode = videoMode;
    if (state->currentVideoStandard == videoStandard) {
        return;
    }

    DVO_SetEnable(0);
    DVO_StopVIClock(DISPLAY_DRC);
    CRTC_DisableAll();

    if (videoStandard == DMCU_VIDEO_STANDARD_NTSC) {
        DEINT_17f2();
        GRPH_SetD1Viewport(0x2c8, 0x1da, 0x2d0, 0x1e0, 0x4, 0x2);
        GRPH_SetD2Viewport(0x2b0, 0x1ca, 0x2d0, 0x1e0, 0x10, 0xe);
        CRTC_SetD1Config(&crtcConfigs[6]);
        CRTC_SetD2Config(&crtcConfigs[8]);
        state->currentVideoStandard = DMCU_VIDEO_STANDARD_NTSC;
        DVO_SetVideoFormat(FVI_VIDEO_MODE_480I);
        DVO_SetRefreshRate(2, FVI_VIDEO_MODE_480I);
    } else {
        DEINT_17dc();
        GRPH_SetD1Viewport(0x2c8, 0x239, 0x2d0, 0x240, 0x4, 0x2);
        GRPH_SetD2Viewport(0x2b0, 0x226, 0x2d0, 0x240, 0x10, 0xe);
        CRTC_SetD1Config(&crtcConfigs[7]);
        CRTC_SetD2Config(&crtcConfigs[9]);
        state->currentVideoStandard = DMCU_VIDEO_STANDARD_PAL;
        DVO_SetVideoFormat(FVI_VIDEO_MODE_576I);
        DVO_SetRefreshRate(2, FVI_VIDEO_MODE_576I);
    }

    CRTC_EnableAll();
    CRTC_WaitUpdate();
    FUN_1eca();
    DVO_RunVIClock(DISPLAY_DRC);
    DVO_SetEnable(1);
    udelay(0x286);
}

void HandleOverscanConfigurationRequest(DMCUState* state)
{
    if (state->pendingOverscanConfiguration == TRUE) {
        IRQ_Lock();
        OVERSCANHandlerFn handler = OVERSCAN_GetHandler();
        if (handler) {
            handler(state);
        }

        state->pendingOverscanConfiguration = FALSE;
        IRQ_Unlock();
    }
}

int main()
{
    DMCUEventHandlerFn eventHandler;
    uint16_t eventMask;

    DMCU_Setup();

    // TODO cleanup
    while (1) {
loop_start:
        getEventHandler(&eventHandler, &eventMask, (uint8_t)gDmcuState.stateFlags);

        switch (eventMask) {
        case 0x1:
            gMaskCount[0]++;
            break;
        case 0x2:
            gMaskCount[1]++;
            break;
        case 0x4:
            gMaskCount[2]++;
            break;
        case 0x8:
            gMaskCount[3]++;
            break;
        }
        DEBUG_Write(0x0, ((uint32_t) gMaskCount[0] << 24) | ((uint32_t) gMaskCount[1] << 16) | (gMaskCount[2] << 8) | gMaskCount[3]);
        DEBUG_Write(0x2, ((uint32_t) gVideoMode << 16) | ((gDmcuState.currentVideoMode & 0xFF) << 12) | (gDmcuState.requestedVideoMode << 8) | (gDmcuState.unk0x0E << 4) | gDmcuState.currentVideoStandard);

        if (eventHandler != NULL) {
            IRQ_Lock();
            gDmcuState.stateFlags = (~eventMask & gDmcuState.stateFlags) | (eventMask << 0x4);
            IRQ_Unlock();

            do {
                eventHandler(&gDmcuState);

                uint8_t bVar5 = gDmcuState.stateFlags >> 8;
                uint16_t uVar2 = eventMask;
                if (((bVar5 >> 4) & eventMask) == 0) {
                    if ((bVar5 & eventMask & 0xf) == 0) {
                        IRQ_Lock();
                        gDmcuState.stateFlags &= ~(eventMask << 0x4);
                        IRQ_Unlock();

                        goto loop_start;
                    }
                } else {
                    uVar2 = eventMask << 4;
                }

                gDmcuState.stateFlags = ((~uVar2 & bVar5) << 8) | (gDmcuState.stateFlags & 0xFF);
            } while (1);
        }

        BOOL res = FVI_GetVideoMode(&gVideoMode);
        if (res == TRUE && gVideoMode != FVI_VIDEO_MODE_INVALID) {
            break;
        }
    }

    HandleOverscanConfigurationRequest(&gDmcuState);

    if (gVideoMode != gDmcuState.currentVideoMode) {
        if (CONF_IsHD(DISPLAY_TV)) {
            configureVideoStandardHD(&gDmcuState, gVideoMode);
        } else if (CONF_IsSD(DISPLAY_TV)) {
            configureVideoStandardSD(&gDmcuState, gVideoMode);
        } else {
            goto loop_start;
        }
    }

    if (NeedsUnblank(&gDmcuState)) {
        CRTC_Unblank();
        CRTC_WaitUpdate();
    } else {
        if (IsVideoModeSet(&gDmcuState) != TRUE) {
            if (CRTC_IsBlanked() != TRUE) {
                CRTC_Blank();
                CRTC_WaitUpdate();
            }

            gDmcuState.stateFlags |= 2;
        }
    }

    goto loop_start;

    return 0;
}

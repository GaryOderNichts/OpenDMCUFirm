/*
 *   This file is part of OpenDMCUFirm
 */
#include "deint.h"
#include "dmcu.h"
#include "fvi.h"
#include "dc.h"
#include "debug.h"
#include "timer.h"
#include "conf.h"
#include "irq.h"

typedef struct DEINTIfTimParams {
    uint32_t topPorchOdd;
    uint32_t topPorchEven;
    uint32_t height;
    uint32_t preHBlank;
} DEINTIfTimParams;

DEINTIfTimParams gDeintIfTimParams;
static uint32_t DAT_0120 = 0; // Why this is a 32-bit var? That's a great question

void DEINT_Lock(void)
{
    FVI_Wait();
    DC_Clear8(DEINT_CONFIG, 0, DEINT_BLK_EN);
    DC_Set8(DEINT_CONFIG, 0, DEINT_SOFT_RESET_EN);
}

void DEINT_Unlock(void)
{
    FVI_Wait();
    DC_Clear8(DEINT_CONFIG, 0, DEINT_SOFT_RESET_EN);
    DC_Set8(DEINT_CONFIG, 0, DEINT_BLK_EN);
}

DEINTIfTimParams* DEINT_GetIfTimParams(FVIVideoMode mode)
{
    DEINTIfTimParams* params;
    switch (mode) {
    case FVI_VIDEO_MODE_480I:
    case FVI_VIDEO_MODE_240P_262:
    case FVI_VIDEO_MODE_240P_263:
        gDeintIfTimParams.topPorchOdd = 0x24;
        gDeintIfTimParams.topPorchEven = 0x25;
        gDeintIfTimParams.height = 240;
        gDeintIfTimParams.preHBlank = 0x77;
        params = &gDeintIfTimParams;
        break;
    case FVI_VIDEO_MODE_576I:
    case FVI_VIDEO_MODE_288P_312:
    case FVI_VIDEO_MODE_288P_313:
        gDeintIfTimParams.topPorchOdd = 0x2B;
        gDeintIfTimParams.topPorchEven = 0x2C;
        gDeintIfTimParams.height = 288;
        gDeintIfTimParams.preHBlank = 0x84;
        params = &gDeintIfTimParams;
        break;
    case FVI_VIDEO_MODE_480P:
        gDeintIfTimParams.topPorchOdd = 0x48;
        gDeintIfTimParams.topPorchEven = 0x48;
        gDeintIfTimParams.height = 480;
        gDeintIfTimParams.preHBlank = 0x7A;
        params = &gDeintIfTimParams;
        break;
    case FVI_VIDEO_MODE_576P:
        gDeintIfTimParams.topPorchOdd = 0x58;
        gDeintIfTimParams.topPorchEven = 0x58;
        gDeintIfTimParams.height = 576;
        gDeintIfTimParams.preHBlank = 0x84;
        params = &gDeintIfTimParams;
        break;
    default:
        DEBUG_SetLowMarker(0x2);
        DEBUG_SetHighMarker(0x4000);
        params = NULL;
        break;
    }

    if (params != NULL) {
        DEBUG_ClearMarker(0x2);
    }

    return params;
}

uint16_t DEINT_SetIfTimParams(DEINTIfTimParams* params)
{
    DC_Write32(DEINT_IF_TIM_RESOLUTION, DEINT_IF_TIM_RES_HEIGHT(params->height) | DEINT_IF_TIM_RES_WIDTH(720));
    DC_Write32(DEINT_IF_TIM_TOP_PORCH, DEINT_IF_TIM_TOP_PORCH_EVEN(params->topPorchEven) | DEINT_IF_TIM_TOP_PORCH_ODD(params->topPorchOdd));
    DC_Write32(DEINT_IF_TIM_HBLANK, DEINT_IF_TIM_PRE_HBLANK(params->preHBlank));
    return 1;
}

void DEINT_Configure(FVIVideoMode mode)
{
    DEBUG_Write0x6(0x2);

    DEINT_Lock();

    DC_Write32(0x10a6, 0x0);
    DC_Write32(0x10a7, 0x0);
    DC_Write32(0x10a5, 0x0);
    DC_Write32(0x109e, 0x0);
    DC_Write32(0x10b9, 0x0);
    DC_Write32(0x10ae, 0x0);
    DC_Write32(0x109e, 0x0);
    DC_Write32(0x10b5, 0x0);
    DC_Write32(0x10b9, 0x0);

    DEINTIfTimParams* params = DEINT_GetIfTimParams(mode);
    if (params) {
        DEINT_SetIfTimParams(params);
    }

    gDmcuState.deintNeedsUpdate = 0;

    DEBUG_Write0x6(0x82);

    DEINT_Unlock();
}

void FUN_1292(void)
{
    if (DC_Read32(0x10A0) == 0x01000000)
    {
        // TODO yuck, not sure if this is even right
        uint32_t count = 0;
        uint16_t endLoop = 0;
        do {
            endLoop = 0;
            if ((DC_Read32(0x108B) >> 16) <= 0x4) {
                endLoop = 1;
            }

            count++;
        } while (!endLoop && (count < 0x30000));

        if (count >= 0x30000) {
            DEBUG_SetLowMarker(0x1);
            DEBUG_SetHighMarker(0x2);
        }
    }
}

void DEINT_EnableKernel(void)
{
    udelay(0x20);
    DC_Set8(DEINT_CONFIG, 0x0, DEINT_KERNEL_EN);
}

void DEINT_EnableMAD(void)
{
    DC_Write32(DEINT_CONFIG, DC_Read32(DEINT_CONFIG) | DEINT_KERNEL_EN | DEINT_MAD_EN);
    DEBUG_Write0x6(4);
}

void DEINT_Reconfigure(void)
{
    DEBUG_Write0x6(0x2);

    DEINT_Lock();

    FUN_1292();

    DC_Write32(0x10a6, 0x0);
    DC_Write32(0x10a7, 0x0);
    DC_Write32(0x10a5, 0x0);
    DC_Write32(0x109e, 0x0);
    DC_Write32(0x10b9, 0x0);
    DC_Write32(0x10ae, 0x0);
    DC_Write32(0x109e, 0x0);
    DC_Write32(0x10b5, 0x0);
    DC_Write32(0x10b9, 0x0);

    DEINTIfTimParams* params = DEINT_GetIfTimParams(gVideoMode);
    if (params) {
        DEINT_SetIfTimParams(params);
    }

    DEBUG_Write0x6(0x4);

    DEINT_EnableKernel();
    DEINT_EnableMAD();

    gDmcuState.deintNeedsUpdate = 0;

    DEINT_Unlock();
}

void DEINT_DisableKernel(void)
{
    udelay(0x20);
    DC_Clear8(DEINT_CONFIG, 0x0, DEINT_KERNEL_EN);
}

void DEINT_DisableMAD(void)
{
    DEBUG_Write0x6(0x8);
    DC_Clear8(DEINT_CONFIG, 0x1, 0x10); // DEINT_MAD_EN
}

void D1SCALER_Enable(void)
{
    DC_Write32(D1SCL_SCALER_ENABLE, SCL_SCALE_EN);
}

void D2SCALER_Enable(void)
{
    DC_PerformRead(D2SCL_SCALER_ENABLE);
    DC_Write8(D2SCL_SCALER_ENABLE, 0, DC_Get8(0) | SCL_SCALE_EN);
}

uint16_t DEINT_139a(FVIVideoMode videoMode)
{
    DEINTIfTimParams* params = DEINT_GetIfTimParams(videoMode);
    if (!params) {
        DEBUG_SetLowMarker(0x100);
        return 0;
    }

    DEINT_SetIfTimParams(params);

    if (videoMode == FVI_VIDEO_MODE_480P || videoMode == FVI_VIDEO_MODE_576P) {
        DEBUG_Write0x6(0x8);
        DEINT_DisableKernel();
        DEINT_DisableMAD();
    } else {
        DEBUG_Write0x6(0x8);
        DEINT_EnableKernel();
        DEINT_EnableMAD();
    }

    if (CONF_IsHD(DISPLAY_TV)) {
        D1SCALER_Enable();
    }

    if (CONF_IsHD(DISPLAY_DRC)) {
        D2SCALER_Enable();
    }

    return 1;
}

void FUN_133f(void)
{
    IRQ_Lock();

    DC_PerformRead(0x108B);
    while (((DC_Get8(0x3) << 8) | DC_Get8(2)) > 0x5)
        ;

    DC_Write32(D1CRTC_COUNT_RESET, CRTC_RESET_FRAME_COUNT);

    IRQ_Unlock();
}

void FUN_0f99(void)
{
    DEINT_Lock();

    FUN_1292();

    DC_Write32(0x10a6, 0x0);
    DC_Write32(0x10a7, 0x0);

    FUN_133f();

    DAT_0120 = 0;

    DEINT_Unlock();

    DEBUG_Write0x6(0x80);
}

void FUN_0e3b(FVIVideoMode videoMode, FVIState* state)
{
    uint16_t local_6 = 0;
    uint32_t uVar1 = 0;
    uint16_t uVar2 = 0;
    if (videoMode == FVI_VIDEO_MODE_288P_312 || videoMode == FVI_VIDEO_MODE_240P_262) {
        local_6 = state->unk0x0A;
        uVar1 = state->unk0x10 + 1;
        uVar2 = state->unk0x0E - 1;
    } else if (videoMode == FVI_VIDEO_MODE_288P_313 || videoMode == FVI_VIDEO_MODE_240P_263) {
        local_6 = state->unk0x0A;
        uVar1 = state->unk0x10 - 1;
        uVar2 = state->unk0x0E + 1;
    }

    DEINT_Lock();

    FUN_1292();

    DC_Write32(0x10A6, uVar1 | ((uint32_t) local_6 << 16) | 0x80008000ul);
    DC_Write32(0x10A7, ((uint32_t) uVar2 << 16) | 0x80008000ul);

    DAT_0120 = 1;

    FUN_133f();

    DEINT_Unlock();

    DEBUG_Write0x6(0x40);
}

uint16_t DEINT_0dff(FVIVideoMode videoMode, FVIState* state)
{
    if (!state) {
        return 0;
    }

    if (videoMode == FVI_VIDEO_MODE_480I || videoMode == FVI_VIDEO_MODE_576I) {
        if (DAT_0120) {
            FUN_0f99();
        }
    } else {
        FUN_0e3b(videoMode, state);
    }

    return DEINT_139a(videoMode);
}

uint16_t DEINT_0de4(FVIVideoMode videoMode)
{
    if (DAT_0120) {
        FUN_0f99();
    }

    return DEINT_139a(videoMode);
}

void DEINT_EnableCBCRSwap(void)
{
    DC_Set8(DEINT_CONFIG, 0x0, CBCR_SWAP);
}

void DEINT_DisableCBCRSwap(void)
{
    DC_Clear8(DEINT_CONFIG, 0x0, CBCR_SWAP);
}

void DEINT_ConfigureCBCRSwap(BOOL* outEnabled)
{
    DC_PerformRead(0x1082);
    uint8_t bVar1 = DC_Get8(0);

    DEINT_Lock();

    if (bVar1 & 0x1) {
        DEINT_DisableCBCRSwap();
        *outEnabled = FALSE;
    } else {
        DEINT_EnableCBCRSwap();
        *outEnabled = TRUE;
    }

    DEINT_Unlock();
}

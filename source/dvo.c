/*
 *   This file is part of OpenDMCUFirm
 */
#include "dvo.h"
#include "dc.h"
#include "crtc.h"
#include "irq.h"
#include "debug.h"

void DVO_SetEnable(uint16_t enable)
{
    if (enable) {
        while (CRTC_GetVertCount() != 2)
            ;
    }

    const uint8_t format = 1; // Selects pixel encoding format
                              //  0=RGB 4:4:4 or YCBCR 4:4:4
                              //  1=YCbCr 4:2:2
    const uint32_t val = enable | (format << 8);
    DC_Write32(DVOA_ENABLE, val);
    DC_Write32(DVOB_ENABLE, val);
}

void DVO_SetVideoFormat(FVIVideoMode videoMode)
{
    // This switch probably decides between NTSC/PAL?
    // See getNTSC_PAL / FUN_1bdc
    switch (videoMode) {
    case FVI_VIDEO_MODE_UNK1:
    case FVI_VIDEO_MODE_UNK5:
    case FVI_VIDEO_MODE_UNK6:
    case FVI_VIDEO_MODE_480P:
        IRQ_Lock();
        DC_PerformRead(DVOA_CONFIG);
        DC_Write8(DVOA_CONFIG, 0, DC_Get8(0) & ~DVO_VIDEO_FORMAT(DVO_VIDEO_FORMAT_MASK));
        DC_PerformRead(DVOB_CONFIG);
        DC_Write8(DVOB_CONFIG, 0, DC_Get8(0) & ~DVO_VIDEO_FORMAT(DVO_VIDEO_FORMAT_MASK));
        IRQ_Unlock();
        break;
    
    case FVI_VIDEO_MODE_UNK2:
    case FVI_VIDEO_MODE_UNK3:
    case FVI_VIDEO_MODE_UNK4:
    case FVI_VIDEO_MODE_576I:
        IRQ_Lock();
        DC_PerformRead(DVOA_CONFIG);
        DC_Write8(DVOA_CONFIG, 0, (DC_Get8(0) & 0xcf) | DVO_VIDEO_FORMAT(1));
        DC_PerformRead(DVOB_CONFIG);
        DC_Write8(DVOB_CONFIG, 0, (DC_Get8(0) & 0xdf) | DVO_VIDEO_FORMAT(1));
        IRQ_Unlock();
        break;

    default:
        DEBUG_SetLowMarker(0x2000);
        DEBUG_SetHighMarker(videoMode);
        break;
    }
}

void DVO_SetRefreshRate(uint16_t param_1, FVIVideoMode videoMode)
{
    uint16_t val = 0;
    switch (videoMode) {
    case FVI_VIDEO_MODE_UNK1:
    case FVI_VIDEO_MODE_UNK5:
    case FVI_VIDEO_MODE_UNK6:
    case FVI_VIDEO_MODE_480P:
        if (param_1 == TRUE) {
            val = 4 | 1; // DVO_REFRESH_RATE_EN | DVO_REFRESH_RATE_FLAGS(2)
        }
        break;
    
    case FVI_VIDEO_MODE_UNK2:
    case FVI_VIDEO_MODE_UNK3:
    case FVI_VIDEO_MODE_UNK4:
    case FVI_VIDEO_MODE_576I:
        if (param_1 == TRUE) {
            val = 2 | 1; // DVO_REFRESH_RATE_EN | DVO_REFRESH_RATE_FLAGS(1)
        }
        break;

    default:
        DEBUG_SetLowMarker(0x1000);
        DEBUG_SetHighMarker(0x1);
        break;
    }

    DC_Write8(DVOA_CONFIG, 1, val);
    DC_Write8(DVOB_CONFIG, 1, val);
}

void DVO_StopVIClock(DisplayType clock)
{
    uint16_t reg;
    if (clock == DISPLAY_TV) {
        reg = DC_GPIO_DVOADATA_MASK;
    } else if (clock == DISPLAY_DRC) {
        reg = DC_GPIO_DVOBDATA_MASK;
    } else {
        return;
    }

    DC_Write32(reg, (1ul << 28));
}

void DVO_RunVIClock(DisplayType clock)
{
    uint16_t reg;
    if (clock == DISPLAY_TV) {
        reg = DC_GPIO_DVOADATA_MASK;
    } else if (clock == DISPLAY_DRC) {
        reg = DC_GPIO_DVOBDATA_MASK;
    } else {
        return;
    }

    DC_Write32(reg, 0);
}

void DVO_StopVIClockAll(void)
{
    DC_Write32(DC_GPIO_DVOADATA_MASK, DC_GPIO_DVOCLK_MASK);
    DC_Write32(DC_GPIO_DVOBDATA_MASK, DC_GPIO_DVOCLK_MASK);
}

void DVO_RunVIClockAll(void)
{
    DC_Write32(DC_GPIO_DVOADATA_MASK, 0);
    DC_Write32(DC_GPIO_DVOBDATA_MASK, 0);
}

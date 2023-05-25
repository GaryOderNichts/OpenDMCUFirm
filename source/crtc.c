/*
 *   This file is part of OpenDMCUFirm
 */
#include "crtc.h"
#include "dc.h"

uint16_t gCRTCIsBlanked = 1;

void CRTC_WaitUpdate(void)
{
    while (DC_Read32(D1CRTC_DOUBLE_BUFFER_CONTROL) & CRTC_UPDATE_PENDING)
        ;
}

void CRTC_Blank(void)
{
    CRTC_WaitUpdate();

    // Set D1CRTC_BLANK_DATA_EN for D1 and D2
    DC_Write8(D1CRTC_BLANK_CONTROL, 1, 1);
    DC_Write8(D2CRTC_BLANK_CONTROL, 1, 1);

    CRTC_WaitUpdate();

    gCRTCIsBlanked = 1;
}

void CRTC_Unblank(void)
{
    CRTC_WaitVertPosition();
    CRTC_WaitUpdate();

    DC_Write8(D1CRTC_BLANK_CONTROL, 1, 0);
    DC_Write8(D2CRTC_BLANK_CONTROL, 1, 0);

    CRTC_WaitUpdate();

    gCRTCIsBlanked = 0;
}

uint8_t CRTC_IsBlanked(void)
{
    DC_PerformRead(D1CRTC_BLANK_CONTROL);
    return DC_Get8(0x0) & CRTC_CURRENT_BLANK_STATE;
}

uint32_t CRTC_GetVertCount(void)
{
    return DC_Read32(D1CRTC_STATUS_POSITION) & CRTC_VERT_COUNT_MASK;
}

void CRTC_WaitVertPosition(void)
{
    while (CRTC_GetVertCount() != 0x3)
        ;
}

void CRTC_DisableAll(void)
{
    CRTC_WaitUpdate();

    CRTC_SetEnable(1, 0);
    CRTC_SetEnable(2, 0);

    CRTC_WaitDisabled();
}

void CRTC_EnableAll(void)
{
    CRTC_SetEnable(1, 1);
    CRTC_SetEnable(2, 1);
}

void CRTC_SetEnable(DisplayType display, uint16_t enable)
{
    // Set/Clear D1CRTC_MASTER_EN/D2CRTC_MASTER_EN
    uint16_t ctl_reg = display == DISPLAY_DRC ? D2CRTC_CONTROL : D1CRTC_CONTROL;
    uint32_t ctl_val = DC_Read32(ctl_reg);
    if (enable) {
        ctl_val |= CRTC_MASTER_EN;
    } else {
        ctl_val &= ~CRTC_MASTER_EN;
    }
    DC_Write32(ctl_reg, ctl_val);

    // Set/Clear D1MODE_MASTER_UPDATE_LOCK/D2MODE_MASTER_UPDATE_LOCK
    DC_Write32(display == DISPLAY_DRC ? D2MODE_MASTER_UPDATE_LOCK : D1MODE_MASTER_UPDATE_LOCK, !enable);
}

void CRTC_WaitDisabled(void)
{
    // Wait on D1CRTC_CURRENT_MASTER_EN_STATE
    while (DC_Read32(D1CRTC_CONTROL) & CRTC_CURRENT_MASTER_EN_STATE)
        ;
}

void CRTC_SetD1Config(const CRTCConfig* conf)
{
    DC_Write32(D1CRTC_H_TOTAL, conf->hTotal);
    DC_Write32(D1CRTC_H_BLANK_START_END, conf->hBlankStartEnd);
    DC_Write32(D1CRTC_H_SYNC_A, conf->hSyncA);
    DC_Write32(D1CRTC_V_TOTAL, conf->vTotal);
    DC_Write32(D1CRTC_V_BLANK_START_END, conf->vBlankStartEnd);
    DC_Write32(D1CRTC_V_SYNC_A, conf->vSyncA);
}

void CRTC_SetD2Config(const CRTCConfig* conf)
{
    DC_Write32(D2CRTC_H_TOTAL, conf->hTotal);
    DC_Write32(D2CRTC_H_BLANK_START_END, conf->hBlankStartEnd);
    DC_Write32(D2CRTC_H_SYNC_A, conf->hSyncA);
    DC_Write32(D2CRTC_V_TOTAL, conf->vTotal);
    DC_Write32(D2CRTC_V_BLANK_START_END, conf->vBlankStartEnd);
    DC_Write32(D2CRTC_V_SYNC_A, conf->vSyncA);
}

// This is not actually in the dmcu but I wanted something like this for testing
void CRTC_SetBlackColor(uint16_t y, uint16_t cb, uint16_t cr)
{
    DC_Write32(D1CRTC_BLACK_COLOR, CRTC_BLACK_COLOR_R_CR(cr) | CRTC_BLACK_COLOR_G_Y(y) | CRTC_BLACK_COLOR_B_CB(cb));
    DC_Write32(D2CRTC_BLACK_COLOR, CRTC_BLACK_COLOR_R_CR(cr) | CRTC_BLACK_COLOR_G_Y(y) | CRTC_BLACK_COLOR_B_CB(cb));
}

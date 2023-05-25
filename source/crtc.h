/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_CRTC_H_
#define _DMCU_CRTC_H_

#include "types.h"
#include "conf.h"

typedef struct CRTCConfig CRTCConfig;

struct CRTCConfig {
    uint32_t hTotal;
    uint32_t hBlankStartEnd;
    uint32_t hSyncA;
    uint32_t vTotal;
    uint32_t vBlankStartEnd;
    uint32_t vSyncA;
};

void CRTC_WaitUpdate(void);

void CRTC_Blank(void);

void CRTC_Unblank(void);

uint8_t CRTC_IsBlanked(void);

uint32_t CRTC_GetVertCount(void);

void CRTC_WaitVertPosition(void);

void CRTC_DisableAll(void);

void CRTC_EnableAll(void);

void CRTC_SetEnable(DisplayType display, uint16_t enable);

void CRTC_WaitDisabled(void);

void CRTC_SetD1Config(const CRTCConfig* conf);

void CRTC_SetD2Config(const CRTCConfig* conf);

void CRTC_SetBlackColor(uint16_t y, uint16_t cb, uint16_t cr);

#endif

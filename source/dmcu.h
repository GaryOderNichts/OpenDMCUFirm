/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_DMCU_H_
#define _DMCU_DMCU_H_

#include "types.h"
#include "fvi.h"

typedef struct DMCUState DMCUState;

struct DMCUState {
    uint16_t stateFlags;
    uint32_t reg0x1060;
    BOOL isVisible;
    FVIVideoMode currentVideoMode;
    FVIVideoMode requestedVideoMode;
    BOOL cbcrSwapEnabled;
    uint16_t unk0x0E;
    // not sure what to call this
    uint16_t palMode;
    int16_t deintNeedsUpdate;
    uint16_t unk0x14;
    BOOL pendingOverscanConfiguration;
    uint16_t yStart;
};

extern DMCUState gDmcuState;
extern FVIVideoMode gVideoMode;

void DMCU_HandleIrq(void);

void DMCU_HandleTimerOverflow(void);

#endif

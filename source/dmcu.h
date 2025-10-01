/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_DMCU_H_
#define _DMCU_DMCU_H_

#include "types.h"
#include "fvi.h"

typedef struct DMCUState DMCUState;

typedef enum {
    DMCU_VIDEO_STANDARD_INVALID = 0,
    DMCU_VIDEO_STANDARD_NTSC    = 1,
    DMCU_VIDEO_STANDARD_PAL     = 2,
} DMCUVideoStandard;

struct DMCUState {
    uint16_t stateFlags;
    uint32_t reg0x1060;
    BOOL isVisible;
    FVIVideoMode currentVideoMode;
    FVIVideoMode requestedVideoMode;
    BOOL cbcrSwapEnabled;
    uint16_t unk0x0E;
    DMCUVideoStandard currentVideoStandard;
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

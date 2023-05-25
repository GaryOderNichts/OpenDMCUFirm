/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_DVO_H_
#define _DMCU_DVO_H_

#include "types.h"
#include "fvi.h"

void DVO_SetEnable(uint16_t enable);

void DVO_SetVideoFormat(FVIVideoMode videoMode);

void DVO_SetRefreshRate(uint16_t param_1, FVIVideoMode videoMode);

void DVO_StopVIClock(DisplayType clock);

void DVO_RunVIClock(DisplayType clock);

void DVO_StopVIClockAll(void);

void DVO_RunVIClockAll(void);

#endif

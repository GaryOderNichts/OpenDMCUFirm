/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_DEINT_H_
#define _DMCU_DEINT_H_

#include <stdint.h>
#include "fvi.h"

void DEINT_Lock(void);

void DEINT_Unlock(void);

void DEINT_Configure(FVIVideoMode videoMode);

void DEINT_Reconfigure(void);

uint16_t DEINT_0dff(FVIVideoMode videoMode, FVIState* state);

uint16_t DEINT_0de4(FVIVideoMode videoMode);

void DEINT_ConfigureCBCRSwap(BOOL* outEnabled);

#endif

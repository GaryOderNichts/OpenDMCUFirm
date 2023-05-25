/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_OVERSCAN_H_
#define _DMCU_OVERSCAN_H_

#include <stdint.h>
#include "dmcu.h"

typedef BOOL (*OVERSCANHandlerFn)(DMCUState* state);

OVERSCANHandlerFn OVERSCAN_GetHandler(void);

#endif

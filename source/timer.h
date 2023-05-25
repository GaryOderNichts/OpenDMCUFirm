/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_TIMER_H_
#define _DMCU_TIMER_H_

#include <stdint.h>

void HandleTimerOverflow(void);

uint32_t GetTimerOverflowCount(void);

void udelay(uint32_t usec);

#endif

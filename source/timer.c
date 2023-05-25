/*
 *   This file is part of OpenDMCUFirm
 */
#include "timer.h"
#include "dc.h"

#include <asm-m68hc11/ports_def.h>

#define TCNT_H (*(volatile uint8_t*) 0x800E)
#define TCNT_L (*(volatile uint8_t*) 0x800F)

static uint32_t timerOverflowCount = 0;

uint32_t GetTCNT(void)
{
    return ((uint32_t) TCNT_H << 0x8) | TCNT_L;
}

void HandleTimerOverflow(void)
{
    // acknowledge timer overflow
    _io_ports[M6811_TFLG2] |= M6811_TOF;

    // DMCU does this explicitly instead of just letting it overflow
    if (timerOverflowCount == 0xffffffff) {
        timerOverflowCount = 0;
    }

    timerOverflowCount++;
}

uint32_t GetTimerOverflowCount(void)
{
    return timerOverflowCount;
}

void tcnt_delay(uint32_t cnt)
{
    uint32_t cur;
    uint32_t start = 0;
    uint32_t elapsed = 0;

    start = GetTCNT();
    do {
        cur = GetTCNT();
        
        // handle overflow
        if (cur > start) {
            elapsed += (cur - start);
        } else {
            elapsed += ((int32_t)cur - start) + 0xFFFF;
        }
    
        start = cur;
    } while (elapsed < cnt);
}

void udelay(uint32_t usec)
{
    tcnt_delay((usec * 9) + (usec / 4) + (usec / 32));
}

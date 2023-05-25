/*
 *   This file is part of OpenDMCUFirm
 */
#include "irq.h"
#include <stdint.h>

static uint32_t irqLockCount = 0;

void IRQ_Lock(void)
{
    __asm__ __volatile__ ("sei");
    irqLockCount++;
}

void IRQ_Unlock(void)
{
    if (--irqLockCount == 0) {
        __asm__ __volatile__ ("cli");
    }
}

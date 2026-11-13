/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_IRQ_H_
#define _DMCU_IRQ_H_

static __inline__ void IRQ_Enable(void)
{
    // Clear "X Interrupt Mask"
    __asm__ __volatile__("tpa\n\tanda #0x2f\n\ttap" : : : "a", "cc", "memory");
}

void IRQ_Lock(void);

void IRQ_Unlock(void);

#endif

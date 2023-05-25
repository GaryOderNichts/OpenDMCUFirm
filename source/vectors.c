/*
 *   This file is part of OpenDMCUFirm
 */
#include <asm-m68hc11/interrupts.h>

#include "irq.h"
#include "dmcu.h"
#include "timer.h"

extern void _start(void);

void __attribute__((interrupt)) unused_handler(void) { }

/* This is unused but a separate handler in the DMCU firmware */
void __attribute__((interrupt)) xirq_handler(void) { }

void __attribute__((interrupt)) irq_handler(void)
{
    IRQ_Lock();
    DMCU_HandleIrq();
    IRQ_Unlock();
}

void __attribute__((interrupt)) timer_overflow_handler(void)
{
    IRQ_Lock();
    HandleTimerOverflow();
    IRQ_Unlock();
}

const struct interrupt_vectors __attribute__((section(".vectors"))) vectors = 
{
    /* Unused */
    res0_handler:           unused_handler, /* res0 */
    res1_handler:           unused_handler,
    res2_handler:           unused_handler,
    res3_handler:           unused_handler,
    res4_handler:           unused_handler,
    res5_handler:           unused_handler,
    res6_handler:           unused_handler,
    res7_handler:           unused_handler,
    res8_handler:           unused_handler,
    res9_handler:           unused_handler,
    res10_handler:          unused_handler, /* res 10 */
    sci_handler:            unused_handler, /* sci */
    spi_handler:            unused_handler, /* spi */
    acc_overflow_handler:   unused_handler, /* acc overflow */
    acc_input_handler:      unused_handler,
    output5_handler:        unused_handler, /* out compare 5 */
    output4_handler:        unused_handler, /* out compare 4 */
    output3_handler:        unused_handler, /* out compare 3 */
    output2_handler:        unused_handler, /* out compare 2 */
    output1_handler:        unused_handler, /* out compare 1 */
    capture3_handler:       unused_handler, /* in capt 3 */
    capture2_handler:       unused_handler, /* in capt 2 */
    capture1_handler:       unused_handler, /* in capt 1 */
    rtii_handler:           unused_handler,
    swi_handler:            unused_handler, /* swi */
    illegal_handler:        unused_handler, /* illegal */
    cop_fail_handler:       unused_handler,
    cop_clock_handler:      unused_handler,

    /* What we need */
    timer_overflow_handler: timer_overflow_handler,
    irq_handler:            irq_handler,    /* IRQ */
    xirq_handler:           xirq_handler,   /* XIRQ */
    reset_handler:          _start,
};

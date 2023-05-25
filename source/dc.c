/*
 *   This file is part of OpenDMCUFirm
 */
#include "dc.h"
#include "irq.h"

#define DC_REG_BASE_PTR     ((volatile uint8_t*) 0x4000)
#define DC_WRITE_CONTROL    (*(volatile uint8_t*) 0x9402)
#define DC_WRITE_DATA_PTR   ((volatile uint8_t*) 0x9404)
#define DC_READ_ADDRESS_PTR ((volatile uint8_t*) 0x9408)
#define DC_READ_CONTROL     (*(volatile uint8_t*) 0x940A)
#define DC_READ_DATA_PTR    ((volatile uint8_t*) 0x940C)

void DC_ImmediateWrite32(uint16_t reg, uint32_t val)
{
    DC_WRITE_DATA_PTR[0] = (val) & 0xFF;
    DC_WRITE_DATA_PTR[1] = (val >> 8) & 0xFF;
    DC_WRITE_DATA_PTR[2] = (val >> 16) & 0xFF;
    DC_WRITE_DATA_PTR[3] = (val >> 24) & 0xFF;
    DC_REG_BASE_PTR[reg * 4] = 0xFF;
}

void DC_Write32(uint16_t reg, uint32_t val)
{
    IRQ_Lock();
    DC_WaitWrite();
    DC_WRITE_DATA_PTR[0] = (val) & 0xFF;
    DC_WRITE_DATA_PTR[1] = (val >> 8) & 0xFF;
    DC_WRITE_DATA_PTR[2] = (val >> 16) & 0xFF;
    DC_WRITE_DATA_PTR[3] = (val >> 24) & 0xFF;
    DC_REG_BASE_PTR[reg * 4] = 0xFF;
    IRQ_Unlock();
}

void DC_DirectWrite32(uint16_t addr, uint32_t val)
{
    IRQ_Lock();
    DC_WaitWrite();
    DC_WRITE_DATA_PTR[0] = (val) & 0xFF;
    DC_WRITE_DATA_PTR[1] = (val >> 8) & 0xFF;
    DC_WRITE_DATA_PTR[2] = (val >> 16) & 0xFF;
    DC_WRITE_DATA_PTR[3] = (val >> 24) & 0xFF;
    *(volatile uint8_t*) addr = 0xFF;
    IRQ_Unlock();
}

void DC_Write8(uint16_t reg, uint16_t off, uint8_t val)
{
    IRQ_Lock();
    DC_WaitWrite();
    DC_REG_BASE_PTR[reg * 4 + off] = val;
    IRQ_Unlock();
}

void DC_DirectWrite8(uint16_t addr, uint8_t val)
{
    IRQ_Lock();
    DC_WaitWrite();
    *(volatile uint8_t*) addr = val;
    IRQ_Unlock();
}

void DC_WaitWrite(void)
{
    uint8_t ctl;
    do {
        IRQ_Lock();
        ctl = DC_WRITE_CONTROL;
        IRQ_Unlock();
    } while (!(ctl & 1));
}

void DC_PerformRead(uint16_t reg)
{
    DC_READ_ADDRESS_PTR[0] = reg & 0xFF;
    DC_READ_ADDRESS_PTR[1] = (reg >> 8) & 0xFF;

    // wait for read to be done
    uint8_t ctl;
    do {
        IRQ_Lock();
        ctl = DC_READ_CONTROL;
        IRQ_Unlock();
    } while (!(ctl & 1));
}

uint8_t DC_Get8(uint16_t off)
{
    return DC_READ_DATA_PTR[off];
}

uint32_t DC_Get32(void)
{
    return ((uint32_t) DC_READ_DATA_PTR[3] << 24) |
           ((uint32_t) DC_READ_DATA_PTR[2] << 16) |
           ((uint32_t) DC_READ_DATA_PTR[1] << 8) |
           ((uint32_t) DC_READ_DATA_PTR[0]);
}

uint32_t DC_Read32(uint16_t reg)
{
    IRQ_Lock();
    DC_PerformRead(reg);
    uint32_t ret = DC_Get32();
    IRQ_Unlock();
    return ret;
}

uint8_t DC_Read8(uint16_t reg, uint16_t off)
{
    IRQ_Lock();
    DC_PerformRead(reg);
    uint8_t ret = DC_READ_DATA_PTR[off];
    IRQ_Unlock();
    return ret;
}

void DC_Set8(uint16_t reg, uint16_t off, uint8_t val)
{
    DC_PerformRead(reg);
    DC_Write8(reg, off, DC_Get8(off) | val);
}

void DC_Clear8(uint16_t reg, uint16_t off, uint8_t val)
{
    DC_PerformRead(reg);
    DC_Write8(reg, off, DC_Get8(off & 0xFF) & ~val);
}

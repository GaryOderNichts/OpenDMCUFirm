/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_CUSTOM_H_
#define _DMCU_CUSTOM_H_

#include <stdint.h>

// Magic and config revision
#define CUSTOM_MAGIC 0xDC00

typedef struct __attribute__((packed)) {
    uint16_t start;
    uint16_t size;
    uint16_t end;
} ViewportAxis;

typedef struct __attribute__((packed)) {
    ViewportAxis x;
    ViewportAxis y;
} Viewport;

typedef struct __attribute__((packed)) {
    uint8_t enabled;
    uint8_t nearest;
} ScalerOptions;

typedef struct __attribute__((packed)) {
    uint16_t magic;
    Viewport tvViewport;
    Viewport tvViewport_576i;
    Viewport drcViewport;
    Viewport drcViewport_576i;
    ScalerOptions tvScaler;
    ScalerOptions drcScaler; 
} Customizations;

extern Customizations _custom;

#endif

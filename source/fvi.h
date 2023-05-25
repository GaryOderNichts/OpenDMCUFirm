/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_FVI_H_
#define _DMCU_FVI_H_

#include "types.h"

typedef struct FVIRegs FVIRegs;
typedef struct FVIState FVIState;

struct FVIRegs {
    uint32_t reg0x1061;
    uint32_t reg0x109B;
    uint32_t reg0x1080;
    uint32_t reg0x1084;
    uint32_t reg0x1083;
    uint32_t reg0x1087;
    uint32_t reg0x1089;
    uint32_t reg0x1092;
    uint32_t reg0x1082;
    uint32_t unknown;
};

struct FVIState {
    uint32_t unk0x00;
    uint16_t unk0x04;
    uint16_t unk0x06;
    uint16_t unk0x08;
    uint16_t unk0x0A;
    uint16_t unk0x0C;
    uint16_t unk0x0E;
    uint32_t unk0x10;
    uint16_t unk0x14;
    uint16_t unk0x16;
    uint16_t unk0x18;
    uint16_t unk0x1A;
    uint32_t videoMode;
};

typedef enum {
    FVI_VIDEO_MODE_INVALID = 0x0,
    //! NTSC 240?
    FVI_VIDEO_MODE_UNK1    = 0x1,
    //! PAL 288?
    FVI_VIDEO_MODE_UNK2    = 0x2,
    //! PAL 288?
    FVI_VIDEO_MODE_UNK3    = 0x3,
    //! PAL 288?
    FVI_VIDEO_MODE_UNK4    = 0x4,
    //! NTSC 240?
    FVI_VIDEO_MODE_UNK5    = 0x5,
    //! NTSC 240?
    FVI_VIDEO_MODE_UNK6    = 0x6,
    //! NTSC 480p?
    FVI_VIDEO_MODE_480P    = 0x7,
    //! PAL 576i?
    FVI_VIDEO_MODE_576I    = 0x8,
} FVIVideoMode;

BOOL FVI_Wait(void);

BOOL FVI_ReadRegs(FVIRegs* regs);

void FVI_SetUpdateRegs(uint16_t update);

FVIRegs* FVI_GetRegs(void);

FVIState* FVI_GetState(void);

BOOL FVI_GetVideoMode(FVIVideoMode* outVideoMode);

#endif

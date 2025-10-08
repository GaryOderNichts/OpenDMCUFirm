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
    //! Equalization pulse in half lines per field
    uint32_t equalization;
    //! Active video lines per field
    uint32_t activeLines;
    //! Preblanking in half lines
    uint32_t preBlanking;
    //! Always 0?
    uint32_t unk0x0C;
    //! Postblanking in half lines
    uint32_t postBlanking;
    //! Unknown, 2, 4, 6, 8 or 24
    uint32_t unk0x14;
    //! 80 when interlaced, otherwise 40
    uint32_t unk0x18;
    //! Half lines per field
    uint32_t halfLines;
};

typedef enum {
    FVI_VIDEO_MODE_INVALID  = 0x0,
    //! NTSC
    FVI_VIDEO_MODE_480I     = 0x1,
    //! PAL
    FVI_VIDEO_MODE_576I     = 0x2,
    //! PAL
    FVI_VIDEO_MODE_288P_312 = 0x3,
    //! PAL
    FVI_VIDEO_MODE_288P_313 = 0x4,
    //! NTSC
    FVI_VIDEO_MODE_240P_262 = 0x5,
    //! NTSC
    FVI_VIDEO_MODE_240P_263 = 0x6,
    //! NTSC
    FVI_VIDEO_MODE_480P     = 0x7,
    //! PAL
    FVI_VIDEO_MODE_576P     = 0x8,
} FVIVideoMode;

BOOL FVI_Wait(void);

BOOL FVI_ReadRegs(FVIRegs* regs);

void FVI_SetUpdateRegs(uint16_t update);

FVIRegs* FVI_GetRegs(void);

FVIState* FVI_GetState(void);

BOOL FVI_GetVideoMode(FVIVideoMode* outVideoMode);

#endif

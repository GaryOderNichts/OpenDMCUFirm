/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_CONF_H_
#define _DMCU_CONF_H_

#include "types.h"

typedef enum {
    //! Used by 720p, 576i, 480p and 480i
    OP_MODE_OTHER   = 0x0,
    OP_MODE_1080I   = 0x1,
    OP_MODE_1080P   = 0x2,
    // Unused?
    OP_MODE_UNKNOWN = 0x3,
} CONFOpMode;

typedef enum {
    //! Set if progressive
    OP_MODE_FLAG_PROGRESSIVE = 0x01,
    //! Set if PAL
    OP_MODE_FLAG_PAL         = 0x02,
    //! Set if HD
    OP_MODE_FLAG_HD          = 0x80,

    // Mask and shift to obtain CONFOpMode
    OP_MODE_MASK  = 0x0C,
    OP_MODE_SHIFT = 2,
} CONFOpModeFlags;

uint8_t CONF_GetTVOpMode(void);

uint8_t CONF_GetDRCOpMode(void);

uint16_t CONF_CheckOpMode(DisplayType disp, uint8_t mask);

uint8_t CONF_GetOpMode(DisplayType disp);

uint16_t CONF_IsHD(DisplayType disp);

uint16_t CONF_IsSD(DisplayType disp);

uint16_t CONF_IsPAL(DisplayType disp);

#endif

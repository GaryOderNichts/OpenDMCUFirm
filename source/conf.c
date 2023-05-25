/*
 *   This file is part of OpenDMCUFirm
 */
#include "conf.h"
#include "debug.h"

extern uint8_t _dc_op_modes[2];

uint16_t gUpdateTVOpMode = 1;
uint8_t gCurrentTVOpMode;
uint16_t gUpdateDRCOpMode = 1;
uint8_t gCurrentDRCOpMode;

uint8_t CONF_GetTVOpMode(void)
{
    if (gUpdateTVOpMode) {
        gCurrentTVOpMode = _dc_op_modes[0];
        gUpdateTVOpMode = 0;
    }

    return gCurrentTVOpMode;
}

uint8_t CONF_GetDRCOpMode(void)
{
    if (gUpdateDRCOpMode) {
        // This is not a typo, DRC and TV use the same op mode for some reason
        gCurrentDRCOpMode = _dc_op_modes[0];
        gUpdateDRCOpMode = 0;
    }

    return gCurrentDRCOpMode;
}

uint16_t CONF_CheckOpMode(DisplayType disp, uint8_t mask)
{
    if (disp == DISPLAY_TV) {
        return CONF_GetTVOpMode() & mask;
    } else if (disp == DISPLAY_DRC) {
        return CONF_GetDRCOpMode() & mask;
    } else {
        DEBUG_SetLowMarker(0x200);
        return 0;
    }
}

uint8_t CONF_GetOpMode(DisplayType disp)
{
    if (disp == DISPLAY_TV) {
        return CONF_GetTVOpMode();
    } else if (disp == DISPLAY_DRC) {
        return CONF_GetDRCOpMode();
    } else {
        DEBUG_SetLowMarker(0x200);
        return 0;
    }
}

uint16_t CONF_IsHD(DisplayType disp)
{
    return CONF_CheckOpMode(disp, OP_MODE_FLAG_HD);
}

uint16_t CONF_IsSD(DisplayType disp)
{
    return CONF_CheckOpMode(disp, OP_MODE_FLAG_HD) == 0;
}

uint16_t CONF_IsPAL(DisplayType disp)
{
    return CONF_CheckOpMode(disp, OP_MODE_FLAG_PAL);
}

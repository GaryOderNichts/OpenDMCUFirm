#include "custom.h"

#ifdef CUSTOM

// Default customization options, can be overriden by loader
Customizations __attribute__((section(".custom"))) _custom = {
    .magic = CUSTOM_MAGIC,
    .tvViewport = {
        .x = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
        .y = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
    },
    .tvViewport_576i = {
        .x = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
        .y = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
    },
    .drcViewport = {
        .x = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
        .y = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
    },
    .drcViewport_576i = {
        .x = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
        .y = { .start = 0xffff, .size = 0xffff, .end = 0xffff, },
    },
    .tvScaler = {
        .enabled = 0xff, .nearest = 0xff,
    },
    .drcScaler = {
        .enabled = 0xff, .nearest = 0xff,
    }
};

#endif

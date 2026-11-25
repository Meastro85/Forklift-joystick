#pragma once

#include <stdint.h>

namespace joystick {
    struct InputState {
        int16_t axes[4] = {};

        uint8_t buttons = 0;

        uint8_t hat = 8;

        uint32_t timestamp = 0;
    };
}
#pragma once

#include <Arduino.h>

namespace joystick {
    constexpr uint8_t AXIS_COUNT = 2;
    constexpr uint8_t BUTTON_COUNT = 7;

    constexpr uint8_t AXIS_PINS[AXIS_COUNT] = {A0, A1};

    constexpr uint8_t BUTTON_PINS[BUTTON_COUNT] = {2, 3, 4, 5, 6, 7, 8};
}
#include <Arduino.h>
#include "config/board_config.hpp"

namespace joystick {
    enum class Axis : uint8_t {
        X = 0,
        Y,
        Count
    };

    constexpr uint16_t ADC_MAX = 1023;

    struct AxisCalibration {
        uint16_t min = 0;
        uint16_t center = ADC_MAX / 2;
        uint16_t max = ADC_MAX;
        bool inverted = false;
    };
}
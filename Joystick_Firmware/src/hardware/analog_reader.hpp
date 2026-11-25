#pragma once

#include "input/axis.hpp"

namespace joystick {

    class AnalogReader {
    public:
        explicit AnalogReader(
            const uint8_t (&pins)[AXIS_COUNT],
            uint8_t samples = 8
        );

        void begin();

        uint16_t read(Axis axis) const;
        void readAll(uint16_t (&values)[AXIS_COUNT]) const;

    private:
        uint16_t sample(uint8_t pin) const;

        uint8_t pins_[AXIS_COUNT];
        uint8_t samples_;
    };

}
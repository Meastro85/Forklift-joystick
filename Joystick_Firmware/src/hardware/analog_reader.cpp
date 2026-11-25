#include "analog_reader.hpp"

namespace joystick {

    AnalogReader::AnalogReader(
        const uint8_t (&pins)[AXIS_COUNT],
        uint8_t samples
    )
        : samples_(samples ? samples : 1) {

        for (size_t i = 0; i < AXIS_COUNT; ++i) {
            pins_[i] = pins[i];
        }
    }

    void AnalogReader::begin() {
        for (size_t i = 0; i < AXIS_COUNT; ++i) {
            pinMode(pins_[i], INPUT);
        }
    }

    uint16_t AnalogReader::sample(uint8_t pin) const {
        analogRead(pin);

        uint32_t sum = 0;

        for (uint8_t i = 0; i < samples_; ++i) {
            sum += analogRead(pin);
        }

        return static_cast<uint16_t>(sum / samples_);
    }

    uint16_t AnalogReader::read(Axis axis) const {
        const size_t i = static_cast<size_t>(axis);

        if (i >= AXIS_COUNT) {
            return 0;
        }

        return sample(pins_[i]);
    }

    void AnalogReader::readAll(
        uint16_t (&values)[AXIS_COUNT]
    ) const {
        for (size_t i = 0; i < AXIS_COUNT; ++i) {
            values[i] = sample(pins_[i]);
        }
    }

}
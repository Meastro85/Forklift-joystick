#pragma once
#include <Arduino.h>

namespace Joystick {
    enum class Axis : uint8_t {
        X, Y, Count
    };

    class Axes {
        public:
            static constexpr size_t COUNT = static_cast<size_t>(Axis::Count);

            // deazone per mille (50 = 5% of full range)
            Axes(const uint8_t (&pins)[COUNT], uint8_t samples = 8, int16_t deadzone = 50);

            void begin();
            void update();

            int16_t value(Axis axis) const;
            uint16_t raw(Axis axis) const;

            void setLearning(bool enabled); // enable/disable learning mode (for calibration)
            void setCalibration(Axis axis, uint16_t min, uint16_t center, uint16_t max); // set calibration values for an axis
            void invert(Axis axis, bool inverted); // invert the axis (for calibration)
            void calibrate(uint32_t durationMs = 6000);   // sweep the handle to all stops meanwhile
            uint16_t minRaw(Axis axis) const;
            uint16_t centerRaw(Axis axis) const;
            uint16_t maxRaw(Axis axis) const;

            private:
                uint16_t sample(uint8_t pin) const;

                uint8_t pins_[COUNT];
                uint8_t samples_;
                int16_t deadzone_;
                bool learning_ = true;
                bool inverted_[COUNT] = {};
                uint16_t raw_[COUNT] = {};
                uint16_t min_[COUNT] = {};
                uint16_t center_[COUNT] = {};
                uint16_t max_[COUNT] = {};
                uint16_t value_[COUNT] = {};
    };

}
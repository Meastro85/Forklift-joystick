#pragma once

#include "axis.hpp"

namespace joystick {

    class AxisProcessor {
        public:
            explicit AxisProcessor(int16_t deadzone = 50);

            void begin(const uint16_t (&initialRaw)[AXIS_COUNT]);

            void update(const uint16_t (&raw)[AXIS_COUNT], uint32_t nowMs);

            void startCalibration(const uint16_t (&calibrationData)[AXIS_COUNT], uint32_t durationMs = 6000);

            bool isCalibrating() const;

            int16_t value(Axis axis) const;
            uint16_t raw(Axis axis) const;

            const AxisCalibration& calibration(Axis axis) const;

            void setCalibration(Axis axis, const AxisCalibration& calibration);

            void setDeadzone(int16_t deadzone);

            void invert(Axis axis, bool inverted);

        private:
            int16_t normalize(size_t index, uint16_t raw) const;
            void updateCalibration(size_t index, uint16_t raw);
            void finishCalibration();

            AxisCalibration calibration_[AXIS_COUNT];

            uint16_t raw_[AXIS_COUNT] = {};
            int16_t value_[AXIS_COUNT] = {};

            int16_t deadzone_;

            bool calibrating_ = false;
            uint32_t calibrationStartMs_ = 0;
            uint32_t calibrationDurationMs_ = 0;
    };

}
#include "axis_processor.hpp"

namespace joystick {
    
    static constexpr uint16_t INITIAL_SPAN = 200;
    static constexpr int32_t OUTPUT_MAX = 1000;
    static constexpr uint16_t MIN_SPAN = 20;

    AxisProcessor::AxisProcessor(int16_t deadzone) : deadzone_(constrain(deadzone, 0, 999)){}

    void AxisProcessor::begin(const uint16_t (&initialRaw)[AXIS_COUNT]) {
        for (size_t i = 0; i < AXIS_COUNT; ++i) {
            const uint16_t center =
                constrain(initialRaw[i], 0, ADC_MAX);

            raw_[i] = center;
            value_[i] = 0;

            calibration_[i].center = center;

            calibration_[i].min =
                center > INITIAL_SPAN
                    ? center - INITIAL_SPAN
                    : 0;

            calibration_[i].max =
                center + INITIAL_SPAN > ADC_MAX
                    ? ADC_MAX
                    : center + INITIAL_SPAN;

            calibration_[i].inverted = false;
        }
    }

    void AxisProcessor::update(const uint16_t (&rawValues)[AXIS_COUNT], uint32_t nowMs) {
        if (calibrating_) {
            for (size_t i = 0; i < AXIS_COUNT; ++i) {
                updateCalibration(i, rawValues[i]);
            }

            if (nowMs - calibrationStartMs_ >=
                calibrationDurationMs_) {
                finishCalibration();
            }
        }

        for (size_t i = 0; i < AXIS_COUNT; ++i) {
            raw_[i] = constrain(rawValues[i], 0, ADC_MAX);
            value_[i] = normalize(i, raw_[i]);
        }
    }

    void AxisProcessor::startCalibration(const uint16_t (&currentRaw)[AXIS_COUNT], uint32_t durationMs) {
        for (size_t i = 0; i < AXIS_COUNT; ++i) {
            const uint16_t current =
                constrain(currentRaw[i], 0, ADC_MAX);

            calibration_[i].min = current;
            calibration_[i].max = current;
        }

        calibrationStartMs_ = millis();
        calibrationDurationMs_ = durationMs;
        calibrating_ = true;
    }

    bool AxisProcessor::isCalibrating() const {
        return calibrating_;
    }

    void AxisProcessor::updateCalibration(size_t index, uint16_t rawValue) {
        rawValue = constrain(rawValue, 0, ADC_MAX);

        if (rawValue < calibration_[index].min) {
            calibration_[index].min = rawValue;
        }

        if (rawValue > calibration_[index].max) {
            calibration_[index].max = rawValue;
        }
    }

    void AxisProcessor::finishCalibration() {
        calibrating_ = false;
    }

    int16_t AxisProcessor::normalize(size_t index, uint16_t rawValue) const {
        const AxisCalibration& c = calibration_[index];

        const int32_t delta =
            static_cast<int32_t>(rawValue) - c.center;

        const int32_t span = delta >= 0
            ? static_cast<int32_t>(c.max) - c.center
            : static_cast<int32_t>(c.center) - c.min;

        const int32_t safeSpan =
            span < MIN_SPAN ? MIN_SPAN : span;

        int32_t result =
            delta * OUTPUT_MAX / safeSpan;

        result = constrain(
            result,
            -OUTPUT_MAX,
            OUTPUT_MAX
        );

        const int32_t magnitude = abs(result);

        if (magnitude <= deadzone_) {
            result = 0;
        } else {
            const int32_t scaled =
                (magnitude - deadzone_) * OUTPUT_MAX /
                (OUTPUT_MAX - deadzone_);

            result = result < 0 ? -scaled : scaled;
        }

        if (c.inverted) {
            result = -result;
        }

        return static_cast<int16_t>(result);
    }

    int16_t AxisProcessor::value(Axis axis) const {
        const size_t i = static_cast<size_t>(axis);

        if (i >= AXIS_COUNT) {
            return 0;
        }

        return value_[i];
    }

    uint16_t AxisProcessor::raw(Axis axis) const {
        const size_t i = static_cast<size_t>(axis);

        if (i >= AXIS_COUNT) {
            return 0;
        }

        return raw_[i];
    }

    const AxisCalibration& AxisProcessor::calibration(Axis axis) const {
        return calibration_[static_cast<size_t>(axis)];
    }

    void AxisProcessor::setCalibration(Axis axis, const AxisCalibration& calibration) {
        const size_t i = static_cast<size_t>(axis);

        if (i >= AXIS_COUNT) {
            return;
        }

        AxisCalibration c = calibration;

        c.min = constrain(c.min, 0, ADC_MAX);
        c.max = constrain(c.max, 0, ADC_MAX);
        c.center = constrain(c.center, 0, ADC_MAX);

        if (c.min > c.max) {
            const uint16_t temp = c.min;
            c.min = c.max;
            c.max = temp;
        }

        c.center = constrain(c.center, c.min, c.max);

        calibration_[i] = c;
    }

    void AxisProcessor::setDeadzone(int16_t deadzone) {
        deadzone_ = constrain(deadzone, 0, 999);
    }

    void AxisProcessor::invert(Axis axis, bool inverted) {
        const size_t i = static_cast<size_t>(axis);

        if (i < AXIS_COUNT) {
            calibration_[i].inverted = inverted;
        }
    }

}
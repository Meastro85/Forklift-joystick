#include "Axes.h"

namespace Joystick {

    static constexpr int16_t INITIAL_SPAN = 200;

    Axes::Axes(const uint8_t (&pins)[COUNT], uint8_t samples, int16_t deadzone)
        : samples_(samples ? samples : 8), deadzone_(deadzone) {
        for (size_t i = 0; i < COUNT; ++i) pins_[i] = pins[i];
    }

    uint16_t Axes::sample(uint8_t pin) const {
        analogRead(pin);
        uint32_t sum = 0;
        for (uint8_t i = 0; i < samples_; ++i) {
            sum += analogRead(pin);
        }
        return static_cast<uint16_t>(sum / samples_);
    }

    void Axes::begin() {
        for (size_t i = 0; i < COUNT; ++i) {
            pinMode(pins_[i], INPUT);
            const uint16_t raw = sample(pins_[i]);
            raw_[i] = center_[i] = raw;
            min_[i] = raw > INITIAL_SPAN ? raw - INITIAL_SPAN : 0;
            max_[i] = raw + INITIAL_SPAN > 1023 ? 1023 : raw + INITIAL_SPAN;
        }
    }

    void Axes::update(){
        for(size_t i = 0; i < COUNT; ++i) {
            const uint16_t raw = sample(pins_[i]);
            raw_[i] = raw;

            if(learning_){
                if(raw < min_[i]) min_[i] = raw;
                if(raw > max_[i]) max_[i] = raw;
            }

            const int32_t d = static_cast<int32_t>(raw) - center_[i];
            int32_t span = d >= 0 ? static_cast<int32_t>(max_[i]) - center_[i]
                                    : static_cast<int32_t>(center_[i]) - min_[i];

            if (span < 20) span = 20; 

            int32_t v = constrain(d * 1000L / span, -1000L, 1000L);

            const int32_t a = abs(v);
            if(a <= deadzone_) v = 0;
            else {
                const int32_t scaled = (a - deadzone_) * 1000L / (1000L - deadzone_);
                v = (v < 0 ? -scaled : scaled);
            };

            value_[i] = static_cast<int16_t>(inverted_[i] ? -v : v);
        }
    }

    void Axes::calibrate(uint32_t durationMs) {
    for (size_t i = 0; i < COUNT; ++i) {
        min_[i] = max_[i] = center_[i];            // start from zero range
    }
    const uint32_t start = millis();
    while (millis() - start < durationMs) {
        for (size_t i = 0; i < COUNT; ++i) {
        const uint16_t r = sample(pins_[i]);
        if (r < min_[i]) min_[i] = r;
        if (r > max_[i]) max_[i] = r;
        }
    }
    learning_ = false;                           // lock the result
    }

    uint16_t Axes::minRaw(Axis axis) const    { return min_[static_cast<size_t>(axis)]; }
    uint16_t Axes::centerRaw(Axis axis) const { return center_[static_cast<size_t>(axis)]; }
    uint16_t Axes::maxRaw(Axis axis) const    { return max_[static_cast<size_t>(axis)]; }

    int16_t Axes::value(Axis axis) const {
        return value_[static_cast<size_t>(axis)];
    }

    uint16_t Axes::raw(Axis axis) const {
        return raw_[static_cast<size_t>(axis)];
    }

    void Axes::setLearning(bool enabled) {
        learning_ = enabled;
    }

    void Axes::setCalibration(Axis axis, uint16_t min, uint16_t center, uint16_t max) {
        const size_t i = static_cast<size_t>(axis);
        min_[i] = min;
        center_[i] = center;
        max_[i] = max;
    }

    void Axes::invert(Axis axis, bool inverted) {
        inverted_[static_cast<size_t>(axis)] = inverted;
    }

}
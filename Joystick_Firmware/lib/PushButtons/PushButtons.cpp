#include "PushButtons.h"

namespace Joystick {

static const char* const NAMES[] = {
  "Push 3A", "Push 3B", "Push 4A", "Push 4B", "Push 5", "Push 6", "Push 7"
};

PushButtons::PushButtons(const uint8_t (&pins)[COUNT], uint16_t debounceMs)
    : debounceMs_(debounceMs) {
  for (size_t i = 0; i < COUNT; ++i) pins_[i] = pins[i];
}

void PushButtons::begin() {
  for (size_t i = 0; i < COUNT; ++i) {
    pinMode(pins_[i], INPUT_PULLUP);
    raw_[i] = stable_[i] = (digitalRead(pins_[i]) == LOW);
    changedAt_[i] = millis();
  }
}

void PushButtons::onChange(Callback callback) { callback_ = callback; }

void PushButtons::update() {
  const uint32_t now = millis();
  for (size_t i = 0; i < COUNT; ++i) {
    const bool raw = (digitalRead(pins_[i]) == LOW);   // LOW = pressed
    if (raw != raw_[i]) {
      raw_[i] = raw;
      changedAt_[i] = now;                             // restart debounce timer
    } else if (raw != stable_[i] && (now - changedAt_[i]) >= debounceMs_) {
      stable_[i] = raw;
      if (callback_) callback_(static_cast<Button>(i), raw);
    }
  }
}

bool PushButtons::isPressed(Button button) const {
  return stable_[static_cast<size_t>(button)];
}

const char* PushButtons::name(Button button) {
  return NAMES[static_cast<size_t>(button)];
}

}  // namespace Joystick
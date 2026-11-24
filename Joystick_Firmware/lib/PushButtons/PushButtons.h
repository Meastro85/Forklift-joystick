#pragma once
#include <Arduino.h>

namespace Joystick {

enum class Button : uint8_t {
  Push3A, Push3B, Push4A, Push4B, Push5, Push6, Push7, Count
};

class PushButtons {
public:
  static constexpr size_t COUNT = static_cast<size_t>(Button::Count);
  using Callback = void (*)(Button button, bool pressed);

  PushButtons(const uint8_t (&pins)[COUNT], uint16_t debounceMs = 10);

  void begin();                         // configures pins as INPUT_PULLUP
  void onChange(Callback callback);     // called on each debounced press/release
  void update();                        // call every loop(), never blocks

  bool isPressed(Button button) const;
  static const char* name(Button button);

private:
  uint8_t pins_[COUNT];
  uint16_t debounceMs_;
  bool raw_[COUNT] = {};
  bool stable_[COUNT] = {};
  uint32_t changedAt_[COUNT] = {};
  Callback callback_ = nullptr;
};

}  // namespace Joystick
#include <Arduino.h>
#include <PushButtons.h>
#include <Axes.h>

using namespace Joystick;

// Order must match the Button enum: 3A, 3B, 4A, 4B, 5, 6, 7
const uint8_t BUTTON_PINS[PushButtons::COUNT] = {2, 3, 4, 5, 6, 7, 8};
const uint8_t AXIS_PINS[Axes::COUNT] = {A0, A1};
PushButtons buttons(BUTTON_PINS);
Axes axes(AXIS_PINS);

void onButton(Button button, bool pressed) {
  Serial.print(PushButtons::name(button));
  Serial.println(pressed ? " pressed" : " released");
}

void setup() {
  Serial.begin(115200);
  buttons.begin();
  buttons.onChange(onButton);
  delay(200);
  axes.begin();

  Serial.println("Calibrating: move the handle to all four stops...");
  axes.calibrate(6000);

  for (uint8_t a = 0; a < Axes::COUNT; ++a) {
    Axis ax = static_cast<Axis>(a);
    Serial.print(a == 0 ? "X" : "Y");
    Serial.print(" min=");    Serial.print(axes.minRaw(ax));
    Serial.print(" center="); Serial.print(axes.centerRaw(ax));
    Serial.print(" max=");    Serial.println(axes.maxRaw(ax));
  }
}

void loop() {
  buttons.update();
  axes.update();

  static uint32_t lastPrint = 0;
  if (millis() - lastPrint >= 100) {
    lastPrint = millis();
    Serial.print("X: ");
    Serial.print(axes.value(Axis::X));
    Serial.print(", Y: ");
    Serial.println(axes.value(Axis::Y));
  }
}
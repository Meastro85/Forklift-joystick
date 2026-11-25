#pragma once

#include "input/input_state.hpp"

namespace joystick {
    class SerialTransport {
        public:
            void begin();

            void sendInputState(const InputState& state);

            void update();
    };
}
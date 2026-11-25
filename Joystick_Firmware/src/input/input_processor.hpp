#pragma once

#include "input_state.hpp"

namespace joystick {
    class InputProcessor {
        public:
            void begin();

            void update();

            const InputState& getState() const;

        private:
            InputState state;

            void updateAxes();
            void updateButtons();
    };
}
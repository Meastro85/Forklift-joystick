#include "application.hpp"
#include "hardware/analog_reader.hpp"
#include "input/axis_processor.hpp"
#include "config/board_config.hpp"
#include <Arduino.h>

namespace joystick {

    AnalogReader analogReader(AXIS_PINS, 8);
    AxisProcessor axisProcessor(50);

    uint16_t rawAxes[AXIS_COUNT];

    void beginAxes() {
        analogReader.begin();

        analogReader.readAll(rawAxes);
        axisProcessor.begin(rawAxes);
    }

    void updateAxes() {
        analogReader.readAll(rawAxes);
        axisProcessor.update(rawAxes, millis());
    }
    
    void Application::begin() {
        beginAxes();
    }

    void Application::update() {
        updateAxes();
    }

}
#ifndef INPUTS_H
#define INPUTS_H

#include <stdint.h>
#include "common/type.h"

// All the inputs that can be used on the microcontroller, according to our specifications.
typedef enum Input_type{
    SWITCH_MCU,                 // Switch mounted on the microcontroller PIN1.3
    SWITCH_MICRO_1,             // External switch n�1 connected to PINX.X
    SWITCH_MICRO_2,             // External switch n�2 connected to PINX.X
    POTENTIOMETER               // Potentiometer connected to PINX.X
}Input_type;

// All the event types that can be done, using the type of inputs available.
typedef enum Input_event{
    PRESS,                      // The switch has been pressed
    RELEASE,                    // The switch has been released. The duration of press should be transmitted.
    POTENTIOMETER_UPDATE        // Reading of the potentiometer value.
}Input_event;


// function that creates a message to be sent the server, specifying the input action that occured.
void send_event(Input_type type, Input_event event, Intensity intensity, Duration duration_ms);

#endif //INPUTS_H

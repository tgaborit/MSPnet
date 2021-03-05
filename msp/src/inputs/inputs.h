#ifndef INPUTS_H
#define INPUTS_H

#include <stdint.h>

// All the inputs that can be used on the microcontroller, according to our specifications.
typedef enum in_type{
    SWITCH_MCU,                 // Switch mounted on the microcontroller PIN1.3
    SWITCH_MICRO_1,             // External switch n°1 connected to PINX.X
    SWITCH_MICRO_2,             // External switch n°2 connected to PINX.X
    POTENTIOMETER               // Potentiometer connected to PINX.X
}in_type;

// All the event types that can be done, using the type of inputs available.
typedef enum in_event{
    PRESS,                      // The switch has been pressed
    RELEASE,                    // The switch has been released. The duration of press should be transmitted.
    POTENTIOMETER_UPDATE        // Reading of the potentiometer value.
}in_event;


// function that creates a message to be sent the server, specifying the input action that occured.
void send_event(in_type type, in_event event, uint16_t intensity, uint16_t duration_ms);




#endif //INPUTS_H

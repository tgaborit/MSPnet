#ifndef INPUTS_H
#define INPUTS_H

#include <stdint.h>
#include "io430.h"
#include "../common/type.h"
#include "../comm/comm.h"

#define DEVICE_ID 123

// All the inputs that can be used on the microcontroller, according to our specifications.
typedef enum Input_type{
    SWITCH_MCU = 0,                 // Switch mounted on the microcontroller PIN1.3
    SWITCH_MICRO_1 = 1,             // External switch n�1 connected to PINX.X
    SWITCH_MICRO_2 = 2,             // External switch n�2 connected to PINX.X
    POTENTIOMETER = 3               // Potentiometer connected to PINX.X
}Input_type;

// All the event types that can be done, using the type of inputs available.
typedef enum Input_event{
    PRESS = 0,                      // The switch has been pressed
    RELEASE = 1,                    // The switch has been released. The duration of press should be transmitted.
    POTENTIOMETER_UPDATE = 2        // Reading of the potentiometer value.
}Input_event;

// Input message, sent by low-level input manager to MQTT message composer
typedef struct Input_message{
     Id           id;           // device id  
     Input_type   type;         // input type
     Input_event  event;        // input event
     Intensity    intensity;    // input intensity
     Duration     duration;     // input duration
}Input_message;

// function that creates a message to be sent the server, specifying the input action that occured.
void send_event(Input_type type, Input_event event, Intensity intensity, Duration duration_ms);

// function to modify variables and switch registers after a debounce operation. It returns 1 if a press/release occured
int debounce(int pressed, int port, int pin);

#endif //INPUTS_H

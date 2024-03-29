#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <stdint.h>
#include "../common/type.h"
#include "../setup/board_setup.h"

// All the output components which can be triggered on the microcontroller, according to our specifications.
typedef enum Output_type{
    LED_D1 = 0,                     // LED D1 mounted on PIN1.0 of MCU
    LED_D2 = 1,                     // LED D2 mounted on PIN1.6 of MCU
    LED_D3 = 2,                     // LED D3 including all its color components, connected to PIN2.3, PIN2.1, PIN2.5.
    LED_D3_GREEN = 3,               // LED D3 as a green LED
    LED_D3_RED = 4,                 // LED D3 as a red LED
    LED_D3_BLUE = 5,                // LED D3 as a blue LED
    LED_D3_PURPLE = 6,              // LED D3 as a purple LED
    BUZZER = 7                      // Buzzer connected to PINX.X
}Output_type;

// All the triggers possible on the output components.
typedef enum Output_trigger{
    ON = 0,                  // The output component is on.
    OFF = 1                  // The output component is off.
}Output_trigger;

// Output message, sent by MQTT parser to low-level output manager
typedef struct Output_message{
     Output_type    type;         // output type
     Output_trigger trigger;      // output trigger
     Intensity      intensity;    // output intensity
     Duration       duration;     // output duration
} Output_message;


// function that analyses a server message and calls other functions to modify the microc outputs consequently.
void output(Output_type type, Output_trigger trigger, Intensity intensity, Duration duration);

// function that updates the outputs state (toggling leds and buzzer, checking if duration is not overlapped ...)
void update_outputs(int mode);

#endif //OUTPUTS_H

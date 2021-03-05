#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <stdint.h>


// All the output components which can be triggered on the microcontroller, according to our specifications.
typedef enum out_type{
    LED_D1,                     // LED D1 mounted on PIN1.0 of MCU
    LED_D2,                     // LED D2 mounted on PIN1.6 of MCU
    LED_D3,                     // LED D3 including all its color components, connected to PIN2.3, PIN2.1, PIN2.5.
    LED_D3_GREEN,               // LED D3 as a green LED
    LED_D3_RED,                 // LED D3 as a red LED
    LED_D3_BLUE,                // LED D3 as a blue LED
    LED_D3_PURPLE,              // LED D3 as a purple LED
    BUZZER                      // Buzzer connected to PINX.X
}out_type;

// All the triggers possible on the output components.
typedef enum output_trigger{
    ON,                  // The output component is on.
    OFF                  // The output component is off.
}out_trigger;

// function that analyses a server message and calls other functions to modify the microc outputs consequently.
void output(out_type type, out_trigger trigger, uint16_t intensity, uint16_t duration);

#endif //OUTPUTS_H

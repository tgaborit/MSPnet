#ifndef BOARD_SETUP_H
#define BOARD_SETUP_H
#include "io430.h"

// uncomment if a switch is used on P1.3
#define SWITCH_0

// uncomment if an external switch is used, and fill the port and pin
#define EXT_SWITCH_1
#ifdef EXT_SWITCH_1
  #define EXT_PORT_1 0          // port (set to 1 or 2)
  #define EXT_PIN_1 0x00        // pin (mask of the pin position between pin 0 and 7)
#endif

// uncomment if another external switch is used, and fill the port and pin
#define EXT_SWITCH_2
#ifdef EXT_SWITCH_2
  #define EXT_PORT_2 1          // port (set to 1 or 2)
  #define EXT_PIN_2 0x00        // pin (mask of the pin position between pin 0 and 7)
#endif
  
// uncomment if a potentiometer is used, and fill the pin (port is automatically 1)
#define POTENTIO
#ifdef POTENTIO
  #define POTENTIO_PIN 1        // pin (pin position between 0 and 7)
#endif
  
//uncomment if a buzzer is used, and fill the port and pin
#define BUZZ
#ifdef BUZZ
  #define BUZZ_PORT 1           // port (set to 1 or 2)
  #define BUZZ_PIN 0x00         // pin (mask of the pin position between pin 0 and 7)
#endif

// function to setup the board leds
void led_setup();

// function to setup the board timers
void timer_setup();

// function to setup the board for buzzer
void buzzer_setup();

// function to setup the board forpotentiometer
void potentio_setup();

// function to setup the board for switchs
void switch_setup(int port, int pin);

// function to setup all the peripherals and devices
void full_setup();

#endif
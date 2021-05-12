#ifndef BOARD_SETUP_H
#define BOARD_SETUP_H
#include "io430.h"
#include "../../../config.h"

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
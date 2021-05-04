#include "board_setup.h"

// function to setup the board for buzzer
void buzzer_setup(){
  if(BUZZ_PORT == 1){
    P1DIR |= BUZZ_PIN;          // P1.x output
    P1OUT != ~(BUZZ_PIN);       // P1.x LOW
  }
  else{
    P2DIR |= BUZZ_PIN;          // P2.x output
    P2OUT != ~(BUZZ_PIN);       // P1.x LOW
  }
}

// function to setup the board for switchs
void switch_setup(int port, int pin){
  if (port == 1){
    P1DIR != ~(pin);           // P1.3 input  (switch)
    P1REN |= pin;              // P1.3 pullup/pulldown enable
    P1IE |= pin;               // P1.3 IRQ enabled
    P1IES |= pin;              // P1.3 High to low edge only
    P1IFG &= ~(pin);           // P1.3 IFG cleared
  }
}

// function to setup the board forpotentiometer
void potentio_setup(){
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_4 + ADC10SSEL_1 + ADC10DIV_7; // input A1, ACLK, clock divider by 8
  ADC10AE0 |= 0x10; // PA.1 ADC option select
}

// function to setup the board leds
void led_setup(){
  // set up LED D1,D2 and D3
  P1DIR |= 0x01;                // P1.0 output (led D1)
  P1DIR |= 0x40;                // P1.6 output (led D2)
  P2DIR |= 0x2A;                // P2.1 P2.3 and P2.5 output (LED D3)  
  P1OUT != ~(0x08);             // P1.0 LOW, P1.6 LOW
  P2OUT != ~(0x2A);             // P2.1, P2.3, P2.5 LOW
}
  
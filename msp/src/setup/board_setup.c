#include "board_setup.h"

// function to setup the board for buzzer
void buzzer_setup(){
  if(BUZZ_PORT == 1){
    P1DIR |= BUZZ_PIN;          // P1.x output
    P1OUT &= ~(BUZZ_PIN);       // P1.x LOW
  }
  else{
    P2DIR |= BUZZ_PIN;          // P2.x output
    P2OUT &= ~(BUZZ_PIN);       // P2.x LOW
  }
}

// function to setup the board for switchs
void switch_setup(int port, int pin){
  if (port == 1){
    P1DIR &= ~(pin);           // P1.x input  (switch)
    P1REN |= pin;              // P1.x pullup/pulldown enable
    P1IE |= pin;               // P1.x IRQ enabled
    P1IES |= pin;              // P1.x High to low edge only
    P1IFG &= ~(pin);           // P1.x IFG cleared
  }
  else{
    P2DIR &= ~(pin);           // P2.x input  (switch)
    P2REN |= pin;              // P2.x pullup/pulldown enable
    P2IE |= pin;               // P2.x IRQ enabled
    P2IES |= pin;              // P2.x High to low edge only
    P2IFG &= ~(pin);           // P2.x IFG cleared
  }
}

// function to setup the board for potentiometer
void potentio_setup(){
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = POTENTIO_PIN*(0x1000u) + ADC10SSEL_1 + ADC10DIV_7; // input Ax, ACLK, clock divider by 8
  ADC10AE0 |= (0x01<<POTENTIO_PIN); // PA.x ADC option select
}

// function to setup the board leds
void led_setup(){
  // set up LED D1,D2 and D3
  P1DIR |= 0x01;                // P1.0 output (led D1)
  P1DIR |= 0x40;                // P1.6 output (led D2)
  P2DIR |= 0x2A;                // P2.1 P2.3 and P2.5 output (LED D3)  
  P1OUT &= ~(0x41);             // P1.0 LOW, P1.6 LOW
  P2OUT &= ~(0x2A);             // P2.1, P2.3, P2.5 LOW
}
  
// function to setup the board timers
void timer_setup(){
  TA1CCR0 = 655-1;              // timer A1 CCR0 value (20 ms before interrupt)
  TA1CCTL0 = CCIE;              // timer A1 CCR0 interrupt enabled (compare mode)
  
  TA0CCTL0 = CCIE;              // timer A0 CCR0 interrupt enabled (compare mode)
  TA0CCR0 = 32-1;               // timer A0 CCR0 value (approx. 1ms before interrupt)
  TA0CTL = TASSEL_1 + MC_1;     // ACLK, up mode
}

// function to setup all the peripherals and devices
void full_setup(){

  // setup buzzer if defined in the board setup header
  #ifdef BUZZ
    buzzer_setup();
  #endif
  
  // setup switch if defined in the board setup header
  #ifdef SWITCH_0
    switch_setup(1,0x08);
  #endif
  
  // setup external switch 1 if defined in the board setup header
  #ifdef EXT_SWITCH_1
    switch_setup(EXT_PORT_1, EXT_PIN_1);
  #endif
  
  // setup external switch 2 if defined in the board setup header
  #ifdef EXT_SWITCH_2
    switch_setup(EXT_PORT_2, EXT_PIN_2);
  #endif
  
  // setup potentiometer if defined in the board setup header  
  #ifdef POTENTIO
    potentio_setup();
  #endif
  
  
  // setup board leds D1, D2 and D3
  led_setup();
  
  // setup board timer 0 A0 and timer 1 A0
  timer_setup();
  
  // enable the interrupts
  __enable_interrupt();
}
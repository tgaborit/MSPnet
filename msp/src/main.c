
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"
#include "setup/board_setup.h"


#ifdef POTENTIO
  unsigned short adc_value = 0x0000;      // last registered adc value from potentiometer
#endif

#ifdef SWITCH_0
  int switch_pressed = 0;                 // boolean value indicating a pressed switch
#endif

#ifdef EXT_SWITCH_1
  int ext_switch_1_pressed = 0;           // boolean value indicating a pressed switch
#endif

#ifdef EXT_SWITCH_2
  int ext_switch_2_pressed = 0;           // boolean value indicating a pressed switch
#endif

int timer0_interrupt = 0;               // boolean value indicating timer 0 A0 interrupt
int timer1_interrupt = 0;               // boolean value indicating timer 1 A0 interrupt
int adc_interrupt = 0;                  // boolean value indicating adc interrupt

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
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

    
  
  TA1CCR0 = 655-1;                          // timer A1 CCR0 value (20 ms before interrupt)
  TA1CCTL0 = CCIE;                          // timer A1 CCR0 interrupt enabled (compare mode)
  
  TA0CCTL0 = CCIE;              // timer A0 CCR0 interrupt enabled (compare mode)
  TA0CCR0 = 32-1;               // timer A0 CCR0 value (approx. 1ms before interrupt)
  TA0CTL = TASSEL_1 + MC_1;     // ACLK, upmode
  
  __enable_interrupt();
  #ifdef POTENTIO
    ADC10CTL0 |= ENC + ADC10SC;   // Sampling and conversion start
  #endif
  
  
  __bis_SR_register(LPM3_bits + GIE); // Enter LPM3 w/ interrupt
  
  // main loop
  while(1){
    
    // check if no interrupts are pending
    if(timer0_interrupt == 0 && timer1_interrupt == 0 && adc_interrupt == 0)
    {
      __bis_SR_register(LPM3_bits);         // Enter LPM3
    }
    
    // check if timer 0 did an interrupt
    if(timer0_interrupt == 1)
    {
      timer0_interrupt = 0;           // clear interrupt flag
      // check if command is received for updating output peripherals
      // TODO
      
    }

    // check if timer 1 did an interrupt
    if(timer1_interrupt == 1){  
      // check if switch is trully pressed
      if(~P1IN & 0x08)
      {
        //send_event(SWITCH_MCU,PRESS,0,0);       // send event to the server
      }
      
      P1IFG &=~0x08;                            // P1.3 IFG cleared
      P1IE = 0x08;                              // P1.3 only IRQ enable (reset switch after debouncing mode)
      timer1_interrupt = 0;                     // reset timer 1 interrupt flag
    }
    
    #ifdef POTENTIO
      // check if adc did an interrupt
      if (adc_interrupt == 1){
        adc_value = ADC10MEM;                                             // store last potentiometer converted value
        //send_event(POTENTIOMETER,POTENTIOMETER_UPDATE,adc_value,0);       // send event to the server
        adc_interrupt = 0;                                                // clear interrupt flag
        ADC10CTL0 |= ENC + ADC10SC;                                       // restart a conversion cycle
      }
    #endif
  }
    
  return 0;
}
  
// Port 1 interrupt service routine
// interrupt function when switch goes from HIGH to LOW
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  // starts debounce timer waiting time in order to check and validate pressed switch
  if (P1IFG & 0x08)
  {
    P1IE = 0x00;                // P1.3 only IRQ disabled
    P1IFG &=~0x08;              // P1.3 IFG cleared
    TA1R = 0;                   // reset count value
    TA1CCR0 = 655-1;            // timer A1 CCR0 value (20 ms before interrupt)
    TA1CTL = TASSEL_1 + MC_1;   // ACLK, upmode
  }
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  timer0_interrupt = 1;         // update interrupt flag
  LPM3_EXIT;
}

// Timer A1 interrupt service routine
// interrupt function when 20ms wait cycle is ended to check and validate pressed switch
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
  TA1CTL = TASSEL_1 + MC_0; // stop counting;
  timer1_interrupt = 1;
  LPM3_EXIT;
  
}

// ADC10 interrupt service routine
#ifdef POTENTIO
  #pragma vector=ADC10_VECTOR
  __interrupt void ADC10_ISR(void)
  {
    adc_interrupt = 1;                    // set flag to 1
    LPM3_EXIT;                            // wake up main function
  }
#endif


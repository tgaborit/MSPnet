
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"
#include "setup/board_setup.h"


#ifdef POTENTIO
  unsigned short adc_value = 0x0000;            // last registered adc value from potentiometer
#endif

#ifdef SWITCH_0
  int switch_pressed = 0;                       // boolean value indicating a pressed switch
  unsigned short switch_press_time = 0;         // counter to record how many time (in ms) the switch is held      
#endif

#ifdef EXT_SWITCH_1
  int ext_switch_1_pressed = 0;                 // boolean value indicating a pressed switch
  unsigned short ext_switch_1_press_time = 0;   // counter to record how many time (in ms) the switch is held
#endif

#ifdef EXT_SWITCH_2
  int ext_switch_2_pressed = 0;                 // boolean value indicating a pressed switch
  unsigned short ext_switch_2_press_time = 0;   // counter to record how many time (in ms) the switch is held
#endif

int timer0_interrupt = 0;                       // boolean value indicating timer 0 A0 interrupt
int timer0_CCR1_interrupt = 0;                  // boolean value indicating timer 0 A0 CCR1 interrupt
int timer0_CCR2_interrupt = 0;                  // boolean value indicating timer 0 A0 CCR2 interrupt
int timer1_interrupt = 0;                       // boolean value indicating timer 1 A0 interrupt
int adc_interrupt = 0;                          // boolean value indicating adc interrupt
int debounce_entity = 0;                        // value that keeps track of who is in debounce mode (0 for switch_0, 1 for ext_switch_1 and 2 for ext_switch_2)

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // setup all the input and output peripherals and devices
  full_setup();
  
  #ifdef POTENTIO
    ADC10CTL0 |= ENC + ADC10SC;   // adc first sampling and conversion start
  #endif
    
  __bis_SR_register(LPM3_bits + GIE); // Enter LPM3 w/ interrupt
  
  // main loop
  while(1){
    
    // check if no interrupts are pending
    if(timer0_interrupt == 0 && timer0_CCR1_interrupt == 0 && timer0_CCR2_interrupt == 0 && timer1_interrupt == 0)
    {
      #ifdef POTENTIO
        if(adc_interrupt == 0){
          __bis_SR_register(LPM3_bits);         // Enter LPM3
        }
      #else
        __bis_SR_register(LPM3_bits);         // Enter LPM3
      #endif
    }
    
    // check if timer 0 did an interrupt
    if(timer0_interrupt == 1)
    {
      timer0_interrupt = 0;           // clear interrupt flag
      
      // update switches press time if they are used and in press state
      #ifdef SWITCH_0
        if(switch_pressed == 1 && switch_press_time < 65535)
        {
          ++switch_press_time;
        }
      #endif
      
      #ifdef EXT_SWITCH_1
        if(ext_switch_1_pressed == 1 && ext_switch_1_press_time < 65535)
        {
          ++ext_switch_1_press_time;
        }
      #endif
      
      #ifdef EXT_SWITCH_2
        if(ext_switch_2_pressed == 1 && ext_switch_2_press_time < 65535)
        {
          ++ext_switch_2_press_time;
        } 
      #endif 
        
      // update outputs
      update_outputs(0);
    }
    
    if(timer0_CCR1_interrupt == 1){
      update_outputs(1);
    }
    
    if(timer0_CCR2_interrupt == 2){
      update_outputs(2);
    }

    // check if timer 1 did an interrupt
    if(timer1_interrupt == 1){  
      #ifdef SWITCH_0
        // check if debounced switch is trully pressed/released
        if (debounce_entity == 0){
          // checks if switch_0 has been pressed/released
          if(debounce(switch_pressed, 1, 0x08))
          {
            switch_pressed = (switch_pressed+1)%2;                              // set to 1 if pressed, 0 if released
            //send_event(SWITCH_MCU,switch_pressed,0,switch_press_time);        // send event to the server
            switch_press_time = 0;                                              // reset press time
          }
        }
      #endif
      
      #ifdef EXT_SWITCH_1
        // check if debounced switch is trully pressed/released
        if (debounce_entity == 1){
          // checks if ext_switch_1 has been pressed/released
          if(debounce(ext_switch_1_pressed, EXT_PORT_1, EXT_PIN_1))
          {
            ext_switch_1_pressed = (ext_switch_1_pressed+1)%2;                                  // set to 1 if pressed, 0 if released
            //send_event(SWITCH_MICRO_1, ext_switch_1_pressed,0,ext_switch_1_press_time);       // send event to the server
            ext_switch_1_press_time = 0;                                                        // reset press time
          }
        }
      #endif
      
      #ifdef EXT_SWITCH_2
        // check if debounced switch is trully pressed/released
        if (debounce_entity == 2){
          // checks if ext_switch_2 has been pressed/released
          if(debounce(ext_switch_2_pressed, EXT_PORT_2, EXT_PIN_2))
          {
            ext_switch_2_pressed = (ext_switch_2_pressed+1)%2;                                  // set to 1 if pressed, 0 if released
            //send_event(SWITCH_MICRO_2, ext_switch_2_pressed,0,ext_switch_2_press_time);       // send event to the server
            ext_switch_2_press_time = 0;                                                        // reset press time
          }
        }
      #endif
      timer1_interrupt = 0;             // reset timer 1 interrupt flag
    }
    
    #ifdef POTENTIO
      // check if adc did an interrupt
      if (adc_interrupt == 1){
        adc_value = ADC10MEM;                                             // store last potentiometer converted value
        //send_event(POTENTIOMETER,POTENTIOMETER_UPDATE,adc_value,0);     // send event to the server
        adc_interrupt = 0;                                                // clear interrupt flag
        ADC10CTL0 |= ENC + ADC10SC;                                       // restart a conversion cycle
      }
    #endif
  }
    
  return 0;
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  #ifdef SWITCH_0
    // check if switch_0 generated the interrupt
    if (P1IFG & 0x08)
    {
      P1IE != ~(0x08);            // P1.3 only IRQ disabled
      P1IFG &= ~(0x08);           // P1.3 only IFG cleared
      TA1R = 0;                   // reset count value
      TA1CCR0 = 655-1;            // timer A1 CCR0 value (20 ms before interrupt)
      TA1CTL = TASSEL_1 + MC_1;   // ACLK, upmode
      debounce_entity = 0;        // indicates switch_0 is in debounce mode
    }
  #endif
    
  #if defined(EXT_SWITCH_1) && (EXT_PORT_1 == 1)
    // check if ext_switch_1 is on port 1 and generated an interrupt
    if(P1IFG & EXT_PIN_1)
    {
      P1IE != ~(EXT_PIN_1);       // P1.x only IRQ disabled
      P1IFG &= ~(EXT_PIN_1);      // P1.x only IFG cleared
      TA1R = 0;                   // reset count value
      TA1CCR0 = 655-1;            // timer A1 CCR0 value (20 ms before interrupt)
      TA1CTL = TASSEL_1 + MC_1;   // ACLK, upmode
      debounce_entity = 1;        // indicates ext_switch_1 is in debounce mode
    }
  #endif
  
  #if defined(EXT_SWITCH_2) && (EXT_PORT_2 == 1)
    // check if ext_switch_2 is on port 1 and generated an interrupt
    if(P1IFG & EXT_PIN_2)
    {
      P1IE != ~(EXT_PIN_2);       // P1.x only IRQ disabled
      P1IFG &= ~(EXT_PIN_2);      // P1.x only IFG cleared
      TA1R = 0;                   // reset count value
      TA1CCR0 = 655-1;            // timer A1 CCR0 value (20 ms before interrupt)
      TA1CTL = TASSEL_1 + MC_1;   // ACLK, upmode
      debounce_entity = 2;        // indicates ext_switch_1 is in debounce mode
    }
  #endif   
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  #if defined(EXT_SWITCH_1) && (EXT_PORT_1 == 2)
    // check if ext_switch_1 is on port 2 and generated an interrupt
    if(P2IFG & EXT_PIN_1)
    {
      P2IE != ~(EXT_PIN_1);       // P2.x only IRQ disabled
      P2IFG &= ~(EXT_PIN_1);      // P2.x only IFG cleared
      TA1R = 0;                   // reset count value
      TA1CCR0 = 655-1;            // timer A1 CCR0 value (20 ms before interrupt)
      TA1CTL = TASSEL_1 + MC_1;   // ACLK, upmode
      debounce_entity = 1;        // indicates ext_switch_1 is in debounce mode
    }
  #endif
  
  #if defined(EXT_SWITCH_2) && (EXT_PORT_2 == 2)
    // check if ext_switch_2 is on port 2 and generated an interrupt
    if(P2IFG & EXT_PIN_2)
    {
      P2IE != ~(EXT_PIN_2);       // P2.x only IRQ disabled
      P2IFG &= ~(EXT_PIN_2);      // P2.x only IFG cleared
      TA1R = 0;                   // reset count value
      TA1CCR0 = 655-1;            // timer A1 CCR0 value (20 ms before interrupt)
      TA1CTL = TASSEL_1 + MC_1;   // ACLK, upmode
      debounce_entity = 2;        // indicates ext_switch_1 is in debounce mode
    }
  #endif   
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  // checks which CCR generated the interrupt
  switch(TA0IV){
    case 2:       timer0_CCR1_interrupt = 1;    // update interrupt flag
                  break;
    case 4:       timer0_CCR2_interrupt = 1;    // update interrupt flag
                  break;
    default:      timer0_interrupt = 1;         // update interrupt flag
  }
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

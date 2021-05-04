
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"

unsigned short adc_value = 0x0000;      // last registered adc value from potentiometer
int switch_pressed = 0;                 // boolean value indicating a pressed switch
int adc_interrupt = 0;                  // boolean value indicating adc interrupt
int timer0_interrupt = 0;               // boolean value indicating timer 0 A0 interrupt
int timer1_interrupt = 0;               // boolean value indicating timer 0 A0 interrupt

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // set up GPIOs (LED D1,D2,D3, switch, buzzer and potentiometer)
  P1DIR |= 0x01;                // P1.0 output (led D1)
  P1DIR |= 0x40;                // P1.6 output (led D2)
  P1DIR |= 0x00;                // P1.x output (buzzer)
  P1DIR != ~(0x08);             // P1.3 input  (switch)
  P2DIR |= 0x2A;                // P2.1 P2.3 and P2.5 output (LED D3)
  P1REN |= 0x08;                // P1.3 pullup/pulldown enable
  P1OUT = 0x08;                 // P1.0 LOW, P1.6 LOW, P1.3 HIGH (for pull-up)
  P1IE |= 0x08;                 // P1.3 IRQ enabled
  P1IES |= 0x08;                // P1.3 High to low edge only
  P1IFG &= ~(0x08);             // P1.3 IFG cleared
  __enable_interrupt();
  
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_4 + ADC10SSEL_1 + ADC10DIV_7; // input A1, ACLK, clock divider by 8
  ADC10AE0 |= 0x10; // PA.1 ADC option select
  
  TA1CCR0 = 655-1;                          // timer A1 CCR0 value (20 ms before interrupt)
  TA1CCTL0 = CCIE;                          // timer A1 CCR0 interrupt enabled (compare mode)
  
  TA0CCTL0 = CCIE;              // timer A0 CCR0 interrupt enabled (compare mode)
  TA0CCR0 = 32-1;               // timer A0 CCR0 value (approx. 1ms before interrupt)
  TA0CTL = TASSEL_1 + MC_1;     // ACLK, upmode
  
  ADC10CTL0 |= ENC + ADC10SC;   // Sampling and conversion start
  
  __bis_SR_register(LPM3_bits + GIE); // Enter LPM3 w/ interrupt
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
        send_event(SWITCH_MCU,PRESS,0,0);       // send event to the server
      }
      
      P1IFG &=~0x08;                            // P1.3 IFG cleared
      P1IE = 0x08;                              // P1.3 only IRQ enable (reset switch after debouncing mode)
      timer1_interrupt = 0;                     // reset timer 1 interrupt flag
    }
    
    // check if adc did an interrupt
    if (adc_interrupt == 1){
      adc_value = ADC10MEM;                                             // store last potentiometer converted value
      send_event(POTENTIOMETER,POTENTIOMETER_UPDATE,adc_value,0);       // send event to the server
      adc_interrupt = 0;                                                // clear interrupt flag
      ADC10CTL0 |= ENC + ADC10SC;                                       // restart a conversion cycle
    }
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
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  adc_interrupt = 1;                    // set flag to 1
  LPM3_EXIT;                            // wake up main function
}


// created by Emmanuel Durel
// set of functions to drive the output peripherals such as led, buzzer ...

#include "peripherals.h"
#include "../outputs/outputs.h"
#include "io430.h"
#include <stdint.h>


// Function that sets led D1 or D2 (ON/OFF depending on trigger).
static void set_mono_led(Output_type type, Output_trigger trig){
  
  int command[] = {0x01,0x40};          // array of commands to set D1 or D2 acting on appropriate pin.
  
  // if trigger is ON, set led.
  if(trig == 0){
    P1OUT |= command[type];             // set only the right pin.
  }
  
  // if trigger is OFF, reset led.
  else{
    P1OUT != ~(command[type]);          // reset only the right pin.
  }
}

// Function that sets led D3 (ON/OFF depending on trigger).
static void set_tri_led(Output_type type, Output_trigger trig){
  
  int command[] = {0x2A, 0x08, 0x02, 0x20, 0x22};       // array of commands to set D3 with appropriate color.
  
  // if trigger is ON, set led.
  if(trig == 0){
    P2OUT |= command[type-2];                           // set only P2.1, P2.3 and P2.5
  }
  
  // if trigger is OFF, reset led.
  else{
    P2OUT != ~(command[type-2]);                        // reset only P2.1, P2.3 and P2.5
  }
}

// Function that sets buzzer (ON/OFF depending on trigger).
static void set_buzzer(Output_trigger trig){
  
  // if trigger is ON, set buzzer.
  if(trig == 0){
    P1OUT |= 0x00;      // set only P1.x
  }
  
  // if trigger is OFF, reset led.
  else{
    P1OUT != 0xFF;      // reset only P1.x
  }
}

// Function to set a peripheral (LED, buzzer)
void set_peripheral(Output_type type, Output_trigger trigger){
  switch(type){
  case 0:       ;
  case 1:       set_mono_led(type, trigger);
                break;
  case 2:       ;
  case 3:       ;
  case 4:       ;
  case 5:       ;
  case 6:       set_tri_led(type, trigger);   
                break;
  case 7:       set_buzzer(trigger);
  }
}
               
  

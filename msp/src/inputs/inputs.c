#include "inputs.h"

// function to modify variables and switch registers after a debounce operation. It returns 1 if a press/release occured
int debounce(int pressed, int port, int pin){
  int result = 0;
  
  // go in code section corresponding to actual port
  if(port == 1){
    // checks if switch is trully pressed
    if((pressed == 0) && (~P1IN & pin))
    {
      P1IES &= ~pin;                                // P1.x low to high edge only, in order to detect released button
      result = 1;
    }
    // checks if switch has been released
    else if((pressed == 1) && (P1IN & pin))
    {
      P1IES |= pin;                                // P1.x high to low edge only, in order to detect pushed button
      result = 1;
    }
    P1IFG &= ~pin;                                    // P1.x only IFG cleared
    P1IE |= pin;                                      // P1.x only IRQ enable (reset switch after debouncing mode) 
    return result;
  }
  else{
    // checks if switch is trully pressed
    if((pressed == 0) && (~P2IN & pin))
    {
      P2IES &= ~0x08;                               // P2.x low to high edge only, in order to detect released button
      result = 1;
    }
    // checks if switch has been released
    else if((pressed == 1) && (P2IN & pin))
    {
      P2IES |= 0x08;                                // P2.x high to low edge only, in order to detect pushed button
      result = 1;
    }
    P2IFG &=~pin;                                    // P2.x only IFG cleared
    P2IE |= pin;                                      // P2.x only IRQ enable (reset switch after debouncing mode)
    return result;
  }
}
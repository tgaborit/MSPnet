#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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


void send_event(Input_type type, Input_event event, Intensity intensity, Duration duration_ms){
  
  char json_string[128];  // final json string
  char tmp[10];           // temporary variable holding int as strings
  
  /* Device ID */
  strcpy(json_string, "{\"id\":");
  sprintf(tmp, "\"%.3u\"", DEVICE_ID);
  strcat(json_string, tmp);
  
  /* TYPE */
  strcat(json_string, ",\"type\":");
  sprintf(tmp, "\"%d\"", type);
  strcat(json_string, tmp);
  
  /* EVENT */
  strcat(json_string, ",\"event\":");
  sprintf(tmp, "\"%d\"", event);
  strcat(json_string, tmp);

  
  /* INTENSITY */
  strcat(json_string, ",\"intensity\":");
  sprintf(tmp, "\"%.6d\"", intensity);
  strcat(json_string, tmp);

  /* DURATION */
  strcat(json_string, ",\"duration\":");
  sprintf(tmp, "\"%.6d\"", duration_ms);
  strcat(json_string,tmp);

  strcat(json_string, "}");
  
  comm_MQTT_pub_event(json_string); //send message to ESP
}

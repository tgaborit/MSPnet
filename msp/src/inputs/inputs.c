#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../inputs/inputs.h"

#define DEVICE_ID 123

void send_event(Input_type type, Input_event event, Intensity intensity, Duration duration_ms){
  
  char json_string[128];  // final json string
  char tmp[10];           // temporary variable holding int as strings
  
  /* Device ID */
  strcpy(json_string, "{\"id\":");
  sprintf(tmp, "%.6d", DEVICE_ID);
  strcat(json_string, tmp);
  
  /* TYPE */
  strcat(json_string, ", \"type\":");
  sprintf(tmp, "%d", type);
  strcat(json_string, tmp);
  
  /* EVENT */
  strcat(json_string, ", \"event\":");
  sprintf(tmp, "%d", event);
  strcat(json_string, tmp);

  
  /* INTENSITY */
  strcat(json_string, ", \"intensity\":");
  sprintf(tmp, "%.6d", intensity);
  strcat(json_string, tmp);

  /* DURATION */
  strcat(json_string, ", \"duration\":");
  sprintf(tmp, "%.6d", duration_ms);
  strcat(json_string,tmp);

  strcat(json_string, "}");
  
  
  // TODO : send message to ESP
}

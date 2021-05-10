#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../json/jsmn.h"
#include "../outputs/outputs.h"
#include "../json/json_parser.h"

#define JSMN_STRICT


/* Checks if substring is in json string
 * Returns 1 if string is found, 0 otherwise
*/
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 1;
  }
  return 0;
}



/* Returns 1 if token t of json_string is an 1-character integer, 0 otherwise */
static int is_int(char* json_string, jsmntok_t* t) {
  
  /* token must be a number (which is a JSMN primitive) */
  if (t->type != JSMN_PRIMITIVE) {
    return 0;
  }
  
  /* token must be a single char */
  if (t->end - t->start != 1) {
    return 0;
  }
  
  /* token must be between 0 and 9 */
  if (json_string[t->start] < '0' || json_string[t->start] > '9') {
    return 0;
  }
  
  return 1;
}



/* Parses json_string and fills output_message with the parsed data */
static int parse_json(char* json_string, Output_message* output_message) {
  
  int i;            // loop iterator
  int r;            // number of tokens found by the parser when parsing
  jsmn_parser p;    // jsmn parser
  jsmntok_t t[16];  // holds the list of tokens found by the parser
  
  /* initialise jsmn parser */
  jsmn_init(&p);
  
  
  
  /* JSON PARSING */
  /* parse input json string */
  r = jsmn_parse(&p, json_string, strlen(json_string), t,
                 sizeof(t) / sizeof(t[0]));
  
  /* if parsing failed */
  if (r < 0) {
    return EXIT_FAILURE;
  }

  /* assume the top-level element is an object */
  if (r < 1 || t[0].type != JSMN_OBJECT) {
    return EXIT_FAILURE;
  }

  
  
  /* DATA INTERPRETATION */
  /* loop over all keys of the root object */
  for (i = 1; i < r; i++) {
    
    /* TYPE field */
    if (jsoneq(json_string, &t[i], "type") == 1) {
      
      /* TYPE should be a 1-char integer */
      if (! is_int(json_string, &t[i+1])) {
        return EXIT_FAILURE;
      }
      
      /* convert TYPE field into an integer */
      output_message->type = (Output_type)atoi(&json_string[t[i+1].start]);
      i++;
      
      
     
    /* TRIGGER field */
    } else if (jsoneq(json_string, &t[i], "trigger") == 1) {
      
      /* TRIGGER should be a 1-char integer */
      if (! is_int(json_string, &t[i+1])) {
        return EXIT_FAILURE;
      }
      
      /* convert TRIGGER field into an integer */
      output_message->trigger = (Output_trigger)atoi(&json_string[t[i+1].start]);
      i++;
      
      
      
    /* INTENSITY field */
    } else if (jsoneq(json_string, &t[i], "intensity") == 1) {
      
      /* convert INTENSITY field into an integer */
      output_message->intensity = atoi(&json_string[t[i+1].start]);
      i++;     

      
      
     /* DURATION field */
     } else if (jsoneq(json_string, &t[i], "duration") == 1) {
       
      /* convert DURATION field into an integer */
      output_message->duration = atoi(&json_string[t[i+1].start]);
      i++;
      
    } else {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

/* Parses a message received by the device */
int parse_message(char* message) {
  
  int r;                          // result of parsing
  Output_message output_message;  // holds output parameters
  
  /* Parse JSON message */
  r = parse_json(message, &output_message);
  if(!r) {
    return EXIT_FAILURE;
  }
  
  /* Asks device to perform ouput operation */
//  output(output_message.type, output_message.trigger, output_message.intensity, output_message.duration); 
  
  return EXIT_SUCCESS;
}
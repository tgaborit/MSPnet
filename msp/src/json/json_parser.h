#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "../outputs/outputs.h"

#define JSON_STRING_LENGTH 59

/* Parses a message received by the device and calls appropriate
 * ouptut functions */
int parse_message(char* message);

#endif //JSON_PARSER_H
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "../outputs/outputs.h"

/* Parses json_string and fills output_message with the parsed data */
int parse_json(char* json_string, Output_message* output_message);


#endif //JSON_PARSER_H
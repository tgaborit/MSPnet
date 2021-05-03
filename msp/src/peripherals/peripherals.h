// created by Emmanuel Durel
// set of low-level functions to drive the output peripherals such as led, buzzer ...

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <stdint.h>
#include "../outputs/outputs.h"

void set_peripheral(Output_type type, Output_trigger trigger);


#endif //PERIPHERALS_H
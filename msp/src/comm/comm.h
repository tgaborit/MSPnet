#ifndef COMM_H

#include "io430.h"

#define COMM_H

void UART_init();
void UART_TX_byte(char value);
void UART_TX_val(char *str);
void UART_TX_end();
void UART_TX(char *str);

#endif
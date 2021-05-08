#ifndef COMM_H

#include "io430.h"

#define COMM_H

#define WIFINETWORK  "HUAWEI P10 lite"
#define WIFIPASSWORD "12345678"

void comm_UART_init();
void comm_UART_TX_byte(char value);
void comm_UART_TX_val(char *str);
void comm_UART_TX_end();
void comm_UART_TX(char *str);
void comm_ESP_delay();
void comm_ESP_rst();
void comm_init();

#endif
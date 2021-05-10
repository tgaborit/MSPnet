#ifndef COMM_H

#include <stdint.h>
#include "io430.h"

#define COMM_H

#define WIFINETWORK  "HUAWEI P10 lite"
#define WIFIPASSWORD "12345678"
#define MQTTHOST     "ec2-3-8-210-204.eu-west-2.compute.amazonaws.com"
#define MQTTPORT     "1883"


void comm_UART_init();
void comm_UART_TX_byte(char value);
void comm_UART_TX_end();
void comm_UART_TX_raw(uint8_t *data, int length);
void comm_UART_TX_str_val(char *str);
void comm_UART_TX_str(char *str);
void comm_delay_s(int delay);
void comm_ESP_rst();
void comm_init_WIFI();
void comm_MQTT_init();
void comm_init();

#endif
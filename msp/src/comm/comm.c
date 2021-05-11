#include "comm.h"

/* Function for initialising UART communication */
void comm_UART_init()
{
  P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2; 
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK - 1 MHz
  
  UCA0BR0 = 8 ;                            // Configuration for 115200 baud
  UCA0BR1 = 0;
  UCA0MCTL = UCBRS2 + UCBRS0;
  
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
  __enable_interrupt();
}

/* Write byte to ESP interface */
void comm_UART_TX_byte(char value)
{
  while (!(IFG2 & UCA0TXIFG))
    ;
  // wait for TX buffer to be ready for new data
  // UCA0TXIFG register will be truthy when available to recieve new data to computer.
  UCA0TXBUF = value;
}

/* Write several bytes to ESP interface */
void comm_UART_TX_raw(uint8_t *data, int length)
{
  for(int i = 0; i < length; i++)
  {
    //write data byte by byte
    comm_UART_TX_byte(data[i]);
  }
}

/* Write end characters for AT commands */
void comm_UART_TX_end()
{
  comm_UART_TX_byte('\r');
  comm_UART_TX_byte('\n');
}

/* Write string to ESP interface */
void comm_UART_TX_str_val(char *str)
{
  int i = 0;
  while (str[i] != '\0') // End of string character not needed for UART
  {
    //write string byte by byte
    comm_UART_TX_byte(str[i++]);
  }
}
  
/* Write string command to ESP interface, used for AT commands */
void comm_UART_TX_str(char *str)
{
  comm_UART_TX_str_val(str);
  comm_UART_TX_end();
}

/* Delay used for ESP processing */
void comm_delay_s(int delay)
{
  for (int i = 0; i< delay; i++)
    __delay_cycles(1000000);
}

/* Reset of ESP configuration */
void comm_ESP_rst()
{
  comm_UART_TX_str("AT+RST");
  comm_delay_s(1);
}

/* Setup of WiFi connection */
void comm_WIFI_init()
{
  /* Configure ESP in Station mode */ 
  comm_UART_TX_str("AT+CWMODE=1");
  comm_delay_s(2);
  
  /* Connect to WiFi network */
  comm_UART_TX_str_val("AT+CWJAP=\"");
  comm_UART_TX_str_val(WIFINETWORK);
  comm_UART_TX_str_val("\",\"");
  comm_UART_TX_str_val(WIFIPASSWORD);
  comm_UART_TX_str_val("\"");
  comm_UART_TX_end();
  comm_delay_s(20);
}

/* Setup of TCP communication with Mosquitto */
void comm_MQTT_TCP()
{
  /* Establishing TCP connection */
  comm_UART_TX_str_val("AT+CIPSTART=\"TCP\",\"");
  comm_UART_TX_str_val(MQTTHOST);
  comm_UART_TX_str_val("\",");
  comm_UART_TX_str_val(MQTTPORT);
  comm_UART_TX_end();
  comm_delay_s(10);
}

/* MQTT CONNECTION with Mosquitto broker */
void comm_MQTT_conn()  
{
  uint8_t data[] = {
      // type 1
      0x10,
      // remaining length 14
      14,
      // protocol name
      0x00, 0x04, 
      'M', 'Q', 'T', 'T',
      // protocol version
      0x05,
      // connect flags
      0x00,
      // keep-alive
      0x00, 0x00,
      // property length
      0x00,
      // client id
      0x00, 0x01,
      '1'
  };
  
  // Prepare to send 16 bytes
  comm_UART_TX_str("AT+CIPSEND=16");
  comm_delay_s(2);
  
  // Send the 16 bytes
  comm_UART_TX_raw(data, 16);
    
  comm_delay_s(10);
}

/* MQTT SUBSCRIBE to topic dedicated to outputs for the device */
void comm_MQTT_sub()
{
  uint8_t data[] = {
      // type 8
      0x82,
      // remaining length 14
      14,
      // packet identifier
      0x00, 0x01,
      // property length
      0x00,
      // length topic filter
      0x00, 0x08,
      'O', 'U', 'T', 'P', 'U', 'T', '/', '1',
      // subscription options
      0x00,
  };
  
  // Prepare to send 16 bytes
  comm_UART_TX_str("AT+CIPSEND=16"); // prepare to send x byte
  comm_delay_s(2);
  
  // Send the 16 bytes
  comm_UART_TX_raw(data, 16);
  
  comm_delay_s(10);
}

void comm_MQTT_id()
{
  uint8_t data[20] = {
      // type 3
      0x30,
      // remaining length xx
      18,
      // topic length
      0x00, 0x03,
      // topic name
      'N', 'E', 'W',
      // property length
      0x00
  };
  
  char json_string[128];  // final json string
  char tmp[10];           // temporary variable holding int as strings
  
  /* Payload composed of Device ID */
  strcpy(json_string, "{\"id\":");
  sprintf(tmp, "\"%.3u\"", DEVICE_ID);
  strcat(json_string, tmp);  
  strcat(json_string, "}");
  
  // payload
  memcpy(&data[8], json_string, 12);
  
  // Prepare to send 20 byte
  comm_UART_TX_str("AT+CIPSEND=20");
  comm_delay_s(2);
  
  // Send the 20 bytes
  comm_UART_TX_raw(data, 20);
  comm_delay_s(2);
}

void comm_MQTT_init()
{
  comm_UART_TX_str("ATE0");        // Disable echo
  comm_delay_s(1);
  
  comm_UART_TX_str("AT+CIPMUX=0"); // Single-Connection Mode
  comm_delay_s(1);
  
  comm_MQTT_TCP();
  
  comm_MQTT_conn();
  
  comm_MQTT_id();
  
  comm_MQTT_sub();
}

// Initialisation of communications
void comm_init()
{
  comm_UART_init();
  IE2 &= ~UCA0RXIE; // Disable UART RX interrupt before we send data
  comm_delay_s(1);
  
  comm_ESP_rst();
  comm_WIFI_init();
  
  comm_MQTT_init();
  
  IE2 |= UCA0RXIE;
}

/* MQTT PUBLISH event with input */
void comm_MQTT_pub_event(uint8_t *payload)
{
  uint8_t data[89] = {
      // type 3
      0x30,
      // remaining length 87
      87,
      // topic length
      0x00, 0x05,
      // topic name
      'I', 'N', 'P','U', 'T',
      // property length
      0x00
  };
  
  // payload
  memcpy(&data[10], payload, 79);
  
  // Prepare to send 89 byte
  comm_UART_TX_str("AT+CIPSEND=89");
  comm_delay_s(2);
  
  // Send the 89 bytes
  comm_UART_TX_raw(data, 89);
  comm_delay_s(2);
}
#include "comm.h"

void comm_UART_init()
{
  P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2; 
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK - 1 MHz
  
  UCA0BR0 = 8 ;                            // Prescaler for 115200 baud
  UCA0BR1 = 0;
  
  UCA0MCTL = UCBRS2 + UCBRS0;
  
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
//  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  
  __enable_interrupt();
}

/* Write byte to USB-Serial interface */
void comm_UART_TX_byte(char value)
{
  while (!(IFG2 & UCA0TXIFG))
    ;
  // wait for TX buffer to be ready for new data
  // UCA0TXIFG register will be truthy when available to recieve new data to computer.
  UCA0TXBUF = value;
}

void comm_UART_TX_raw(uint8_t *data, int length)
{
  for(int i = 0; i < length; i++)
  {
    //write data byte by byte
    comm_UART_TX_byte(data[i]);
  }
}

void comm_UART_TX_end()
{
  comm_UART_TX_byte('\r');
  comm_UART_TX_byte('\n');
}

  
void comm_UART_TX_str_val(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    //write string byte by byte
    comm_UART_TX_byte(str[i++]);
  }
}
  
void comm_UART_TX_str(char *str)
{
  comm_UART_TX_str_val(str);
  comm_UART_TX_end();
}

void comm_delay_s(int delay)
{
  for (int i = 0; i< delay; i++)
    __delay_cycles(1000000);
}

void comm_ESP_rst()
{
  comm_UART_TX_str("AT+RST");
  comm_delay_s(1);
}

void comm_init_WIFI()
{
  comm_UART_TX_str("AT+CWMODE=1");
  comm_delay_s(2);
  
  comm_UART_TX_str_val("AT+CWJAP=\"");
  comm_UART_TX_str_val(WIFINETWORK);
  comm_UART_TX_str_val("\",\"");
  comm_UART_TX_str_val(WIFIPASSWORD);
  comm_UART_TX_str_val("\"");
  comm_UART_TX_end();
  comm_delay_s(20);
}

void comm_MQTT_init()
{
  comm_UART_TX_str("ATE0");        // Disable echo
  comm_delay_s(1);
  
  comm_UART_TX_str("AT+CIPMUX=0"); // Single-Connection Mode
  comm_delay_s(1);
  
  comm_UART_TX_str_val("AT+CIPSTART=\"TCP\",\""); // Establish TCP connection
  comm_UART_TX_str_val(MQTTHOST);
  comm_UART_TX_str_val("\",");
  comm_UART_TX_str_val(MQTTPORT);
  comm_UART_TX_end();
  comm_delay_s(10);
  
  IE2 |= UCA0RXIE;
  
  uint8_t data[] = {
      // type 1
      0x10,
      // length 14
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
  
  comm_UART_TX_str("AT+CIPSEND=16"); // prepare to send x byte
  comm_delay_s(2);
  
  comm_UART_TX_raw(data, 16);
    
  comm_delay_s(10);
  
  uint8_t data2[] = {
      // type 1
      0x82,
      // length 14
      7,
      // packet identifier
      0x00, 0x01,
      // property length
      0x00,
      // length topic filter
      0x00, 0x01,
      '1',
      // subscription options
      0x00,
  };
  
  comm_UART_TX_str("AT+CIPSEND=9"); // prepare to send x byte
  comm_delay_s(2);
  
  comm_UART_TX_raw(data2, 9);
  
}


void comm_init()
{
  comm_UART_init();
  IE2 &= ~UCA0RXIE; // Disable UART RX interrupt before we send data
  comm_delay_s(1);
  
  comm_ESP_rst();
  comm_init_WIFI();
  
  comm_MQTT_init();

}
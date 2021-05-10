#include <string.h>
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"
#include "comm/comm.h"
#include "comm/mqtt_recv.h"
#include "json/json_parser.h"

char rx_buffer[256] = {'\0'};
int i_rx_buffer = 0;


int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  comm_init();
       
  while(1);
  
  return 0;
}

// UART RX interrupt service routine
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    char topic[TOPIC_MAX_LENGTH];
    char payload[PAYLOAD_MAX_LENGTH];
    uint8_t mqtt_message[100];
  
    rx_buffer[i_rx_buffer] = UCA0RXBUF;
    i_rx_buffer++;
    
    if (rx_buffer[i_rx_buffer - 1] == '}')
    {
      memcpy(mqtt_message, &rx_buffer[11], 100);
      mqtt_recv_publish(mqtt_message, topic, payload);
      parse_message(payload);
      
      i_rx_buffer = 0;
    }
}   


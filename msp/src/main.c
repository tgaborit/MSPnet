#include <string.h>
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"
#include "comm/comm.h"

#define WIFINETWORK  "HUAWEI P10 lite"
#define WIFIPASSWORD "12345678"

char rx_buffer[256] = {'\0'};
int i_rx_buffer = 0;


int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  UART_init();
  
  IE2 &= ~UCA0RXIE; // Disable UART RX interrupt before we send data
  __delay_cycles(500000);
  
//  UART_TX("AT+RST");
//  
//  __delay_cycles(20000);
  
  UART_TX("AT+CWMODE=1");
  
  __delay_cycles(500000);
//  __delay_cycles(5000);
  
//  UART_TX("AT+CWJAP=\"HUAWEI P10 lite\",\"12345678\"");
  
  UART_TX_val("AT+CWJAP=\"");
  UART_TX_val(WIFINETWORK);
  UART_TX_val("\",\"");
  UART_TX_val(WIFIPASSWORD);
  UART_TX_val("\"");
  UART_TX_end();
  
  __delay_cycles(500000);
  
  IE2 |= UCA0RXIE;
  
  while(1);
  
  return 0;
}

// UART RX interrupt service routine
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    rx_buffer[i_rx_buffer] = UCA0RXBUF;
    i_rx_buffer++;
}   


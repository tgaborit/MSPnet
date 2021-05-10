#include <string.h>
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"
#include "comm/comm.h"


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
    rx_buffer[i_rx_buffer] = UCA0RXBUF;
    i_rx_buffer++;
}   


#include "comm.h"

void UART_init()
{
  P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2; 
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK - 1 MHz
  
  UCA0BR0 = 8 ;                            // Prescaler for 115200 baud
  UCA0BR1 = 0;
  
  UCA0MCTL = UCBRS2 + UCBRS0;
  
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  
  __enable_interrupt();
}

/* Write byte to USB-Serial interface */
void UART_TX_byte(char value)
{
  while (!(IFG2 & UCA0TXIFG))
    ;
  // wait for TX buffer to be ready for new data
  // UCA0TXIFG register will be truthy when available to recieve new data to computer.
  UCA0TXBUF = value;
}

void UART_TX_val(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    //write string byte by byte
    UART_TX_byte(str[i++]);
  }
}
  
void UART_TX_end()
{
  UART_TX_byte('\r');
  UART_TX_byte('\n');
}
  
void UART_TX(char *str)
{
  UART_TX_val(str);
  UART_TX_end();
}
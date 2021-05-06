
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"

char WifiNetwork[] = "HUAWEI P10 lite";
char password[] = "12345678";

char rx_buffer[256] = {0};
int i_rx_buffer = 0;

void init_UART()
{
  P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2; 
  
//  P1DIR = P2;
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK - 1 MHz
  
  UCA0BR0 = 104;                            // Prescaler for 9600 baud
  UCA0BR1 = 0;
  
  UCA0MCTL = UCBRS0;
  
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  
//  IFG2 = UCA0TXIFG;
  //__enable_interrupt();
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

void UART_TX(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    //write string byte by byte
    UART_TX_byte(str[i++]);
  }
  UART_TX_byte('\r');
  UART_TX_byte('\n');
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  init_UART();
  
//  IE2 &= ~UCA0RXIE; // Disable UART RX interrupt before we send data
  
  UART_TX("AT");
  
  while(1);
  
  return 0;
}

// UART RX interrupt service routine
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    rx_buffer[i_rx_buffer] = UCA0TXBUF;
    i_rx_buffer++;
}   


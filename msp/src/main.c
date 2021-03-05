
#include "io430.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  return 0;
}

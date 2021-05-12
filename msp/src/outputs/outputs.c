#include "outputs.h"
#include "io430.h"

int D1_PWM = 0;                 // flag indicating if D1 is in PWM mode
int LED_D1_timer = 0;           // duration timer for led D1
int LED_D2_timer = 0;           // duration timer for led D2
int D2_PWM = 0;                 // flag indicating if D2 is in PWM mode
int LED_D3_timer = 0;           // duration timer for led D3
int D3_PWM = 0;                 // flag indicating if D3 is in PWM mode
int buzzer_timer = 0;           // duration timer for buzzer
int buzzer_PWM = 0;             // flag indicating if buzzer is in PWM mode

// Function that sets led D1 following the trigger, intensity and duration parameters
static void set_D1(Output_trigger trig, Intensity intensity, Duration duration){
  
  // if trigger is ON, set led.
  if(trig == 0){
    if (intensity != 0){
      D1_PWM = 1;                       // indicate PWM mode ON
      TA0CCR1 = (intensity/8);          // define PWM period with intensity
      TA0R = 0;                         // restart timer A0 to avoid inverted PWM
      TA0CCTL1 = CCIE;                  // timer A0 CCR1 interrupt enabled (compare mode)
    }
    // set timer to infinite (-1) if value is 0
    if(duration == 0){
      LED_D1_timer = -1;
    }
    // set timer to indicated duration
    else{
      LED_D1_timer = duration;
    }
    
    P1OUT |= 0x01;             // set only the right pin.
  }
  
  // if trigger is OFF, reset led.
  else{
    P1OUT &= ~(0x01);          // reset only the right pin.
    LED_D1_timer = 0;          // reset the timer
    D1_PWM = 0;
  }
}


// Function that sets led D2 following the trigger, intensity and duration parameters
static void set_D2(Output_trigger trig, Intensity intensity, Duration duration){
  
  // if trigger is ON, set led.
  if(trig == 0){
    if (intensity != 0){
      D2_PWM = 1;                       // indicate PWM mode ON
      TA0CCR1 = (intensity/8);          // define PWM period with intensity
      TA0R = 0;                         // restart timer A0 to avoid inverted PWM
      TA0CCTL1 = CCIE;                  // timer A0 CCR1 interrupt enabled (compare mode)
    }
    
    // set timer to infinite (-1) if value is 0
    if(duration == 0){
      LED_D2_timer = -1;
    }
    // set timer to indicated duration
    else{
      LED_D2_timer = duration;
    }
    P1OUT |= 0x40;             // set only the right pin.
  }
  
  // if trigger is OFF, reset led.
  else{
    P1OUT &= ~(0x40);          // reset only the right pin.
    LED_D2_timer = 0;          // reset the timer
    D2_PWM = 0;
  }
}

// Function that sets led D3 (ON/OFF depending on trigger).
static void set_D3(Output_type type, Output_trigger trig, Intensity intensity, Duration duration){
  
  int command[] = {0x2A, 0x08, 0x02, 0x20, 0x22};       // array of commands to set D3 with appropriate color.
  
  // if trigger is ON, set led.
  if(trig == 0){
    if (intensity != 0){
      D3_PWM = 1;                       // indicate D3 PWM mode ON
      TA0CCR1 = (intensity/8);          // define PWM period with intensity
      TA0R = 0;                         // restart timer A0 to avoid inverted PWM
      TA0CCTL1 = CCIE;                  // timer A0 CCR1 interrupt enabled (compare mode)
    }
    // set timer to infinite (-1) if value is 0
    if(duration == 0){
      LED_D3_timer = -1;
    }
    // set timer to indicated duration
    else{
      LED_D3_timer = duration;
    }
    P2OUT &= ~(command[0]);                             // reset first P2.1, P2.3 and P2.5
    P2OUT |= command[type-2];                           // set only P2.1, P2.3 and P2.5
  }
  
  // if trigger is OFF, reset led.
  else{
    P2OUT &= ~(command[0]);                             // reset only P2.1, P2.3 and P2.5
    LED_D3_timer = 0;                                   // reset the timer
    D3_PWM = 0;
  }
}

// Function that sets buzzer (ON/OFF depending on trigger).
static void set_buzzer(Output_trigger trig, Intensity intensity, Duration duration){
  
  // if trigger is ON, set buzzer.
  if(trig == 0){
    if (intensity != 0){
      buzzer_PWM = 1;                       // indicate buzzer PWM mode ON
      TA0CCR2 = (intensity/8);          // define PWM period with intensity
      TA0R = 0;                         // restart timer A0 to avoid inverted PWM
      TA0CCTL2 = CCIE;                  // timer A0 CCR2 interrupt enabled (compare mode)
    }
    // set timer to infinite (-1) if value is 0
    if(duration == 0){
      buzzer_timer = -1;
    }
    // set timer to indicated duration
    else{
      buzzer_timer = duration;
    }
    if(BUZZ_PORT == 1){
      P1OUT |= BUZZ_PIN;        // set only P1.x
    }
    else{
      P2OUT |= BUZZ_PIN;        // set only P2.x
    }
  }
  
  // if trigger is OFF, reset led.
  else{
    if(BUZZ_PORT == 1){
      P1OUT &= ~(BUZZ_PIN);        // reset only P1.x
    }
    else{
      P2OUT &= ~(BUZZ_PIN);        // reset only P2.x
    }
    buzzer_timer = 0;                                   // reset the timer
    buzzer_PWM = 0;
  }
}

// function that analyses a server message and calls other functions to modify the microc outputs consequently.
void output(Output_type type, Output_trigger trigger, Intensity intensity, Duration duration){
  switch(type){
  case 0:       set_D1(trigger, intensity, duration);
                break;
  case 1:       set_D2(trigger, intensity, duration);
                break;
  case 2:       ;
  case 3:       ;
  case 4:       ;
  case 5:       ;
  case 6:       set_D3(type, trigger, intensity, duration);   
                break;
  case 7:       set_buzzer(trigger, intensity, duration);
  }
}

// function that updates the outputs state (toggling leds and buzzer, checking if duration is not overlapped ...)

void update_outputs(int mode){
  // check if timer is still good for D1 and update it
  if(LED_D1_timer != 0){
   // update timer in case of finite duration
    if((mode == 0) && (LED_D1_timer != -1)){
      --LED_D1_timer;
    }
    // check if D1 needs to be toggled in case of PWM mode
    if((mode != 2) && (D1_PWM == 1)){
      P1OUT ^= 0x01;
    }
  }
  else{
    D1_PWM = 0;         // else reset led parameters
    P1OUT &= ~(0x01);
  }
  
  // check if timer is still good for D2 and update it
  if(LED_D2_timer != 0){
    // update timer in case of finite duration
    if((mode == 0) && (LED_D2_timer != -1)){
      --LED_D2_timer;
    }
    // check if D2 needs to be toggled in case of PWM mode
    if((mode != 2) && (D2_PWM == 1)){
      P1OUT ^= 0x40;
    }
  }
  else{
    D2_PWM = 0;         // else reset led parameters
    P1OUT &= ~(0x40);
  }
    
  // check if timer is still good for D3 and update it
  if(LED_D3_timer != 0){
   // update timer in case of finite duration
    if((mode == 0) && (LED_D3_timer != -1)){
      --LED_D3_timer;
    }
    // check if D3 needs to be toggled in case of PWM mode
    if((mode != 2) && (D3_PWM == 1)){
      P2OUT ^= 0x2A;
    }
  }
  else{
    D3_PWM = 0;         // else reset led parameters
    P2OUT &= ~(0x2A);
  }
  
  // check if timer is still good for buzzer and update it
  if(buzzer_timer != 0){
   // update timer in case of finite duration
    if((mode == 0) && (buzzer_timer != -1)){
      --buzzer_timer;
    }
    // check if D3 needs to be toggled in case of PWM mode
    if((mode != 1) && (buzzer_PWM == 1)){
      if(BUZZ_PORT == 1){
        P1OUT ^= BUZZ_PIN;        // toggle only P1.x
      }
      else{
        P2OUT ^= BUZZ_PIN;     // toggle only P2.x
      }
    }
  }
  else{
    D3_PWM = 0;         // else reset led parameters
    if(BUZZ_PORT == 1){
        P1OUT &= ~(BUZZ_PIN);   // reset only P1.x
      }
    else{
      P2OUT &= ~(BUZZ_PIN);     // reset only P2.x
    }
  }
  if((D1_PWM == 0) && (D2_PWM == 0) && (D3_PWM == 0)){
    TA0CCTL1 &= ~(CCIE);        // timer A0 CCR1 interrupt disable
  }
  if(buzzer_PWM == 0){
    TA0CCTL2 &= ~(CCIE);            // timer A0 CCR2 interrupt enabled (compare mode)
  }
}
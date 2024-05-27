#include "shaft_encoder.h"

void setup_encoder(uint8_t mask, uint16_t timer_duration_ms)
{
  UART_putString("Setting up encoders\n");

  // set sleep mode
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  // set as input
  DDRB &= ~mask;
  // enable pull up resistor
  PORTB |= mask;
  
  // set interrupt on change, looking up PCMSK0
  PCICR |= (1 << PCIE0); 

  // set timer
  TCCR5A = 0;
  TCCR5B = (1 << WGM52) | (1 << CS50) | (1 << CS52);
  OCR5A = (uint16_t)(15.62*timer_duration_ms);

  // clear interrupt
  cli();
  // set PCINT0 to trigger an interrupt on state change 
  PCMSK0 |= mask;   
  // enable the timer interrupt
  TIMSK5 |= (1 << OCIE5A);
  // enable interrupt
  sei();
}

void print_status_encoder(state_t *enc, int tot_enc) 
{
  for (int i = 0; i < tot_enc; i++) 
      {     
        unsigned char out[1024];
        sprintf((char*) out, 
          "encoder: [%d]\n"
          "prev value    = %d%d\n"
          "curr value    = %d%d\n"
          "counter value = %d\n\n", 
          i, 
          (enc[i].prev_value & 0x2) >> 1, enc[i].prev_value & 0x1,
          (enc[i].curr_value & 0x2) >> 1, enc[i].curr_value & 0x1,
          enc[i].counter
        );
        UART_putString(out);
      }
      UART_putString((uint8_t*)"***********************\n");
}

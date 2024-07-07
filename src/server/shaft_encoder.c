#include "shaft_encoder.h"
 
state_t enc[TOTAL_ENCODERS] = 
{
  {
    {
      .speed = 0,
      .old_cnt = 0,
      .target_speed = 0,
      .integral_err = 0,
      .output = 0
    },
    .counter = 0,
    .prev_value = 0b11,
    .curr_value = 0b11,
    .msk_0 = M_50,
    .msk_1 = M_52
  },
  {
    {
      .speed = 0,
      .old_cnt = 0,
      .target_speed = 0,
      .integral_err = 0,
      .output = 0
    },
    .counter = 0,
    .prev_value = 0b11,
    .curr_value = 0b11,
    .msk_0 = M_53,
    .msk_1 = M_51
  },
};

ISR (PCINT0_vect)
{
  update_encoder(enc, TOTAL_ENCODERS);
}

void setup_encoder(uint8_t mask)
{  
  // set as input
  DDRB &= ~mask;
  // enable pull up resistor
  PORTB |= mask;
  
  // set interrupt on change, looking up PCMSK0
  PCICR |= (1 << PCIE0); 

  // clear interrupt
  cli();
  // set PCINT0 to trigger an interrupt on state change 
  PCMSK0 |= mask;   
  // enable interrupt
  sei();
}

void update_encoder(state_t *enc, int tot_enc)
{
  for (int i = 0; i < tot_enc; i++) 
  {
    enc[i].prev_value = enc[i].curr_value;

    enc[i].curr_value = 
      ((PINB & enc[i].msk_1)? 0b10 : 0) | ((PINB & enc[i].msk_0)? 0b01 : 0);
    
    uint8_t idx = ((enc[i].prev_value << 2) | (enc[i].curr_value));
    enc[i].counter += _transition_table[idx];

  }
}

void print_status_encoder(state_t *enc, int tot_enc) 
{
  for (int i = 0; i < tot_enc; i++) 
  {     
    unsigned char out[1024];
    sprintf((char*) out, 
      "encoder: [%d]\n"
      "counter value = %d\n\n",
      i, 
      enc[i].counter
    );
    UART_putString(out);
  }
}

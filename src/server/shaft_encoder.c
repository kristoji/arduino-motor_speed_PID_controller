/**************************************************\
 * @file shaft_encoder.c                         
 *
 * @brief Implementation of functions to handle encoders
\**************************************************/
#include "shaft_encoder.h"


/**************************************************\
 * @brief The wheels state to control
\**************************************************/
state_t wheels[TOTAL_WHEELS] = 
{
  {
    {
      .counter = 0,
      .prev_value = 0b11,
      .curr_value = 0b11,
      .msk_0 = M_50,
      .msk_1 = M_52
    },
    .old_cnt = 0,
    .delta_enc = 0,
    .target_delta_enc = 0,
    .integral_err = 0,
    .output_pid = 0
  },
  {
    {
      .counter = 0,
      .prev_value = 0b11,
      .curr_value = 0b11,
      .msk_0 = M_53,
      .msk_1 = M_51
    },
    .old_cnt = 0,
    .delta_enc = 0,
    .target_delta_enc = 0,
    .integral_err = 0,
    .output_pid = 0
  },
};

ISR (PCINT0_vect)
{
  update_encoder(wheels, TOTAL_WHEELS);
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

void update_encoder(state_t *wheels, uint8_t tot_wheels)
{
  for (int i = 0; i < tot_wheels; i++) 
  {
    wheels[i].enc.prev_value = wheels[i].enc.curr_value;

    wheels[i].enc.curr_value = 
      ((PINB & wheels[i].enc.msk_1)? 0b10 : 0) | ((PINB & wheels[i].enc.msk_0)? 0b01 : 0);
    
    uint8_t idx = ((wheels[i].enc.prev_value << 2) | (wheels[i].enc.curr_value));
    wheels[i].enc.counter += _transition_table[idx];

  }
}

void print_status_encoder(state_t *wheels, uint8_t tot_wheels)
{
  for (int i = 0; i < tot_wheels; i++) 
  {     
    unsigned char out[1024];
    sprintf((char*) out, 
      "encoder: [%d]\n"
      "counter value = %d\n\n",
      i, 
      wheels[i].enc.counter
    );
    UART_putString(out);
  }
}

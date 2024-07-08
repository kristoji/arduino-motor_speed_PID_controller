/**************************************************\
 * @file time.c                         
 *
 * @brief Implementation of functions to handle time
\**************************************************/
#include "time.h"

/**************************************************\
 * @brief Flag to indicate the timer interrupt
\**************************************************/
volatile uint8_t timer_irq = 0;

void setup_update_timer()
{
    // set timer
    TCCR3A = 0;
    TCCR3B = TCCR3B_MASK;
    OCR3A = (uint16_t)(15.62*UPDATE_MS);
    
    cli();
    // enable the timer interrupt
    TIMSK3 |= (1 << OCIE3A);
    sei();
}

ISR(TIMER3_COMPA_vect)
{
  timer_irq = 1;
}
 